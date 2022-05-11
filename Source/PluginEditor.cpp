#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FilmStripKnob.h"

//==============================================================================
SubSyzorAudioProcessorEditor::SubSyzorAudioProcessorEditor(SubSyzorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    triangleMorph(juce::String("Knob256.png"), knobFile256, 256, false, 110, 110),
    triangleVolume(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    squareMorph(juce::String("Knob256.png"), knobFile256, 256, false, 110, 110),
    squareVolume(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    ampAttack(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    ampDecay(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    ampSustain(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    ampRelease(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    filterCutoff(juce::String("Knob256.png"), knobFile256, 256, false, 110, 110),
    filterMod(juce::String("Knob256.png"), knobFile256, 256, false, 40, 40),
    filterResonance(juce::String("Knob98.png"), knobFile098, 98, false, 70, 70),
    filterAttack(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    filterDecay(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    filterSustain(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    filterRelease(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    LfoRate(juce::String("Knob256.png"), knobFile256, 256, false, 110, 110),
    LfoDepth(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    phaserRate(juce::String("Knob256.png"), knobFile256, 256, false, 110, 110),
    phaserMix(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    outputVolume(juce::String("Knob256.png"), knobFile256, 256, false, 70, 70),
    lpButton(juce::String("LpButtonMix.png"), buttonLP, 40, 20),
    bpButton(juce::String("BpButtonMix.png"), buttonBP, 40, 20),
    hpButton(juce::String("HpButtonMix.png"), buttonHP, 40, 20),
    ampButton(juce::String("AmpButtonMix.png"), buttonAmp, 40, 20),
    filterButton(juce::String("FilterButtonMix.png"), buttonFilter, 40, 20),
    lfoButton(juce::String("LfoButtonMix.png"), buttonLFO, 40, 20),
    lfoTargetButton(juce::String("MorphButtonMix.png"), buttonLFOTarget, 40, 20),
    oscilloscope()
{
    Background = juce::ImageCache::getFromMemory(BinaryData::Background_png, BinaryData::Background_pngSize);
    LfoBackground = juce::ImageCache::getFromMemory(BinaryData::LfoBackground_png, BinaryData::LfoBackground_pngSize);
    AdsrBackground = juce::ImageCache::getFromMemory(BinaryData::AdsrBackground_png, BinaryData::AdsrBackground_pngSize);

    setSize(800, 501);

    // osc 1
    triangleMorph.resize(110, 110);
    triangleMorph.setBounds(90,20,110,110);
    triangleMorph.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    triangleMorph.setRange(0.0, 4.0, 0.01);
    triangleMorph.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    triangleMorph.onDragStart = [this]() {
        triangleMorph.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    triangleMorph.onDragEnd = [this]() {
        triangleMorph.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    triangleMorphAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "TriangleMorph", triangleMorph));
    addAndMakeVisible(triangleMorph);

    triangleVolume.resize(70, 70);
    triangleVolume.setBounds(230, 100, 70, 70);
    triangleVolume.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    triangleVolume.setRange(0.0, 1.0, 0.01);
    triangleVolume.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    triangleVolume.onDragStart = [this]() {
        triangleVolume.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    triangleVolume.onDragEnd = [this]() {
        triangleVolume.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    triangleVolumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "TriangleVolume", triangleVolume));
    addAndMakeVisible(triangleVolume);

    //osc 2
    squareMorph.resize(110, 110);
    squareMorph.setBounds(317, 20, 110, 110);
    squareMorph.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    squareMorph.setRange(0.0, 4.0, 0.01);
    squareMorph.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    squareMorph.onDragStart = [this]() {
        squareMorph.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    squareMorph.onDragEnd = [this]() {
        squareMorph.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    squareMorphAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "SquareMorph", squareMorph));
    addAndMakeVisible(squareMorph);

    squareVolume.resize(70, 70);
    squareVolume.setBounds(457, 100, 70, 70);
    squareVolume.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    squareVolume.setRange(0.0, 1.0, 0.01);
    squareVolume.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    squareVolume.onDragStart = [this]() {
        squareVolume.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    squareVolume.onDragEnd = [this]() {
        squareVolume.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    squareVolumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "SquareVolume", squareVolume));
    addAndMakeVisible(squareVolume);


    //amp section
    ampAttack.resize(70, 70);
    ampAttack.setBounds(538, 20, 70, 70);
    ampAttack.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ampAttack.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    ampAttack.onDragStart = [this]() {
        ampAttack.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    ampAttack.onDragEnd = [this]() {
        ampAttack.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    ampAttackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "ampAttack", ampAttack));
    addAndMakeVisible(ampAttack);

    ampDecay.resize(70, 70);
    ampDecay.setBounds(654, 20, 70, 70);
    ampDecay.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ampDecay.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    ampDecay.onDragStart = [this]() {
        ampDecay.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    ampDecay.onDragEnd = [this]() {
        ampDecay.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    ampDecayAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "ampDecay", ampDecay));
    addAndMakeVisible(ampDecay);

    ampSustain.resize(70, 70);
    ampSustain.setBounds(596, 100, 70, 70);
    ampSustain.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ampSustain.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    ampSustain.onDragStart = [this]() {
        ampSustain.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    ampSustain.onDragEnd = [this]() {
        ampSustain.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    ampSustainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "ampSustain", ampSustain));
    addAndMakeVisible(ampSustain);

    ampRelease.resize(70, 70);
    ampRelease.setBounds(712, 100, 70, 70);
    ampRelease.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ampRelease.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    ampRelease.onDragStart = [this]() {
        ampRelease.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    ampRelease.onDragEnd = [this]() {
        ampRelease.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    ampReleaseAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "ampRelease", ampRelease));
    addAndMakeVisible(ampRelease);


    //filter section
    filterCutoff.resize(110, 110);
    filterCutoff.setBounds(101, 380, 110, 110);
    filterCutoff.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);   
    filterCutoff.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    filterCutoff.onDragStart = [this]() {
        filterCutoff.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    filterCutoff.onDragEnd = [this]() {
        filterCutoff.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    filterCutoffAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "filterCutoff", filterCutoff));
    addAndMakeVisible(filterCutoff);

    filterMod.resize(40, 40);
    filterMod.setBounds(65, 454, 40, 40);
    filterMod.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterMod.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    filterMod.onDragStart = [this]() {
        filterMod.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    filterMod.onDragEnd = [this]() {
        filterMod.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    filterModAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "filterMod", filterMod));
    addAndMakeVisible(filterMod);

    filterResonance.resize(70, 70);
    filterResonance.setBounds(220, 330, 70, 70);
    filterResonance.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterResonance.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    filterResonance.onDragStart = [this]() {
        filterResonance.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    filterResonance.onDragEnd = [this]() {
        filterResonance.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    filterResonanceAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "filterResonance", filterResonance));
    addAndMakeVisible(filterResonance);


    // filter amp
    filterAttack.resize(70, 70);
    filterAttack.setBounds(538, 20, 70, 70);
    filterAttack.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterAttack.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    filterAttack.onDragStart = [this]() {
        filterAttack.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    filterAttack.onDragEnd = [this]() {
        filterAttack.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    filterAttackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "filterAttack", filterAttack));
    addAndMakeVisible(filterAttack);
    filterAttack.setVisible(false);

    filterDecay.resize(70, 70);
    filterDecay.setBounds(654, 20, 70, 70); 
    filterDecay.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterDecay.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    filterDecay.onDragStart = [this]() {
        filterDecay.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    filterDecay.onDragEnd = [this]() {
        filterDecay.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    filterDecayAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "filterDecay", filterDecay));
    addAndMakeVisible(filterDecay);
    filterDecay.setVisible(false);

    filterSustain.resize(70, 70);
    filterSustain.setBounds(596, 100, 70, 70);
    filterSustain.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterSustain.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    filterSustain.onDragStart = [this]() {
        filterSustain.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    filterSustain.onDragEnd = [this]() {
        filterSustain.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    filterSustainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "filterSustain", filterSustain));
    addAndMakeVisible(filterSustain);
    filterSustain.setVisible(false);

    filterRelease.resize(70, 70);
    filterRelease.setBounds(712, 100, 70, 70);
    filterRelease.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterRelease.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    filterRelease.onDragStart = [this]() {
        filterRelease.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    filterRelease.onDragEnd = [this]() {
        filterRelease.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    filterReleaseAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "filterRelease", filterRelease));
    addAndMakeVisible(filterRelease);
    filterRelease.setVisible(false);

    LfoRate.resize(110, 110);
    LfoRate.setBounds(553, 14, 110, 110);
    LfoRate.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    LfoRate.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    LfoRate.onDragStart = [this]() {
        LfoRate.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    LfoRate.onDragEnd = [this]() {
        LfoRate.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    LfoRateAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "LfoRate", LfoRate));
    addAndMakeVisible(LfoRate);
    LfoRate.setVisible(false);

    LfoDepth.resize(70, 70);
    LfoDepth.setBounds(678, 99, 70, 70);
    LfoDepth.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    LfoDepth.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    LfoDepth.onDragStart = [this]() {
        LfoDepth.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    LfoDepth.onDragEnd = [this]() {
        LfoDepth.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    LfoDepthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "LfoDepth", LfoDepth));
    addAndMakeVisible(LfoDepth);
    LfoDepth.setVisible(false);

    phaserRate.resize(110, 110);
    phaserRate.setBounds(307, 380, 110, 110);
    phaserRate.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    phaserRate.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    phaserRate.onDragStart = [this]() {
        phaserRate.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    phaserRate.onDragEnd = [this]() {
        phaserRate.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    phaserRateAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "phaserRate", phaserRate));
    addAndMakeVisible(phaserRate);

    phaserMix.resize(70, 70);
    phaserMix.setBounds(450, 356, 70, 70);
    phaserMix.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    phaserMix.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    phaserMix.onDragStart = [this]() {
        phaserMix.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    phaserMix.onDragEnd = [this]() {
        phaserMix.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    phaserMixAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "phaserMix", phaserMix));
    addAndMakeVisible(phaserMix);

    outputVolume.resize(70, 70);
    outputVolume.setBounds(530, 420, 70, 70);
    outputVolume.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    outputVolume.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    outputVolume.onDragStart = [this]() {
        outputVolume.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    };
    outputVolume.onDragEnd = [this]() {
        outputVolume.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    };
    outputVolumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.states, "outputVolume", outputVolume));
    addAndMakeVisible(outputVolume);

    lpButton.setBounds(225, 409, 40, 20);
    lpButton.setToggleState(true, juce::dontSendNotification);
    lpButton.onClick = [this]() {
        lpButton.setToggleState(true, juce::dontSendNotification);
        bpButton.setToggleState(false, juce::dontSendNotification);
        hpButton.setToggleState(false, juce::dontSendNotification);
        lpButton.repaint();
        bpButton.repaint();
        hpButton.repaint();
        audioProcessor.changeFilterType(0);
    };
    addAndMakeVisible(lpButton);

    bpButton.setBounds(225, 433, 40, 20);
    bpButton.setToggleState(false, juce::dontSendNotification);
    bpButton.onClick = [this]() {
        lpButton.setToggleState(false, juce::dontSendNotification);
        bpButton.setToggleState(true, juce::dontSendNotification);
        hpButton.setToggleState(false, juce::dontSendNotification);
        lpButton.repaint();
        bpButton.repaint();
        hpButton.repaint();
        audioProcessor.changeFilterType(1);
    };
    addAndMakeVisible(bpButton);

    hpButton.setBounds(225, 457, 40, 20);
    hpButton.setToggleState(false, juce::dontSendNotification);
    hpButton.onClick = [this]() {
        lpButton.setToggleState(false, juce::dontSendNotification);
        bpButton.setToggleState(false, juce::dontSendNotification);
        hpButton.setToggleState(true, juce::dontSendNotification);
        lpButton.repaint();
        bpButton.repaint();
        hpButton.repaint();
        audioProcessor.changeFilterType(2);
    };
    addAndMakeVisible(hpButton);

    ampButton.setBounds(756, 4, 40, 20);
    ampButton.setToggleState(true, juce::dontSendNotification);
    ampButton.onClick = [this]() {
        ampButton.setToggleState(true, juce::dontSendNotification);
        filterButton.setToggleState(false, juce::dontSendNotification);
        lfoButton.setToggleState(false, juce::dontSendNotification);
        ampButton.repaint();
        filterButton.repaint();
        lfoButton.repaint();
        ampAttack.setVisible(true);
        ampDecay.setVisible(true);
        ampSustain.setVisible(true);
        ampRelease.setVisible(true);
        filterAttack.setVisible(false);
        filterDecay.setVisible(false);
        filterSustain.setVisible(false);
        filterRelease.setVisible(false);
        LfoRate.setVisible(false);
        LfoDepth.setVisible(false);
        lfoTargetButton.setVisible(false);
        showAdsr = true;
        repaint();
    };
    addAndMakeVisible(ampButton);

    filterButton.setBounds(756, 28, 40, 20);
    filterButton.setToggleState(false, juce::dontSendNotification);
    filterButton.onClick = [this]() {
        ampButton.setToggleState(false, juce::dontSendNotification);
        filterButton.setToggleState(true, juce::dontSendNotification);
        lfoButton.setToggleState(false, juce::dontSendNotification);
        ampButton.repaint();
        filterButton.repaint();
        lfoButton.repaint();
        ampAttack.setVisible(false);
        ampDecay.setVisible(false);
        ampSustain.setVisible(false);
        ampRelease.setVisible(false);
        filterAttack.setVisible(true);
        filterDecay.setVisible(true);
        filterSustain.setVisible(true);
        filterRelease.setVisible(true);
        LfoRate.setVisible(false);
        LfoDepth.setVisible(false);
        lfoTargetButton.setVisible(false);
        showAdsr = true;
        repaint();
    };
    addAndMakeVisible(filterButton);

    lfoButton.setBounds(756, 52, 40, 20);
    lfoButton.setToggleState(false, juce::dontSendNotification);
    lfoButton.onClick = [this]() {
        ampButton.setToggleState(false, juce::dontSendNotification);
        filterButton.setToggleState(false, juce::dontSendNotification);
        lfoButton.setToggleState(true, juce::dontSendNotification);
        ampButton.repaint();
        filterButton.repaint();
        lfoButton.repaint();
        ampAttack.setVisible(false);
        ampDecay.setVisible(false);
        ampSustain.setVisible(false);
        ampRelease.setVisible(false);
        filterAttack.setVisible(false);
        filterDecay.setVisible(false);
        filterSustain.setVisible(false);
        filterRelease.setVisible(false);
        LfoRate.setVisible(true);
        LfoDepth.setVisible(true);
        lfoTargetButton.setVisible(true);
        showAdsr = false;
        repaint();
    };
    addAndMakeVisible(lfoButton);

    lfoTargetButton.setBounds(596, 140, 40, 20);
    lfoTargetButton.setToggleState(false, juce::dontSendNotification);
    audioProcessor.changeLfoTarget(lfoTargetButton.getToggleState());
    lfoTargetButton.onClick = [this]() {
        lfoTargetButton.setToggleState(!lfoTargetButton.getToggleState(), juce::dontSendNotification);
        audioProcessor.changeLfoTarget(lfoTargetButton.getToggleState());
        repaint();
    };
    addAndMakeVisible(lfoTargetButton);
    lfoTargetButton.setVisible(false);

    oscilloscope.setBounds(585, 330, 200, 80);
    addAndMakeVisible(oscilloscope);

    startTimer(1000 / 60);
}

SubSyzorAudioProcessorEditor::~SubSyzorAudioProcessorEditor()
{
}

//==============================================================================
void SubSyzorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    juce::Rectangle<float> windowArea(0, 0, 800.0, 501.0);
    g.drawImage(Background, windowArea);
    if(showAdsr)
        g.drawImage(AdsrBackground, windowArea);
    else
        g.drawImage(LfoBackground, windowArea);
}

void SubSyzorAudioProcessorEditor::resized()
{

}

void SubSyzorAudioProcessorEditor::timerCallback()
{
    oscilloscope.processBlock(*audioProcessor.buffer_meter.getArrayOfReadPointers(), audioProcessor.buffer_meter.getNumSamples());
}

void SubSyzorAudioProcessorEditor::sliderValueChanged(juce::Slider* slider){
    /*
    if(slider == &squarenessSlider)
        //SET THE SQUARENESS
        audioProcessor.setSquareness(squarenessSlider.getValue());
    else if (slider == &trianglenessSlider)
        //SET HOW TRIANGULAR THE WAVE WILL BE
        audioProcessor.setTriangleness(trianglenessSlider.getValue());
    
    else if (slider == &squareGainSlider)
        audioProcessor.setSquareGain(squareGainSlider.getValue());
    
    else if (slider == &triangleGainSlider)
        audioProcessor.setTriangleGain(triangleGainSlider.getValue());
    
    else if (slider == &volumeSlider)
        audioProcessor.setVolume(volumeSlider.getValue());
    */
}
