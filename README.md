[![License: Zlib](https://img.shields.io/badge/License-Zlib-lightgrey.svg)](https://opensource.org/licenses/Zlib)

# Melodizer

Melodizer works like an analog sequencer where each step in the sequence is controlled by a column of knobs. However, rather than having a Pitch and Gate knob for each step, Melodizer's main knob is Probability. At each step you can control the probability that it will play a note when that step is activated. The actual pitch played will depend on the chosen Key, Scale, Octave, and Range. At each step you can also control how the note is panned, set the velocity, and shape the amplitude envelope of the note using ADSR knobs (attack, decay, sustain, release).

Melodizer will play notes using a built-in oscillator and effects. Any MIDI notes received by Melodizer will be used to change the Key and Octave or, in the case of the Fingered Scale, will be used as the pool of notes to choose from.  The VST and Standalone versions also send MIDI note messages so that you can use them to drive any soft-synth you already own. MIDI Out will be coming to the VST3 and AU versions in a future release.

# How to Build

- clone https://github.com/ddf/wdl-ol
- create a folder named Projects in the root of the repo
- clone this repo into the Projects folder
- clone https://github.com/ddf/minim-cpp into the Projects folder
- to build the VST version, follow the instructions here to install the VST SDK: https://github.com/ddf/wdl-ol/tree/master/VST_SDK
- to build the VST3 version, follow the instructions to install the VST 3.6.6 SDK: https://github.com/ddf/wdl-ol/tree/master/VST3_SDK, it should not be necessary to modify any project files
- open Melodizer.sln in Visual Studio 2015 or Evaluator.xcodeproj in XCode 9 and build the flavor you are interested in
