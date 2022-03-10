/*
  ==============================================================================

    File: clock.h
    Author: Daniel Schwartz
    Description: Generates a pulse wave clock signal.

  ==============================================================================
*/

#pragma once
#ifndef DINGUS_CLOCK_H
#define DINGUS_CLOCK_H

#include <JuceHeader.h>

namespace dingus_dsp
{
    // Generates a pulse wave clock signal.
    class Clock
    {
    public:
        Clock() {}
        ~Clock() {}

        // Initialize the clock for playback given the audio rate.
        void Init(float sample_rate);

        // Process a single sample.
        float Process();

        // Advance time by some number of samples.
        void SetTimeAdvance(int time_samples);

        // Reset the phase.
        void Reset()
        {
            phase_ = 0;
        }

        // Set the clock tempo (bpm)
        void SetTempo(float tempo)
        {
            tempo_ = tempo;
            UpdateDelta();
        }

        /// Set the pulses per quarter note
        void SetPpqn(int ppqn)
        {
            ppqn_ = ppqn;
            UpdateDelta();
        }

        // Set the clock multiplier/divider
        // Positive values will multiply and negative values will divide
        void SetMulDiv(int mulDiv);

    private:
        // The tempo in bpm
        float tempo_{};

        // Pulses per quarter note
        int ppqn_{};

        // A multiplier/divider
        float mulDiv_{ 1.f };

        // The current phase.
        float phase_{};

        // The delta to increment the phase.
        float delta_{};

        // The audio sample rate.
        float sample_rate_{};

        // Calculate the delta amount to increment the phase.
        void UpdateDelta();
    };
}


#endif