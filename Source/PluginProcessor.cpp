/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>
#include <fmt/format.h>

#if JUCE_WINDOWS
#ifdef JUCE_DEBUG
#pragma comment (lib, "fmtd.lib")
#else
#pragma comment (lib, "fmt.lib")
#endif
#endif

//==============================================================================
PascalEqAudioProcessor::PascalEqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),
#endif
    freqRange(20.0f, 20'000.0f,
        [](float start, float end, float value) { return std::powf(10.0f, value * 3.0f + std::log10f(20.0f)); },
        [](float start, float end, float value) { return (std::log10f(value) - std::log10f(20.0f)) / 3.0f; }
    ),
    parameters(*this, nullptr, Identifier("ParametersV1"),
        {
            std::make_unique<AudioParameterFloat>("filter1Freq", "Filter 1 Freq", freqRange, 80.0f,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f} Hz", value); }),
            std::make_unique<AudioParameterFloat>("filter1Gain", "Filter 1 Gain", NormalisableRange<float>(-15.0f, 15.0f), 0.0f,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f} dB", value); }),
            std::make_unique<AudioParameterFloat>("filter1Q", "Filter 1 Q", NormalisableRange<float>(0.1f, 20.0f, 0.0f, 0.4f), PascalFilter<float>::defaultQ,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f}", value); }),
            std::make_unique<AudioParameterBool>("filter1Enabled", "Filter 1 Enabled", true),

            std::make_unique<AudioParameterFloat>("filter2Freq", "Filter 2 Freq", freqRange, 500.0f,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f} Hz", value); }),
            std::make_unique<AudioParameterFloat>("filter2Gain", "Filter 2 Gain", NormalisableRange<float>(-15.0f, 15.0f), 0.0f,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f} dB", value); }),
            std::make_unique<AudioParameterFloat>("filter2Q", "Filter 2 Q", NormalisableRange<float>(0.1f, 20.0f, 0.0f, 0.4f), PascalFilter<float>::defaultQ,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f}", value); }),
            std::make_unique<AudioParameterBool>("filter2Enabled", "Filter 2 Enabled", true),

            std::make_unique<AudioParameterFloat>("filter3Freq", "Filter 3 Freq", freqRange, 2'000.0f,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f} Hz", value); }),
            std::make_unique<AudioParameterFloat>("filter3Gain", "Filter 3 Gain", NormalisableRange<float>(-15.0f, 15.0f), 0.0f,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f} dB", value); }),
            std::make_unique<AudioParameterFloat>("filter3Q", "Filter 3 Q", NormalisableRange<float>(0.1f, 20.0f, 0.0f, 0.4f), PascalFilter<float>::defaultQ,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f}", value); }),
            std::make_unique<AudioParameterBool>("filter3Enabled", "Filter 3 Enabled", true),

            std::make_unique<AudioParameterFloat>("filter4Freq", "Filter 4 Freq", freqRange, 8'000.0f,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f} Hz", value); }),
            std::make_unique<AudioParameterFloat>("filter4Gain", "Filter 4 Gain", NormalisableRange<float>(-15.0f, 15.0f), 0.0f,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f} dB", value); }),
            std::make_unique<AudioParameterFloat>("filter4Q", "Filter 4 Q", NormalisableRange<float>(0.1f, 20.0f, 0.0f, 0.4f), PascalFilter<float>::defaultQ,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f}", value); }),
            std::make_unique<AudioParameterBool>("filter4Enabled", "Filter 4 Enabled", true),

            std::make_unique<AudioParameterFloat>("filter5Freq", "Filter 5 Freq", freqRange, 16'000.0f,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f} Hz", value); }),
            std::make_unique<AudioParameterFloat>("filter5Gain", "Filter 5 Gain", NormalisableRange<float>(-15.0f, 15.0f), 0.0f,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f} dB", value); }),
            std::make_unique<AudioParameterFloat>("filter5Q", "Filter 5 Q", NormalisableRange<float>(0.1f, 20.0f, 0.0f, 0.4f), PascalFilter<float>::defaultQ,
                String(), AudioProcessorParameter::genericParameter, [](float value, int _) { return fmt::format("{:0.2f}", value); }),
            std::make_unique<AudioParameterBool>("filter5Enabled", "Filter 5 Enabled", true),

            std::make_unique<AudioParameterFloat>("outputGain", "Output Gain", -20.0f, 20.0f, 0.0f)
        })
{
    for (int i = 0; i < numFilters; ++i)
    {
        filtersFreq[i] = parameters.getRawParameterValue(fmt::format("filter{}Freq", i + 1));
        filtersGain[i] = parameters.getRawParameterValue(fmt::format("filter{}Gain", i + 1));
        filtersQ[i] = parameters.getRawParameterValue(fmt::format("filter{}Q", i + 1));
        filtersEnabled[i] = static_cast<AudioParameterBool*>(parameters.getParameter(fmt::format("filter{}Enabled", i + 1)));
    }

    outputGain = parameters.getRawParameterValue("outputGain");

    filters.resize(getTotalNumInputChannels());
}

PascalEqAudioProcessor::~PascalEqAudioProcessor()
{
    // Nothing to do here
}

//==============================================================================
const String PascalEqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PascalEqAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PascalEqAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PascalEqAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PascalEqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PascalEqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PascalEqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PascalEqAudioProcessor::setCurrentProgram(int index)
{
}

const String PascalEqAudioProcessor::getProgramName(int index)
{
    return {};
}

void PascalEqAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void PascalEqAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (int c = 0; c < getTotalNumInputChannels(); ++c)
    {
        for (int i = 0; i < numFilters; ++i)
        {
            if (filters[c][i])
            {
                filters[c][i]->setSampleRate(sampleRate);
            }
            else
            {
                switch (i)
                {
                case 0:
                    filters[c][i] = std::make_unique<LowShelfFilter<float>>(sampleRate, *filtersFreq[i], *filtersGain[i]);
                    break;
                case numFilters - 1:
                    filters[c][i] = std::make_unique<HighShelfFilter<float>>(sampleRate, *filtersFreq[i], *filtersGain[i]);
                    break;
                default:
                    filters[c][i] = std::make_unique<PeakFilter<float>>(sampleRate, *filtersFreq[i], *filtersGain[i], *filtersQ[i]);
                }
            }

            filters[c][i]->resetBuffers();
        }
    }
}

void PascalEqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PascalEqAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void PascalEqAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, numSamples);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    float outputGainValue = *outputGain;
    float linearOutputGainValue = Decibels::decibelsToGain(outputGainValue);

    for (int c = 0; c < totalNumInputChannels; ++c)
    {
        for (int i = 0; i < numFilters; ++i)
        {
            if (*filtersEnabled[i])
            {
                filters[c][i]->setParameters(*filtersFreq[i], *filtersGain[i], *filtersQ[i]);
                filters[c][i]->processBlock(buffer.getWritePointer(c), numSamples);
            }
        }
        buffer.applyGain(linearOutputGainValue);
    }
}

//==============================================================================
bool PascalEqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PascalEqAudioProcessor::createEditor()
{
    return new PascalEqAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void PascalEqAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xmlState(state.createXml());
    copyXmlToBinary(*xmlState, destData);
}

void PascalEqAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PascalEqAudioProcessor();
}
