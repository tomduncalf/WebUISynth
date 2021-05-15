/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

using namespace tomduncalf::BrowserIntegration;

//==============================================================================
/**
*/
class WebUISynthAudioProcessorEditor : public juce::AudioProcessorEditor,
                                       public juce::Timer,
                                       public BrowserIntegrationPluginClient
{
public:
    WebUISynthAudioProcessorEditor (WebUISynthAudioProcessor&, juce::AudioProcessorValueTreeState& parameterValueTree);
    ~WebUISynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WebUISynthAudioProcessor& audioProcessor;

    juce::ValueTree state { "AppState" };
    juce::AudioProcessorValueTreeState& parameterValueTree;

    BrowserComponent browser;

    BrowserIntegration browserIntegration { browser };

    juce::Random r;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebUISynthAudioProcessorEditor)
};
