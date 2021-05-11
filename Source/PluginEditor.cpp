/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
WebUISynthAudioProcessorEditor::WebUISynthAudioProcessorEditor (WebUISynthAudioProcessor& p,
                                                                juce::AudioProcessorValueTreeState& t)
    : AudioProcessorEditor (&p),
      BrowserIntegrationClient ("Main", browserIntegration),
      audioProcessor (p),
      parameterValueTree (t),
      browser ("http://127.0.0.1:3000"),
      browserIntegration (browser),
      valueTreeSynchroniser (parameterValueTree.state, "parameters", browser)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible (browser);

    registerBrowserCallback ("AppReady", [this] (juce::var) {
        valueTreeSynchroniser.sendFullSyncCallback();
    });

    registerBrowserCallback ("setParameter", [this] (juce::var data) {
        parameterValueTree.getParameterAsValue (data["id"].toString()).setValue (data["value"]);
    });
}

WebUISynthAudioProcessorEditor::~WebUISynthAudioProcessorEditor()
{
}

//==============================================================================
void WebUISynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void WebUISynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    browser.setBounds (getBounds());
}

void WebUISynthAudioProcessorEditor::timerCallback()
{
    auto number = r.nextInt (100);
    state.setProperty ("intProperty", number, nullptr);
}
