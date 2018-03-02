//
//  Instruments.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/7/11.
//  Copyright (c) 2011 Damien Di Fede. All rights reserved.
//

#include "Instruments.h"

#include "Wavetable.h"
#include "Scales.h"

//static void generateNote( Minim::Summer& bus,
//                          int   tick,
//                          float time,
//                          WaveformType type, 
//                          const Scale* notes, 
//                          int lowOctave, 
//                          int hiOctave, 
//                          float panRange, 
//                          int& previousNoteIndex
//                         )
//{
//    int nextNoteIndex = notes->nextNoteIndex( previousNoteIndex );
//    int baseNote      = notes->scale[nextNoteIndex][0] + Settings::Key;
//    int octave        = (int)ofRandom(lowOctave, hiOctave);
//    int note          = baseNote + octave * 12;
//    float freq        = Minim::Frequency::ofMidiNote( note ).asHz();
//    float amp         = ofRandom(0.41f, 0.61f);
//    float pan         = 0.f;
//    
//    if ( panRange != 0 )
//    {
//        pan = ofRandom(panRange) + 0.2f;
//        if ( ofRandom(1) < 0.5f )
//        {
//            pan *= -1;
//        }
//    }
//    
//    // we want note duration to be the same regardless of tempo, so we have to adjust for tempo
//    const float dur = Settings::Duration * (Settings::Tempo/60.f);
//    
//    tone( &bus, tick, time, type, freq, amp, dur, ofRandom(-pan, pan) );
//    
//    previousNoteIndex = nextNoteIndex;    
//}

//---------------------------------------
//-- TONE
//---------------------------------------
Tone::Tone( Summer& inSummer )
: out( inSummer )
, wave(new Wave())
, oscil( 0, 1, wave )
, panner( 0 )
, adsr()
{
    adsr.setAudioChannelCount( 2 );
    oscil.patch( panner ).patch ( adsr );
}

Tone::~Tone()
{
	// we don't delete wave because Oscil will do it.
}

float Tone::Wave::value(const float at) const
{
	return source != nullptr ? source->value(at) : 0.0f;
}

void Tone::noteOn( float dur, Waveform* waveform, int tick, float freq, float amp, float pan )
{
	wave->source = waveform;
    oscil.frequency.setLastValue( freq );
    adsr.setParameters( amp,          // max amp
                        0.01f,              // attack time
                        0.01f,              // decay time
                        0.7f,               // sustain level
                        dur*0.45f,          // release time
                        0,                  // before amp
                        0                   // after amp
                       );
    panner.pan.setLastValue( pan );
    tick = tick;
    
    adsr.noteOn();
    adsr.patch( out );
}

void Tone::noteOff()
{
    adsr.noteOff();
    adsr.unpatchAfterRelease( &out );
}