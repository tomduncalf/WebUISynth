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
      BrowserIntegrationPluginClient (browserIntegration, t, __FILE__),
      audioProcessor (p),
      parameterValueTree (t),
      scopeDataSender (p.getAudioBufferQueue(), browserIntegration)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    setResizable (true, false);

    addAndMakeVisible (browser);

    setupBrowserPluginIntegration();
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
