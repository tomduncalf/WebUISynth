/*
  ==============================================================================

    CustomOscillator.h
    Created: 11 May 2021 9:19:47am
    Author:  Tom Duncalf

  ==============================================================================
*/

#pragma once

template<typename Type>
class CustomOscillator
{
    enum OscTypeEnum
    {
        saw,
        sin
    };

public:
    //==============================================================================
    CustomOscillator()
    {
        setType (OscTypeEnum::saw);
    }

    void setType (int type)
    {
        auto& osc = processorChain.template get<oscIndex>();

        if (type == OscTypeEnum::saw)
        {
            osc.initialise (
                [] (Type x) {
                    return juce::jmap (x,
                                       Type (-juce::MathConstants<double>::pi),
                                       Type (juce::MathConstants<double>::pi),
                                       Type (-1),
                                       Type (1));
                },
                2);
        }
        else if (type == OscTypeEnum::sin)
        {
            osc.initialise (
                [] (Type x) {
                    return std::sin (x);
                },
                128);
        }
    }

    //==============================================================================
    void setFrequency (Type newValue, bool force = false)
    {
        auto& osc = processorChain.template get<oscIndex>();
        osc.setFrequency (newValue, force);
    }

    //==============================================================================
    void setLevel (Type newValue)
    {
        auto& gain = processorChain.template get<gainIndex>();
        gain.setGainLinear (newValue);
    }

    //==============================================================================
    void reset() noexcept
    {
        processorChain.reset();
    }

    //==============================================================================
    template<typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        processorChain.process (context);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        processorChain.prepare (spec);
    }

private:
    //==============================================================================
    enum
    {
        oscIndex,
        gainIndex// [2]
    };

    juce::dsp::ProcessorChain<juce::dsp::Oscillator<Type>, juce::dsp::Gain<Type>>
        processorChain;
};
