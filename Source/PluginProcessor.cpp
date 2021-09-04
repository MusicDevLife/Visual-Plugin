/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VisualAudioProcessor::VisualAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{

    state = new AudioProcessorValueTreeState(*this, nullptr);

    state->createAndAddParameter("Drive", "Drive", "Drive", NormalisableRange<float>(0.01f, 1.f, 0.0001), 1.0, nullptr, nullptr);
    state->createAndAddParameter("Distortion", "Distortion", "Distortion", NormalisableRange<float>(0.01f, 25.f, 0.00001), 1.0, nullptr, nullptr);
    state->createAndAddParameter("Mix", "Mix", "Mix", NormalisableRange<float>(0.01f, 1.f, 0.0001), 1.0, nullptr, nullptr);
    state->createAndAddParameter("Volume", "Volume", "Volume", NormalisableRange<float>(0.01f, 3.f, 0.0001), 1.0, nullptr, nullptr);

    state->state = ValueTree("Drive");
    state->state = ValueTree("Distortion");
    state->state = ValueTree("Mix");  
    state->state = ValueTree("Volume");

}

VisualAudioProcessor::~VisualAudioProcessor()
{
}

//==============================================================================
const juce::String VisualAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VisualAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VisualAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VisualAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VisualAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VisualAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VisualAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VisualAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VisualAudioProcessor::getProgramName (int index)
{
    return {};
}

void VisualAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VisualAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void VisualAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VisualAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void VisualAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

   
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float drive = *state->getRawParameterValue("Drive");
    float range = *state->getRawParameterValue("Distortion");
    float blend = *state->getRawParameterValue("Mix");
    float volume = *state->getRawParameterValue("Volume");


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {

            float cleanSignal = *channelData;

            *channelData *= drive * range;

            *channelData = (((((2 / float_Pi) * atan(*channelData) * blend) + (cleanSignal * (1.f - blend))) / 2.f) * volume);

            channelData++;
        }
     
    }
}

AudioProcessorValueTreeState& VisualAudioProcessor::getState() {

    return *state;
}

//==============================================================================
bool VisualAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VisualAudioProcessor::createEditor()
{
    return new VisualAudioProcessorEditor (*this);
}

//==============================================================================
void VisualAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    MemoryOutputStream stream(destData, false);

    state->state.writeToStream(stream);
}

void VisualAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    ValueTree tree = ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid()) {
        state->state = tree;

    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VisualAudioProcessor();
}
