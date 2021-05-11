/*
  ==============================================================================

    AudioEngine.h
    Created: 11 May 2021 9:16:15am
    Author:  Tom Duncalf

  ==============================================================================
*/

#pragma once

#include "Voice.h"

class AudioEngine : public juce::MPESynthesiser,
                    public juce::AudioProcessorValueTreeState::Listener
{
public:
    static constexpr auto maxNumVoices = 4;

    //==============================================================================
    AudioEngine (juce::AudioProcessorValueTreeState& p): parameterValueTree (p)
    {
        for (auto i = 0; i < maxNumVoices; ++i)
            addVoice (new Voice);

        setVoiceStealingEnabled (true);

        parameterValueTree.addParameterListener (ParameterIdentifiers::filterCutoff, this);
        parameterValueTree.addParameterListener (ParameterIdentifiers::filterResonance, this);
        parameterValueTree.addParameterListener (ParameterIdentifiers::ampEnvAttack, this);
        parameterValueTree.addParameterListener (ParameterIdentifiers::ampEnvDecay, this);
        parameterValueTree.addParameterListener (ParameterIdentifiers::ampEnvSustain, this);
        parameterValueTree.addParameterListener (ParameterIdentifiers::ampEnvRelease, this);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec) noexcept
    {
        setCurrentPlaybackSampleRate (spec.sampleRate);

        for (auto* v: voices)
            dynamic_cast<Voice*> (v)->prepare (spec);
    }

    void parameterChanged (const juce::String& parameterId, float newValue) override
    {
        if (parameterId == ParameterIdentifiers::filterCutoff)
        {
            for (auto* v: voices)
                dynamic_cast<Voice*> (v)->setCutoff (newValue);
        }
        else if (parameterId == ParameterIdentifiers::filterResonance)
        {
            for (auto* v: voices)
                dynamic_cast<Voice*> (v)->setResonance (newValue);
        }
    }

private:
    //==============================================================================
    void renderNextSubBlock (juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples) override
    {
        MPESynthesiser::renderNextSubBlock (outputAudio, startSample, numSamples);
    }

    juce::AudioProcessorValueTreeState& parameterValueTree;
};
