/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <fmt/format.h>

#if JUCE_WINDOWS
#ifdef JUCE_DEBUG
#pragma comment (lib, "fmtd.lib")
#else
#pragma comment (lib, "fmt.lib")
#endif
#endif

//==============================================================================
PascalEqAudioProcessorEditor::PascalEqAudioProcessorEditor(PascalEqAudioProcessor& p, AudioProcessorValueTreeState& v)
    : AudioProcessorEditor(&p), processor(p), parameters(v)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(950, 400);

    for (int i = 0; i < 5; ++i)
    {
        filtersFreq[i].setSliderStyle(Slider::RotaryVerticalDrag);
        addAndMakeVisible(&filtersFreq[i]);
        filtersFreqAttachment[i].reset(new SliderAttachment(parameters, fmt::format("filter{}Freq", i + 1), filtersFreq[i]));

        filtersGain[i].setSliderStyle(Slider::RotaryVerticalDrag);
        addAndMakeVisible(&filtersGain[i]);
        filtersGainAttachment[i].reset(new SliderAttachment(parameters, fmt::format("filter{}Gain", i + 1), filtersGain[i]));

        if (i != 0 && i != 4)
        {
            filtersQ[i].setSliderStyle(Slider::RotaryVerticalDrag);
            addAndMakeVisible(&filtersQ[i]);
            filtersQAttachment[i].reset(new SliderAttachment(parameters, fmt::format("filter{}Q", i + 1), filtersQ[i]));
        }

        addAndMakeVisible(&filtersEnabled[i]);
        filtersEnabledAttachment[i].reset(new ButtonAttachment(parameters, fmt::format("filter{}Enabled", i + 1), filtersEnabled[i]));
    }

    outputGain.setSliderStyle(Slider::RotaryVerticalDrag);
    outputGain.setTextValueSuffix(" dB");
    addAndMakeVisible(&outputGain);
    outputGainAttachment.reset(new SliderAttachment(parameters, "outputGain", outputGain));
}

PascalEqAudioProcessorEditor::~PascalEqAudioProcessorEditor()
{
}

//==============================================================================
void PascalEqAudioProcessorEditor::paint(Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::darkgrey);

    g.setColour(Colours::black);
    g.setFont(15.0f);
    g.drawFittedText("Version 1", getLocalBounds(), Justification::bottomLeft, 1);
}

void PascalEqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    for (int i = 0; i < 5; ++i)
    {
        filtersFreq[i].setBounds(50 + i * 150, 20, 80, 100);
        filtersFreq[i].setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

        filtersGain[i].setBounds(40 + i * 150, 120, 100, 120);
        filtersGain[i].setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        
        if (i != 0 && i != 4)
        {
            filtersQ[i].setBounds(50 + i * 150, 240, 80, 100);
            filtersQ[i].setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        }

        filtersEnabled[i].setBounds(75 + i * 150, 350, 30, 30);
    }

    outputGain.setBounds(800, 50, 100, 120);
    outputGain.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
}
