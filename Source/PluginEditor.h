#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FilmStripKnob.h"
#include "imageButton.h"
#include "dRowAudio_AudioOscilloscope.h"

//==============================================================================
/**
*/
class SubSyzorAudioProcessorEditor  :
    public juce::AudioProcessorEditor,
    private juce::Slider::Listener,
    public juce::Timer
{
public:
    SubSyzorAudioProcessorEditor (SubSyzorAudioProcessor&);
    ~SubSyzorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SubSyzorAudioProcessor& audioProcessor;
    
    //WAVE PARAMETERS
    juce::Slider squarenessSlider;
    juce::Slider trianglenessSlider;
    
    juce::Label squarenessLabel;
    juce::Label trianglenessLabel;

    //VOLUME & GAIN FOR EACH OSCILLATOR VOICE
    juce::Slider squareGainSlider;
    juce::Slider triangleGainSlider;
    juce::Slider volumeSlider;
    
    juce::Label squareGainLabel;
    juce::Label triangleGainLabel;
    juce::Label volumeLabel;

    juce::Image Background;
    juce::Image AdsrBackground;
    juce::Image LfoBackground;

    juce::Image knobFile256 = juce::ImageCache::getFromMemory(BinaryData::Knob256_png, BinaryData::Knob256_pngSize);
    juce::Image knobFile101 = juce::ImageCache::getFromMemory(BinaryData::Knob101_png, BinaryData::Knob101_pngSize);
    juce::Image knobFile098 = juce::ImageCache::getFromMemory(BinaryData::Knob98_png, BinaryData::Knob98_pngSize);

    juce::Image buttonBP = juce::ImageCache::getFromMemory(BinaryData::BpButtonMix_png, BinaryData::BpButtonMix_pngSize);
    juce::Image buttonLP = juce::ImageCache::getFromMemory(BinaryData::LpButtonMix_png, BinaryData::LpButtonMix_pngSize);
    juce::Image buttonHP = juce::ImageCache::getFromMemory(BinaryData::HpButtonMix_png, BinaryData::HpButtonMix_pngSize);

    juce::Image buttonLFO = juce::ImageCache::getFromMemory(BinaryData::LfoButtonMix_png, BinaryData::LfoButtonMix_pngSize);
    juce::Image buttonFilter = juce::ImageCache::getFromMemory(BinaryData::FilterButtonMix_png, BinaryData::FilterButtonMix_pngSize);
    juce::Image buttonAmp = juce::ImageCache::getFromMemory(BinaryData::AmpButtonMix_png, BinaryData::AmpButtonMix_pngSize);

    juce::Image buttonLFOTarget = juce::ImageCache::getFromMemory(BinaryData::MorphButtonMix_png, BinaryData::MorphButtonMix_pngSize);
    
    FilmStripKnob triangleMorph;
    FilmStripKnob triangleVolume;
    FilmStripKnob squareMorph;
    FilmStripKnob squareVolume;
    FilmStripKnob ampAttack;
    FilmStripKnob ampDecay;
    FilmStripKnob ampSustain;
    FilmStripKnob ampRelease;
    FilmStripKnob filterAttack;
    FilmStripKnob filterDecay;
    FilmStripKnob filterSustain;
    FilmStripKnob filterRelease;
    FilmStripKnob LfoRate;
    FilmStripKnob LfoDepth;
    FilmStripKnob filterCutoff;
    FilmStripKnob filterMod;
    FilmStripKnob filterResonance;
    FilmStripKnob phaserRate;
    FilmStripKnob phaserMix;
    FilmStripKnob outputVolume;

    ImageButton lpButton;
    ImageButton bpButton;
    ImageButton hpButton;

    ImageButton ampButton;
    ImageButton filterButton;
    ImageButton lfoButton;

    ImageButton lfoTargetButton;

    bool showAdsr = true;

    AudioOscilloscope oscilloscope;

    void sliderValueChanged(juce::Slider* slider) override;
    
    //OSC TRIANGLE
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> triangleMorphAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> triangleVolumeAttachment;

    //OSC SQUARE
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> squareMorphAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> squareVolumeAttachment;

    //AMP ENV
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampDecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampSustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampReleaseAttachment;

    //FILTER
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterModAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;

    //FILTER ENV
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterDecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterSustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterReleaseAttachment;

    //LFO
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LfoRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LfoDepthAttachment;

    //PHASER
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserMixAttachment;

    //OUTPUT VOLUME
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputVolumeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubSyzorAudioProcessorEditor)
};
