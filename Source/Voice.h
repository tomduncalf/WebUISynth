/*
  ==============================================================================

    Voice.h
    Created: 11 May 2021 9:18:22am
    Author:  Tom Duncalf

  ==============================================================================
*/

#pragma once

#include "CustomOscillator.h"

class Voice : public juce::MPESynthesiserVoice
{
public:
    Voice()
    {
        auto& masterGain = processorChain.get<masterGainIndex>();
        masterGain.setGainLinear (0.5f);

        auto& filter = processorChain.get<filterIndex>();
        filter.setCutoffFrequencyHz (100.0f);
        filter.setResonance (0.3f);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
        processorChain.prepare (spec);

        adsrEnvelope.setSampleRate (spec.sampleRate);
        juce::ADSR::Parameters adsrParameters;
        adsrParameters.attack = 1.0f;
        adsrParameters.release = 2.0f;
        adsrEnvelope.setParameters (adsrParameters);
    }

    void setCutoff (float newValue)
    {
        auto& filter = processorChain.get<filterIndex>();
        filter.setCutoffFrequencyHz (newValue);
    }

    void setResonance (float newValue)
    {
        auto& filter = processorChain.get<filterIndex>();
        filter.setResonance (newValue);
    }

    //==============================================================================
    void noteStarted() override
    {
        auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
        auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();

        processorChain.get<osc1Index>().setFrequency (freqHz, true);
        processorChain.get<osc1Index>().setLevel (velocity);

        adsrEnvelope.noteOn();
    }

    //==============================================================================
    void notePitchbendChanged() override
    {
        auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();
        processorChain.get<osc1Index>().setFrequency (freqHz);
    }

    //==============================================================================
    void noteStopped (bool) override
    {
        //        clearCurrentNote();

        adsrEnvelope.noteOff();
    }

    //==============================================================================
    void notePressureChanged() override {}
    void noteTimbreChanged() override {}
    void noteKeyStateChanged() override {}

    //==============================================================================
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        auto block = tempBlock.getSubBlock (0, (size_t) numSamples);
        block.clear();
        juce::dsp::ProcessContextReplacing<float> context (block);
        processorChain.process (context);

        for (auto sample = 0; sample < block.getNumSamples(); sample++)
        {
            auto envelopeValue = adsrEnvelope.getNextSample();

            if (! adsrEnvelope.isActive())
                clearCurrentNote();

            for (auto channel = 0; channel < block.getNumChannels(); channel++)
                block.getChannelPointer (channel)[sample] *= envelopeValue;
        }

        juce::dsp::AudioBlock<float> (outputBuffer)
            .getSubBlock ((size_t) startSample, (size_t) numSamples)
            .add (tempBlock);
    }

private:
    //==============================================================================
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;

    juce::ADSR adsrEnvelope;

    enum
    {
        osc1Index,
        filterIndex,
        masterGainIndex
    };

    juce::dsp::ProcessorChain<
        CustomOscillator<float>,
        juce::dsp::LadderFilter<float>,
        juce::dsp::Gain<float>>
        processorChain;

    static constexpr size_t lfoUpdateRate = 100;
};
