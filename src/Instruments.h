//
//  Instruments.h
//  melodizer
//
//  Created by Damien Di Fede on 12/7/11.
//  Copyright (c) 2011 Damien Di Fede. All rights reserved.
//

#ifndef melodizer_Instruments_h
#define melodizer_Instruments_h

#include "Instrument.h"
#include "Line.h"
#include "Oscil.h"
#include "Multiplier.h"
#include "Noise.h"
#include "MoogFilter.h"
#include "Summer.h"
#include "Pan.h"
#include "ADSR.h"

#include <list>

void SetupInstruments();
void ClearInstruments();

class Looper : public Minim::Instrument
{
public:
    Looper();
    
    void noteOn( float dur );
    void noteOff();
    
private:
    int  tick;
};

class Tone : public Minim::Instrument
{
public:
    Tone( Minim::Waveform* waveform );
    
    void init( Minim::Summer* out, float freq, float amp, float pan );
    void noteOn( float dur );
    void noteOff();
    
private:
    
    struct ToneParams
    {
        ToneParams( Minim::Summer* o, float f, float a, float p ) : out(o), freq(f), amp(a), pan(p) {}
        
        Minim::Summer* out;
        float freq, amp, pan;
    };
    
    std::list< ToneParams > params;
    
    Minim::Summer*  out;
    Minim::Oscil    wave;
    Minim::ADSR     adsr;
    Minim::Pan      panner;
};

#endif
