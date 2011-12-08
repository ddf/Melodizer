//
//  Instruments.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/7/11.
//  Copyright (c) 2011 Damien Di Fede. All rights reserved.
//

#include "Instruments.h"

#include "Audio.h"
#include "AudioOutput.h"
#include "Summer.h"

#include "Wavetable.h"
#include "Waves.h"

enum WaveformType
{
    WT_Sine,
    WT_Triangle,
    WT_Saw,
    WT_Square,
    WT_Quarterpulse,
    WT_Sine4,
    WT_Sine8,
    WT_Sine16,
    WT_Sine32,
};

struct WaveformGenerator
{
    WaveformType type;
    
    WaveformGenerator( WaveformType wt ) : type( wt ) {}
    
    Minim::Waveform* generate()
    {
        switch( type )
        {
            case WT_Sine:           return Minim::Waves::SINE();
            case WT_Triangle:       return Minim::Waves::TRIANGLE();
            case WT_Saw:            return Minim::Waves::SAW();
            case WT_Square:         return Minim::Waves::SQUARE();
            case WT_Quarterpulse:   return Minim::Waves::QUARTERPULSE();
            case WT_Sine4:          return Minim::Waves::randomNHarms(4);
            case WT_Sine8:          return Minim::Waves::randomNHarms(8);
            case WT_Sine16:         return Minim::Waves::randomNHarms(16);
            case WT_Sine32:         return Minim::Waves::randomNHarms(32);
                
            default: return Minim::Waves::SINE();
        }
    }
};

WaveformGenerator   generators[] = { WaveformGenerator(WT_Sine),
                                     WaveformGenerator(WT_Triangle),
                                     WaveformGenerator(WT_Saw),
                                     WaveformGenerator(WT_Square),
                                     WaveformGenerator(WT_Quarterpulse),
                                     WaveformGenerator(WT_Sine4),
                                     WaveformGenerator(WT_Sine8),
                                     WaveformGenerator(WT_Sine16),
                                     WaveformGenerator(WT_Sine32)
};

static void kick( float time, float amp )
{
    Kick* k = new Kick(amp);
    Out().playNote( time, 0.3f, *k );
}

static void snare( float time, float amp )
{
    Out().playNote( time, 0.3f, *(new Snare(amp)) );
}

static void hat( float time, float amp )
{
    Out().playNote( time, 0.5f, *(new Hat(amp)) );
}

static void tone( Minim::Summer& bus, float time, Minim::Waveform* wave, float freq, float amp, float dur, float pan )
{
    Out().playNote( time, dur, *(new Tone(bus, wave, freq, amp, pan)) );
}


static int prevNoteIndex = 0;

//static void generateMeasure( Minim::Summer& bus, Minim::Waveform* wave, float* probs, Scale& notes, int lowOctave, int hiOctave, float panRange )
//{
//    
//}

//-----------------------------------------
//-- LOOPER
//-----------------------------------------
void Looper::noteOn(float dur)
{
    // int scaleIndex = // get from settings
    // generateMeasure( Melody(), generators[ melodyWave ].generate(), noteProb, Scales[scaleIndex], 5, 7, 0.8f );
    // generateMeasure( Bass(),   generators[ bassWave   ].generate(), bassProb, Scales[scaleIndex], 2, 5, 0.0f );
    
    // TODO get Shuffle from settings
    float Shuffle = 0.08f * 0.5f;
    
    kick( 0.f, 0.5f );
    kick( 1.f, 0.5f );
    kick( 2.f, 0.5f );
    kick( 3.f, 0.5f );
    
    snare( 1.f, 1.f );
    snare( 2.75f + Shuffle, 1.5f );
    snare( 3.5f, 1.0f );
    
    hat( 0.5f, 0.5f );
    hat( 1.5f, 0.5f );
    hat( 1.75f + Shuffle, 0.25f );
    hat( 2.5f, 0.5f );
    hat( 3.25f + Shuffle, 0.25f );
    hat( 3.75f + Shuffle, 0.5f );
}

void Looper::noteOff()
{
    // TODO get tempo from settings
    Out().setTempo( 120 );
    Out().playNote( 0.f, 4.0f, *this );
}

//--------------------------------------
//-- KICK
//--------------------------------------
Kick::Kick( float amp )
: osc( 80.f, 0.f, Minim::Waves::SINE() )
, freqSweep( 0.1f, 120.f, 20.f )
, ampSweep( 0.1f, amp, 0 )
{
    freqSweep.patch( osc.frequency );
    ampSweep.patch( osc.amplitude );
}

void Kick::noteOn(float dur)
{
    ampSweep.activate();
    freqSweep.activate();
    osc.patch( Out() );
}

void Kick::noteOff()
{
    osc.unpatch( Out() );
    delete this;
}

//--------------------------------------
//-- SNARE
//--------------------------------------
Snare::Snare( float amp )
: noize( amp, Minim::Noise::eTintPink )
, ampSweep( 0.05f, amp, 0 )
, filter( 200.f, 0.5f, Minim::MoogFilter::HP )
{
    noize.patch( filter );
    ampSweep.patch( noize.amplitude );
}

void Snare::noteOn(float dur)
{
    ampSweep.activate();
    filter.patch( Out() );
}

void Snare::noteOff()
{
    filter.unpatch( Out() );
    delete this;
}

//--------------------------------------
//-- HAT
//--------------------------------------
Hat::Hat( float amp )
: noize( amp, Minim::Noise::eTintWhite )
, ampSweep( 0.05f, amp * 0.8f, 0 )
, filter( 10000.0f, 0.1f, Minim::MoogFilter::HP )
{
    noize.patch( filter );
    ampSweep.patch( noize.amplitude );
}

void Hat::noteOn(float dur)
{
    ampSweep.activate();
    filter.patch( Out() );
}

void Hat::noteOff()
{
    filter.unpatch( Out() );
    delete this;
}

//---------------------------------------
//-- TONE
//---------------------------------------
Tone::Tone( Minim::Summer& bus, Minim::Waveform* waveform, float freq, float amp, float pan )
: out( bus )
, wave( freq, amp, waveform )
, panner( pan )
// , adsr( amp, 0.01f, 0.01f, amp * 0.7f, 0.2f )
{
    // adsr.setAudioChannelCount( 2 );
    // wave.patch( panner ).patch ( adsr );
}

void Tone::noteOn( float dur )
{
    // adsr.patch( out );
    // adsr.noteOn();
}

void Tone::noteOff()
{
    // TODO how to handle this? just sweep amplitude instead??
    // adsr.noteOff();
    // adsr.unpatchAfterRelease( out );
}