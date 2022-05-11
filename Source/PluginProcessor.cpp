#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubSyzorAudioProcessor::SubSyzorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    states(*this, nullptr, "parameters", {
            std::make_unique<juce::AudioParameterFloat>("TriangleMorph", "Triangle Morph", juce::NormalisableRange<float>(0.0f, 4.0f), 4.0f),
            std::make_unique<juce::AudioParameterFloat>("TriangleVolume", "Triangle Volume", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f),
            std::make_unique<juce::AudioParameterFloat>("SquareMorph", "Square Morph", juce::NormalisableRange<float>(0.0f, 4.0f), 4.0f),
            std::make_unique<juce::AudioParameterFloat>("SquareVolume", "Square Volume", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f),
            std::make_unique<juce::AudioParameterFloat>("ampAttack", "Amplitude Attack", juce::NormalisableRange<float>(0.0f, 2.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("ampDecay", "Amplitude Decay", juce::NormalisableRange<float>(0.0f, 4.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("ampSustain", "Amplitude Sustain", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f),
            std::make_unique<juce::AudioParameterFloat>("ampRelease", "Amplitude Release", juce::NormalisableRange<float>(0.0f, 4.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("filterAttack", "Filter Attack", juce::NormalisableRange<float>(0.0f, 2.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("filterDecay", "Filter Decay", juce::NormalisableRange<float>(0.0f, 2.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("filterSustain", "Filter Sustain", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f),
            std::make_unique<juce::AudioParameterFloat>("filterRelease", "Filter Release", juce::NormalisableRange<float>(0.0f, 4.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("LfoRate", "LFO Rate", juce::NormalisableRange<float>(0.1f, 50.0f), 0.1f),
            std::make_unique<juce::AudioParameterFloat>("LfoDepth", "LFO Depth", juce::NormalisableRange<float>(0.0f, 2.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("filterCutoff", "Filter Cutoff", juce::NormalisableRange<float>(20.0f, 20000.0f), 9990.0f),   //straved away from given parameters as these options seem better
            std::make_unique<juce::AudioParameterFloat>("filterMod", "Filter Modulation", juce::NormalisableRange<float>(-100.0f, 100.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("filterResonance", "Filter Resonance", juce::NormalisableRange<float>(0.1f, 12.0f), 0.1f), //straved away from given parameters as these options seem better
            std::make_unique<juce::AudioParameterFloat>("phaserRate", "Phaser Rate", juce::NormalisableRange<float>(0.0f, 10.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("phaserMix", "Phaser Mix", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("outputVolume", "Output Volume", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f)
        })
#endif
{
}

SubSyzorAudioProcessor::~SubSyzorAudioProcessor()
{
}

//==============================================================================
const juce::String SubSyzorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SubSyzorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SubSyzorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SubSyzorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SubSyzorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SubSyzorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SubSyzorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SubSyzorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SubSyzorAudioProcessor::getProgramName (int index)
{
    return {};
}

void SubSyzorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SubSyzorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{    
    //AMPLITUDE ADSR INITIALIZATION
    envParams.attack = states.getParameterAsValue("ampAttack").getValue();
    envParams.decay = states.getParameterAsValue("ampDecay").getValue();
    envParams.sustain = states.getParameterAsValue("ampSustain").getValue();
    envParams.release = states.getParameterAsValue("ampRelease").getValue();
    
    //FILTER ADSR INITIALIZATION
    filterEnvParams.attack = (float)states.getParameterAsValue("filterAttack").getValue() / samplesPerBlock;
    filterEnvParams.decay = (float)states.getParameterAsValue("filterDecay").getValue() / samplesPerBlock;
    filterEnvParams.sustain = states.getParameterAsValue("filterSustain").getValue();
    filterEnvParams.release = (float)states.getParameterAsValue("filterRelease").getValue() / samplesPerBlock;
    
    //DSP STUFF INITIALIZATION
    currentSampleRate = sampleRate;
    
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;
    spec.maximumBlockSize = samplesPerBlock;
    
    for (int i = 0; i < voiceCount; i++) {
        
        osc[i] = voice(0, 0.0, envParams, filterEnvParams, sampleRate, spec);
        osc[i].setSquareness(states.getParameterAsValue("SquareMorph").getValue());
        osc[i].setTriangleness(states.getParameterAsValue("TriangleMorph").getValue());
        osc[i].setSquareGain(states.getParameterAsValue("SquareVolume").getValue());
        osc[i].setTriangleGain(states.getParameterAsValue("TriangleVolume").getValue());
        osc[i].setCutoff((float)states.getParameterAsValue("filterCutoff").getValue());
        osc[i].setResonance((float)states.getParameterAsValue("filterResonance").getValue());
        age[i] = 0;
        //DBG("Resonance: " << (float)states.getParameterAsValue("filterResonance").getValue());
        osc[i].prepareToFilter(sampleRate, samplesPerBlock, this->getMainBusNumOutputChannels());
    }
    
    buffer_temp = juce::AudioBuffer<float>(1, samplesPerBlock);
    buffer_sum = juce::AudioBuffer<float>(1, samplesPerBlock);
    
    //LFO INITIALIZATION
    lfoSwitch = true; //true: morph, false: pitch
    lfoAmp = 1.0;
    lfoDepth = (float)states.getParameterAsValue("LfoDepth").getValue();
    lfoFreq = (float)states.getParameterAsValue("LfoRate").getValue();
    lfoPhase = 0.0;
    squarenessMod = 0.0;
    trianglenessMod = 0.0;
    
    //PHAZOR INITIALIZATION
    phRate = (float)states.getParameterAsValue("phaserRate").getValue();
    phDepth = 1.0;
    phCenterFreq = 440.0;
    phFeedback = 0.5;
    phMix = (float)states.getParameterAsValue("phaserMix").getValue();
    
    phazor.reset();
    phazor.setRate(phRate);
    phazor.setDepth(phDepth);
    phazor.setCentreFrequency(phCenterFreq);
    phazor.setFeedback(phFeedback);
    phazor.setMix(phMix);
    phazor.prepare(spec);
    
    //MASTER VOLUME
    masterVolume.prepare(spec);
    masterVolume.setGainLinear(0.1 * (float)states.getParameterAsValue("outputVolume").getValue());

    this->samplesPerBlock = samplesPerBlock;
}

void SubSyzorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SubSyzorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SubSyzorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    float squareness = states.getParameterAsValue("SquareMorph").getValue();
    float triangleness = states.getParameterAsValue("TriangleMorph").getValue();
    float squareGain = states.getParameterAsValue("SquareVolume").getValue();
    float triangleGain = states.getParameterAsValue("TriangleVolume").getValue();

    float cutoff = (float)states.getParameterAsValue("filterCutoff").getValue();
    float mod = (float)states.getParameterAsValue("filterMod").getValue();
    float resonance = (float)states.getParameterAsValue("filterResonance").getValue();

    envParams.attack = states.getParameterAsValue("ampAttack").getValue();
    envParams.decay = states.getParameterAsValue("ampDecay").getValue();
    envParams.sustain = states.getParameterAsValue("ampSustain").getValue();
    envParams.release = states.getParameterAsValue("ampRelease").getValue();

    filterEnvParams.attack = (float)states.getParameterAsValue("filterAttack").getValue() / samplesPerBlock;
    filterEnvParams.decay = (float)states.getParameterAsValue("filterDecay").getValue() / samplesPerBlock;
    filterEnvParams.sustain = states.getParameterAsValue("filterSustain").getValue();
    filterEnvParams.release = (float)states.getParameterAsValue("filterRelease").getValue() / samplesPerBlock;

    phazor.setMix((float)states.getParameterAsValue("phaserMix").getValue());
    phazor.setRate((float)states.getParameterAsValue("phaserRate").getValue());
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::MidiMessage m;
    int time;

    //Iteration between all midi messages
    for (juce::MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);) {
        if (m.isNoteOn()) {
            int currentNote = m.getNoteNumber();
            bool found = false;
            
            //we want to play a new note
            //We first look for either a free oscillator or one that's already playing the same note we want to play next
            for(int j = 0; j < voiceCount && found == false; j++){
                if(!osc[j].isPlaying() || osc[j].getNote() == currentNote){
                    osc[j].setEnvParameters(envParams);
                    osc[j].setFilterEnvParameters(filterEnvParams);
                    osc[j].setCutoff(cutoff);
                    osc[j].setRange(mod);
                    osc[j].setResonance(resonance);
                    osc[j].resetEnv();
                    osc[j].resetFilterEnv();
                    osc[j].setFreq(m.getMidiNoteInHertz(currentNote));
                    osc[j].setNote(currentNote);
                    osc[j].startEnv();
                    osc[j].startFilterEnv();
                    note[j] = m.getMidiNoteInHertz(currentNote);
                    
                    found = true;
                }
            }
            //if there's no free oscillator we look for the one that has been currently playing the longest and overwrite it to the new frequency
            if(!found){
                int oldest = 0;
                for(int k = 0; k < voiceCount; k++){
                    if(age[k] > age[oldest])
                        oldest = k;
                }
                osc[oldest].setEnvParameters(envParams);
                osc[oldest].setFilterEnvParameters(filterEnvParams);
                osc[oldest].setCutoff(cutoff);
                osc[oldest].setRange(mod);
                osc[oldest].setResonance(resonance);
                osc[oldest].setFreq(m.getMidiNoteInHertz(currentNote));
                osc[oldest].setNote(currentNote);
                osc[oldest].startEnv();
                osc[oldest].startFilterEnv();
                note[oldest] = m.getMidiNoteInHertz(currentNote);
                
                age[oldest] = 0;
            }
        }
        //If a NoteOff message is received we look for the oscillator which is playing the same note number as the one in the message and we initialize the release sequence of the ADSR
        else if (m.isNoteOff()) {
            int currentNote = m.getNoteNumber();
            bool found = false;
            
            for(int j = 0; j < voiceCount && found == false; j++){
                if(osc[j].getNote() == currentNote){
                    osc[j].endEnv();
                    osc[j].endFilterEnv();
                    
                    found = true;
                }
            }
        }
    }
    
    int numSamples = buffer.getNumSamples();
    
    lfoDepth = (float)states.getParameterAsValue("LfoDepth").getValue();
    lfoFreq = (float)states.getParameterAsValue("LfoRate").getValue();
    //Compute the value of the lfo for each bufferSize
    lfoAmp = sin(lfoPhase);
    lfoPhase += doublePi*(float)(lfoFreq/currentSampleRate)*(float)numSamples;

    if(lfoPhase > doublePi)
        lfoPhase-=doublePi;
        
    if (lfoSwitch) {
        //Compute the modulated value of both Morph parameters
        squarenessMod = squareness + (lfoDepth * 2 * lfoAmp);
        trianglenessMod = triangleness + (lfoDepth * 2 * lfoAmp);

        //Chech if the modulated parameters are inside the possible values
        if (trianglenessMod < 0.0)
            trianglenessMod = 0.0;

        if (squarenessMod < 0.0)
            squarenessMod = 0.0;
        else if (squarenessMod > 4.0)
            squarenessMod = 4.0;
    }
    else {
        // Computation and setting of the new values of frequency to alterate the pitch in each playing osc
        for (int i = 0; i < voiceCount; i++) {
            osc[i].setFreq(note[i] * pow(2, (lfoDepth * lfoAmp / 6.0)));
        }
    }
    
    
    //Actually making the Oscillators play notes
    buffer_sum.clear(0, 0, numSamples);

    for (int i = 0; i < voiceCount; i++){
        //Choosing the behaviour if the LFO is active
        if(lfoSwitch){
            osc[i].setSquareness(squarenessMod);
            osc[i].setTriangleness(trianglenessMod);
        }
        else{
            osc[i].setSquareness(squareness);
            osc[i].setTriangleness(triangleness);
        }
        //We write to a mono buffer, sum to another temporary buffer and then copy that into the output buffer. Also we reset the "age" parameter of the oscillator while we are checking of it's playing
        if(osc[i].isPlaying()){
            osc[i].setSquareGain(squareGain);
            osc[i].setTriangleGain(triangleGain);
            osc[i].setEnvParameters(envParams);
            osc[i].setFilterEnvParameters(filterEnvParams);
            osc[i].setCutoff(cutoff);
            osc[i].setRange(mod);
            osc[i].setResonance(resonance);
            buffer_temp = osc[i].generateBuffer(numSamples); //wave synthesis
            osc[i].processFilter(buffer_temp); //single voice filtering
            age[i] += 1;
            buffer_sum.addFrom(0, 0, buffer_temp, 0, 0, numSamples);
        }
        else
            age[i] = 0;
    }

    buffer.copyFrom(0, 0, buffer_sum, 0, 0, numSamples);
    buffer.copyFrom(1, 0, buffer_sum, 0, 0, numSamples);
    
    //Master Output volume and phaser
    juce::dsp::AudioBlock<float> block(buffer);
    
    phazor.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    masterVolume.setGainLinear(0.1 * (float)states.getParameterAsValue("outputVolume").getValue());
    masterVolume.process(juce::dsp::ProcessContextReplacing<float>(block));

    buffer_meter = buffer;
}

//==============================================================================
bool SubSyzorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SubSyzorAudioProcessor::createEditor()
{
    return new SubSyzorAudioProcessorEditor (*this);
}

//==============================================================================
void SubSyzorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SubSyzorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SubSyzorAudioProcessor();
}

void SubSyzorAudioProcessor::changeFilterType(int choice) {
    for (int j = 0; j < voiceCount; j++) 
        osc[j].setMenuChoice(choice);
}

void SubSyzorAudioProcessor::changeLfoTarget(bool val) {
    lfoSwitch = val;
}