/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ClockmakerAudioProcessor::ClockmakerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       ),
#endif
    parameters(*this, nullptr, "parameters", { std::make_unique<juce::AudioParameterInt> ("ppqn", "PPQN", 2, 96, 24) })
{
    parameters.addParameterListener("ppqn", this);   
}

ClockmakerAudioProcessor::~ClockmakerAudioProcessor()
{
}

//==============================================================================
void ClockmakerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "ppqn")
        dingusClock.SetPpqn(static_cast<int>(newValue));
}

//==============================================================================
const juce::String ClockmakerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ClockmakerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ClockmakerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ClockmakerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ClockmakerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ClockmakerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ClockmakerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ClockmakerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ClockmakerAudioProcessor::getProgramName (int index)
{
    return {};
}

void ClockmakerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ClockmakerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dingusClock.Init (sampleRate);
    dingusClock.SetPpqn (8);
}

void ClockmakerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ClockmakerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ClockmakerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    this->getPlayHead()->getCurrentPosition (currentPositionInfo);
    dingusClock.SetTempo(currentPositionInfo.bpm);

    // Interpolate the number of samples that have passed since the last downbeat
    double samplesPerQuarter = (60.0 / currentPositionInfo.bpm) * this->getSampleRate();
    double positionFrac = currentPositionInfo.ppqPosition - static_cast<int>(currentPositionInfo.ppqPosition);
    double sampleOffset = positionFrac * samplesPerQuarter;

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear the buffers for saftey.  If playhead is not moving this will set output to 0.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (currentPositionInfo.isPlaying || currentPositionInfo.isLooping || currentPositionInfo.isRecording)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            // Advance the clock position given the number of samples since the last downbeat
            dingusClock.SetTimeAdvance (sampleOffset);

            auto* channelData = buffer.getWritePointer (channel);

            for (int i = 0; i < buffer.getNumSamples(); i++)
            {
                channelData[i] = dingusClock.Process();
            }
        }
    }
}

//==============================================================================
bool ClockmakerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ClockmakerAudioProcessor::createEditor()
{
    return new ClockmakerAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void ClockmakerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ClockmakerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ClockmakerAudioProcessor();
}
