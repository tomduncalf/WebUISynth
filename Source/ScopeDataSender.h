/*
  ==============================================================================

    ScopeComponent.h
    Created: 15 May 2021 3:10:15pm
    Author:  Tom Duncalf

  ==============================================================================
*/

#pragma once

using namespace juce;
using namespace tomduncalf::BrowserIntegration;

template<typename SampleType>
class ScopeDataSender : private juce::Timer,
                        public BrowserIntegrationClient
{
public:
    using Queue = AudioBufferQueue<SampleType>;

    //==============================================================================
    ScopeDataSender (Queue& queueToUse, BrowserIntegration& b)
        : BrowserIntegrationClient ("Scope", b),
          audioBufferQueue (queueToUse)
    {
        sampleData.fill (SampleType (0));
        setFramesPerSecond (30);
    }

    //==============================================================================
    void setFramesPerSecond (int framesPerSecond)
    {
        jassert (framesPerSecond > 0 && framesPerSecond < 1000);
        startTimerHz (framesPerSecond);
    }

private:
    //==============================================================================
    Queue& audioBufferQueue;
    std::array<SampleType, Queue::bufferSize> sampleData;

    juce::dsp::FFT fft { Queue::order };
    using WindowFun = juce::dsp::WindowingFunction<SampleType>;
    WindowFun windowFun { (size_t) fft.getSize(), WindowFun::hann };
    std::array<SampleType, 2 * Queue::bufferSize> spectrumData;

    //==============================================================================
    void timerCallback() override
    {
        audioBufferQueue.pop (sampleData.data());
        juce::FloatVectorOperations::copy (spectrumData.data(), sampleData.data(), (int) sampleData.size());

        auto fftSize = (size_t) fft.getSize();

        jassert (spectrumData.size() == 2 * fftSize);
        windowFun.multiplyWithWindowingTable (spectrumData.data(), fftSize);
        fft.performFrequencyOnlyForwardTransform (spectrumData.data());

        static constexpr auto mindB = SampleType (-160);
        static constexpr auto maxdB = SampleType (0);

        for (auto& s: spectrumData)
            s = juce::jmap (juce::jlimit (mindB, maxdB, juce::Decibels::gainToDecibels (s) - juce::Decibels::gainToDecibels (SampleType (fftSize))), mindB, maxdB, SampleType (0), SampleType (1));

        juce::var samples;
        for (auto sample: sampleData)
            samples.append (sample);

        sendEventToBrowser ("scopeUpdate", samples, true);
    }
};
