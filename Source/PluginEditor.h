/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Style.h"

//==============================================================================
/**
*/
class ClockmakerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ClockmakerAudioProcessorEditor (ClockmakerAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~ClockmakerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ClockmakerAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& parameters;

    dingus_dsp::Style style;

    juce::Slider ppqnSlider;
    juce::Label ppqnBoxLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ppqnAttach;

    juce::Slider mulDivSlider;
    juce::Label mulDivLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mulDivAttach;

    const int padding = 10;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClockmakerAudioProcessorEditor)
};
