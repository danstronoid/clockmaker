/*
  ==============================================================================

    File: clock.cpp
    Author: Daniel Schwartz
    Description: Generates a pulse wave clock signal.

  ==============================================================================
*/

#include "clock.h"

using namespace dingus_dsp;

void Clock::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    Reset();
    UpdateDelta();
}

float Clock::Process()
{
    float sample = (phase_ < juce::MathConstants<float>::pi) ? 1 : -1;

    phase_ += delta_;

    if (phase_ > juce::MathConstants<float>::twoPi)
    {
        phase_ -= juce::MathConstants<float>::twoPi;
    }

    return sample;
}

void Clock::SetTimeAdvance(int time_samples)
{
    phase_ = delta_ * time_samples;
    phase_ = std::fmodf(phase_, juce::MathConstants<float>::twoPi);
}

void Clock::SetMulDiv(int mulDiv)
{
    if (mulDiv < -1.f)
        mulDiv_ = 1.f / (-1.f * mulDiv);
    else if (mulDiv > 1.f)
        mulDiv_ = mulDiv;
    else
        mulDiv_ = 1.f;

    UpdateDelta();
}

void Clock::UpdateDelta()
{
    float freq = tempo_ * ppqn_ * mulDiv_ / 60.f;
    delta_ = juce::MathConstants<float>::twoPi * freq / sample_rate_;
}