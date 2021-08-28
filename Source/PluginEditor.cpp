/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VisualAudioProcessorEditor::VisualAudioProcessorEditor (VisualAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
 

        addAndMakeVisible(driveKnob = new Slider("Drive"));
        driveKnob->setSliderStyle(Slider::Rotary);
        driveKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

        addAndMakeVisible(rangeKnob = new Slider("Range"));
        rangeKnob->setSliderStyle(Slider::Rotary);
        rangeKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

        addAndMakeVisible(blendKnob = new Slider("Blend"));
        blendKnob->setSliderStyle(Slider::Rotary);
        blendKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

        addAndMakeVisible(volumeKnob = new Slider("Volume"));
        volumeKnob->setSliderStyle(Slider::Rotary);
        volumeKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

        driveAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "Drive", *driveKnob);
        rangeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "Range", *rangeKnob);
        blendAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "Blend", *blendKnob);
        volumeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "Volume", *volumeKnob);

        setSize(500, 200);
}

VisualAudioProcessorEditor::~VisualAudioProcessorEditor()
{
}

//==============================================================================
void VisualAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void VisualAudioProcessorEditor::resized()
{
    driveKnob->setBounds(((getWidth()/5) *1) - (100/2), (getHeight() /2) - (100/2), 100,100);
    rangeKnob->setBounds(((getWidth()/ 5) * 1) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);
    blendKnob->setBounds(((getWidth()/ 5) * 1) - (100 / 2), (getHeight() / 2)- (100 / 2), 100, 100);
    volumeKnob->setBounds(((getWidth()/ 5) * 1) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);
}
