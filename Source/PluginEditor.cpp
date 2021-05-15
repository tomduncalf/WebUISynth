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
#if JUCE_DEBUG
      browser ("http://127.0.0.1:3000"),
#else
      // TODO Windows etc path
      browser ("file://" + juce::File::getSpecialLocation (juce::File::SpecialLocationType::currentApplicationFile).getChildFile ("Contents/Resources/build/index.html").getFullPathName()),
#endif
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

void WebUISynthAudioProcessorEditor::timerCallback()
{
    auto number = r.nextInt (100);
    state.setProperty ("intProperty", number, nullptr);
}
