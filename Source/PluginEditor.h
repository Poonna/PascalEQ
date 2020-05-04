/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

//==============================================================================
/**
*/
class PascalEqAudioProcessorEditor : public AudioProcessorEditor
{
public:
    PascalEqAudioProcessorEditor(PascalEqAudioProcessor&, AudioProcessorValueTreeState&);
    ~PascalEqAudioProcessorEditor();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PascalEqAudioProcessor& processor;
    AudioProcessorValueTreeState& parameters;

    Slider filtersFreq[PascalEqAudioProcessor::numFilters];
    Slider filtersGain[PascalEqAudioProcessor::numFilters];
    Slider filtersQ[PascalEqAudioProcessor::numFilters];
    ToggleButton filtersEnabled[PascalEqAudioProcessor::numFilters];

    Slider outputGain;
    ToggleButton noiseGateEnabled;

    std::unique_ptr<SliderAttachment> filtersFreqAttachment[PascalEqAudioProcessor::numFilters];
    std::unique_ptr<SliderAttachment> filtersGainAttachment[PascalEqAudioProcessor::numFilters];
    std::unique_ptr<SliderAttachment> filtersQAttachment[PascalEqAudioProcessor::numFilters];
    std::unique_ptr<ButtonAttachment> filtersEnabledAttachment[PascalEqAudioProcessor::numFilters];
    std::unique_ptr<SliderAttachment> outputGainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PascalEqAudioProcessorEditor)
};
