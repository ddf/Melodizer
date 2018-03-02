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