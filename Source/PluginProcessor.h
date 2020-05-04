/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PascalFilter.h"

//==============================================================================
/**
*/
class PascalEqAudioProcessor : public AudioProcessor
{
public:
    //==============================================================================
    PascalEqAudioProcessor();
    ~PascalEqAudioProcessor();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    //==============================================================================
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    enum class FilterType {
        LOW_SHELF,
        HIGH_SHELF,
        PEAK
    };

    static const int numFilters = 5;

private:
    //==============================================================================
    NormalisableRange<float> freqRange;

    AudioProcessorValueTreeState parameters;

    std::vector<std::array<std::unique_ptr<PascalFilter<float>>, numFilters>> filters;

    std::atomic<float>* filtersFreq[numFilters];
    std::atomic<float>* filtersGain[numFilters];
    std::atomic<float>* filtersQ[numFilters];
    AudioParameterBool* filtersEnabled[numFilters];

    std::atomic<float>* outputGain = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PascalEqAudioProcessor)
};
