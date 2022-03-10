# Clockmaker
by Daniel Schwartz

A simple VST3 plugin to generate an audio clock signal that can be used to sync an external sequencer to a DAW timeline.  Since MIDI clock is often unusable as a reliable clock source, Clockmaker provides an easy way to generate a stable audio clock signal from your DAW of choice.  Simply place clockmaker onto a track and send that to an appropriate audio output.  Note, acidentally sending clockmaker to your monitors is not fun.

## Parameters
- PPQN: sets the pulses per quarter note of the clock signal
- Mul/Div: a clock multiplier/divider