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
int  CurrentTick();

class Looper : public Minim::Instrument
{
public:
    Looper();
    
    void noteOn( float dur );
    void noteOff();
    
    int currentTick() const { return tick; }
    
private:
    int  tick;
};

class Tone : public Minim::Instrument
{
public:
    Tone( Minim::Waveform* waveform );
    
    void init( Minim::Summer* out, int tick, float freq, float amp, float pan );
    void noteOn( float dur );
    void noteOff();
    int  getTick() const { return tick; }
    
private:
    
    struct ToneParams
    {
        ToneParams( Minim::Summer* o, int t, float f, float a, float p ) : out(o), tick(t), freq(f), amp(a), pan(p) {}
        
        Minim::Summer* out;
        int   tick;
        float freq, amp, pan;
    };
    
    std::list< ToneParams > params;
    
    Minim::Summer*  out;
    Minim::Oscil    wave;
    Minim::ADSR     adsr;
    Minim::Pan      panner;
    int             tick;
};

#endif
