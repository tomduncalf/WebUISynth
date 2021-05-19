#pragma once

#include "CustomOscillator.h"

class Voice : public juce::MPESynthesiserVoice,
              public juce::AudioProcessorValueTreeState::Listener
{
public:
    Voice (juce::AudioProcessorValueTreeState& p): parameterValueTree (p)
    {
        parameterValueTree.addParameterListener (ParameterIds::osc1Type, this);
        parameterValueTree.addParameterListener (ParameterIds::filterCutoff, this);
        parameterValueTree.addParameterListener (ParameterIds::filterResonance, this);
        parameterValueTree.addParameterListener (ParameterIds::ampEnvAttack, this);
        parameterValueTree.addParameterListener (ParameterIds::ampEnvDecay, this);
        parameterValueTree.addParameterListener (ParameterIds::ampEnvSustain, this);
        parameterValueTree.addParameterListener (ParameterIds::ampEnvRelease, this);

        auto& masterGain = processorChain.get<masterGainIndex>();
        masterGain.setGainLinear (0.7f);

        auto& filter = processorChain.get<filterIndex>();
        filter.setCutoffFrequencyHz (p.getParameterAsValue (ParameterIds::filterCutoff).getValue());
        filter.setResonance (p.getParameterAsValue (ParameterIds::filterResonance).getValue());

        setAdsrParameters (
            p.getParameterAsValue (ParameterIds::ampEnvAttack).getValue(),
            p.getParameterAsValue (ParameterIds::ampEnvDecay).getValue(),
            p.getParameterAsValue (ParameterIds::ampEnvSustain).getValue(),
            p.getParameterAsValue (ParameterIds::ampEnvRelease).getValue());
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
        processorChain.prepare (spec);

        adsrEnvelope.setSampleRate (spec.sampleRate);
    }

    void setAdsrParameters (float a, float d, float s, float r)
    {
        auto currentParameters = adsrEnvelope.getParameters();
        juce::ADSR::Parameters newParameters;

        newParameters.attack = a == -1 ? currentParameters.attack : a;
        newParameters.decay = d == -1 ? currentParameters.decay : d;
        newParameters.sustain = s == -1 ? currentParameters.sustain : s;
        newParameters.release = r == -1 ? currentParameters.release : r;

        adsrEnvelope.setParameters (newParameters);
    }

    void parameterChanged (const juce::String& parameterId, float newValue) override
    {
        if (parameterId == ParameterIds::filterCutoff)
            processorChain.get<filterIndex>().setCutoffFrequencyHz (newValue);
        else if (parameterId == ParameterIds::filterResonance)
            processorChain.get<filterIndex>().setResonance (newValue);
        else if (parameterId == ParameterIds::ampEnvAttack)
            setAdsrParameters (newValue, -1, -1, -1);
        else if (parameterId == ParameterIds::ampEnvDecay)
            setAdsrParameters (-1, newValue, -1, -1);
        else if (parameterId == ParameterIds::ampEnvSustain)
            setAdsrParameters (-1, -1, newValue, -1);
        else if (parameterId == ParameterIds::ampEnvRelease)
            setAdsrParameters (-1, -1, -1, newValue);
        else if (parameterId == ParameterIds::osc1Type)
            processorChain.get<osc1Index>().setType ((int) newValue);
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

    juce::AudioProcessorValueTreeState& parameterValueTree;

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
