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
            addVoice (new Voice (parameterValueTree));

        setVoiceStealingEnabled (true);
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
    }

private:
    //==============================================================================
    void renderNextSubBlock (juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples) override
    {
        MPESynthesiser::renderNextSubBlock (outputAudio, startSample, numSamples);
    }

    juce::AudioProcessorValueTreeState& parameterValueTree;
};
