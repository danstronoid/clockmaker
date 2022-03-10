/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ClockmakerAudioProcessorEditor::ClockmakerAudioProcessorEditor (ClockmakerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), parameters(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 200);

    ppqnSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    ppqnSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    ppqnAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(parameters, "ppqn", ppqnSlider));
    addAndMakeVisible(&ppqnSlider);

    ppqnBoxLabel.setText("PPQN", juce::dontSendNotification);
    ppqnBoxLabel.setJustificationType(juce::Justification::centred);
    ppqnBoxLabel.attachToComponent(&ppqnSlider, false);
    addAndMakeVisible(&ppqnBoxLabel);

    mulDivSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mulDivSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    mulDivAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(parameters, "mulDiv", mulDivSlider));
    addAndMakeVisible(&mulDivSlider);

    mulDivLabel.setText("Mul/Div", juce::dontSendNotification);
    mulDivLabel.setJustificationType(juce::Justification::centred);
    mulDivLabel.attachToComponent(&mulDivSlider, false);
    addAndMakeVisible(&mulDivLabel);

    setLookAndFeel(&style);
}

ClockmakerAudioProcessorEditor::~ClockmakerAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void ClockmakerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (40.0f);
    g.drawFittedText ("CLOCKMAKER", getLocalBounds().reduced(padding), juce::Justification::centredTop, 1);
}

void ClockmakerAudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(padding);

    int componentWidth = area.getWidth();
    int componentHeight = area.getHeight() / 2;

    area.removeFromTop(componentHeight);

    ppqnSlider.setSize(componentWidth / 2, componentHeight);
    ppqnSlider.setBoundsToFit(area.removeFromLeft(componentHeight), juce::Justification::centred, true);

    mulDivSlider.setSize(componentWidth / 2, componentHeight);
    mulDivSlider.setBoundsToFit(area.removeFromLeft(componentHeight), juce::Justification::centred, true);
}
