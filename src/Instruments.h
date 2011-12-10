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

class Looper : public Minim::Instrument
{
public:
    
    void noteOn( float dur );
    void noteOff();
};

class Kick : public Minim::Instrument
{
public:
    
    Kick( float amp );
    
    void noteOn( float dur );
    void noteOff();
    
private:
    
    Minim::Line     freqSweep;
    Minim::Line     ampSweep;
    Minim::Oscil    osc;
};

class Snare : public Minim::Instrument
{
public:
    
    Snare( float amp );
    
    void noteOn( float dur );
    void noteOff();
    
private:
    
    Minim::Noise        noize;
    Minim::Line         ampSweep;
    Minim::MoogFilter   filter;
};

class Hat : public Minim::Instrument
{
public:
    Hat( float amp );
    
    void noteOn( float dur );
    void noteOff();
    
private:
    
    Minim::Noise        noize;
    Minim::Line         ampSweep;
    Minim::MoogFilter   filter;
};

class Tone : public Minim::Instrument
{
public:
    Tone( Minim::Summer& bus, Minim::Waveform* waveform, float freq, float amp, float pan );
    
    void noteOn( float dur );
    void noteOff();
    
private:
    
    Minim::Summer&  out;
    Minim::Oscil    wave;
    Minim::ADSR     adsr;
    Minim::Pan      panner;
};

#endif
