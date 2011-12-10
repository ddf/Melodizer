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

#include "Scales.h"

enum WaveformType
{
    WT_Sine = 0,
    WT_Triangle,
    WT_Saw,
    WT_Square,
    WT_Quarterpulse,
    WT_Sine4,
    WT_Sine8,
    WT_Sine16,
    WT_Sine32,
    WT_Count,
};

static Minim::Waveform* generate( WaveformType type )
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

// our instrument pools.
static std::list<Kick*>        kicks;
static std::list<Snare*>       snares;
static std::list<Hat*>         hats;
static std::list<Tone*>        tones[WT_Count];

void SetupInstruments()
{
    kicks.push_back( new Kick );
    kicks.push_back( new Kick );
    kicks.push_back( new Kick );
    
    snares.push_back( new Snare );
    snares.push_back( new Snare );
    snares.push_back( new Snare );
    
    hats.push_back( new Hat );
    hats.push_back( new Hat );
    hats.push_back( new Hat );
    
    // make 4 of each kind of tone
    for( int i = 0; i < WT_Count; ++i )
    {
        for( int c = 0; c < 32; ++c )
        {
            Minim::Waveform* wave = generate( (WaveformType)i );
            tones[i].push_back( new Tone( wave ) );
        }
    }
}

void ClearInstruments()
{
    for( std::list<Kick*>::iterator iter = kicks.begin(), end = kicks.end(); iter != end; ++iter )
    {
        delete *iter;
    }
    kicks.clear();
    
    for( std::list<Snare*>::iterator iter = snares.begin(), end = snares.end(); iter != end; ++iter )
    {
        delete *iter;
    }
    snares.clear();
    
    for( std::list<Hat*>::iterator iter = hats.begin(), end = hats.end(); iter != end; ++iter )
    {
        delete *iter;
    }
    hats.clear();
    
    for( int i = 0; i < WT_Count; ++i )
    {
        std::list<Tone*> & list = tones[ i ];
        for( std::list<Tone*>::iterator iter = list.begin(), end = list.end(); iter != end; ++iter )
        {
            delete *iter;
        }
        list.clear();
    }
}

static void kick( float time, float amp )
{
    Kick* k = kicks.front();
    k->init( amp );
    
    Out().playNote( time, 0.3f, *k );
    
    kicks.pop_front();
    kicks.push_back( k );
}

static void snare( float time, float amp )
{
    Snare* s = snares.front();
    s->init( amp );
    
    Out().playNote( time, 0.3f, *s );
    
    snares.pop_front();
    snares.push_back(s);
}

static void hat( float time, float amp )
{
    Hat* h = hats.front();
    h->init( amp );
    
    Out().playNote( time, 0.5f, *h );
    
    hats.pop_front();
    hats.push_back(h);
}

static void tone( Minim::Summer* bus, float time, WaveformType type, float freq, float amp, float dur, float pan )
{
    std::list<Tone*> & list = tones[ type ];
    Tone* t = list.front();
    t->init( bus, freq, amp, pan );
    
    Out().playNote( time, dur, *t );
    
    list.pop_front();
    list.push_back( t );
}


static int prevNoteIndex = 0;

static void generateMeasure( Minim::Summer& bus, WaveformType type, float* probs, const Scale* notes, int lowOctave, int hiOctave, float panRange )
{
    int scaleKey = 0; // TODO Get from settings
    
    for ( int i = 0; i < 16; ++i )
    {
        if ( ofRandom(1) < probs[i] )
        {
            int nextNoteIndex = notes->nextNoteIndex( prevNoteIndex );
            int baseNote      = notes->scale[nextNoteIndex][0] + scaleKey;
            int octave        = (int)ofRandom(lowOctave, hiOctave);
            int note          = baseNote + octave * 12;
            float freq        = Minim::Frequency::ofMidiNote( note ).asHz();
            float amp         = ofRandom(0.6f, 0.8f);
            float dur         = ofRandom(0.15f, 0.25f);
            float pan         = 0.f;
            
            if ( panRange != 0 )
            {
                pan = ofRandom(panRange) + 0.2f;
                if ( ofRandom(1) < 0.5f )
                {
                    pan *= -1;
                }
            }
            
            float time = i * 0.25f;
            
            // shuffle odd notes
            if ( i % 2 == 0 )
            {
                // TODO get shuffle from settings
                float Shuffle = 0.f; 
                time += 0.08f * Shuffle;
            }
            
            tone( &bus, time, type, freq, amp, dur, ofRandom(-pan, pan) );
            
            prevNoteIndex = nextNoteIndex;
        }
    }
    
}

//-----------------------------------------
//-- LOOPER
//-----------------------------------------
void Looper::noteOn(float dur)
{
    float noteProb[] = { 1.0f, 0.3f, 0.1f, 0.f,  0.8f, 0.34f, 0.6f, 0.11f,  0.7f, 0.24f, 0.6f, 0.7f,  0.12f, 0.3f, 0.23f, 0.9f };
    float bassProb[] = { 1.0f, 0.3f, 0.1f, 0.f,  0.8f, 0.34f, 0.6f, 0.11f,  0.7f, 0.24f, 0.6f, 0.7f,  0.12f, 0.3f, 0.23f, 0.9f };
    // int scaleIndex = // get from settings
    generateMeasure( Melody(), WT_Saw, noteProb, Scales[2], 5, 7, 0.8f );
    generateMeasure( Bass(),   WT_Square, bassProb, Scales[2], 2, 5, 0.0f );
    
    // TODO get Shuffle from settings
    float Shuffle = 0.08f * 0.f;
    
    kick( 0.f, 0.75f );
    if ( ofRandom(1) < 0.3f )
    {
        kick( 0.5f, 0.5f );
    }
    kick( 1.f, 0.75f );
    kick( 2.f, 0.75f );
    kick( 3.f, 0.75f );
    if ( ofRandom(1) < 0.2f )
    {
        kick( 3.75f, 0.3f );
    }
    
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
Kick::Kick()
: osc( 80.f, 0.f, Minim::Waves::SINE() )
, freqSweep()
, ampSweep()
{
    freqSweep.patch( osc.frequency );
    ampSweep.patch( osc.amplitude );
}

void Kick::init( float amp )
{
    amplitudes.push_back(amp);
}

void Kick::noteOn(float dur)
{
    ampSweep.activate( 0.1f, amplitudes.front(), 0 );
    freqSweep.activate( 0.1f, 120.f, 20.f );
    osc.patch( Out() );
    
    amplitudes.pop_front();
}

void Kick::noteOff()
{
    osc.unpatch( Out() );
}

//--------------------------------------
//-- SNARE
//--------------------------------------
Snare::Snare()
: noize( 1, Minim::Noise::eTintPink )
, ampSweep()
, filter( 200.f, 0.5f, Minim::MoogFilter::HP )
{
    noize.patch( filter );
    ampSweep.patch( noize.amplitude );
}

void Snare::init( float amp )
{
    amplitudes.push_back(amp);
}

void Snare::noteOn(float dur)
{
    ampSweep.activate( 0.05f, amplitudes.front(), 0 );
    filter.patch( Out() );
    
    amplitudes.pop_front();
}

void Snare::noteOff()
{
    filter.unpatch( Out() );
}

//--------------------------------------
//-- HAT
//--------------------------------------
Hat::Hat()
: noize( 1, Minim::Noise::eTintWhite )
, ampSweep()
, filter( 10000.0f, 0.1f, Minim::MoogFilter::HP )
{
    noize.patch( filter );
    ampSweep.patch( noize.amplitude );
}

void Hat::init( float amp )
{
    amplitudes.push_back( amp );
}

void Hat::noteOn(float dur)
{
    ampSweep.activate( 0.05f, amplitudes.front() * 0.8f, 0 );
    filter.patch( Out() );
    
    amplitudes.pop_front();
}

void Hat::noteOff()
{
    filter.unpatch( Out() );
}

//---------------------------------------
//-- TONE
//---------------------------------------
Tone::Tone( Minim::Waveform* waveform )
: out( NULL )
, wave( 0, 1, waveform )
, panner( 0 )
, adsr()
{
    adsr.setAudioChannelCount( 2 );
    wave.patch( panner ).patch ( adsr );
}

void Tone::init( Minim::Summer* out, float freq, float amp, float pan )
{
    params.push_back( ToneParams( out, freq, amp, pan ) );
}

void Tone::noteOn( float dur )
{
    ToneParams& param = params.front();
    wave.frequency.setLastValue( param.freq );
    adsr.setParameters( param.amp, 0.01f, 0.01f, param.amp * 0.7f, 0.2f, param.amp, 0 );
    panner.pan.setLastValue( param.pan );
    out = param.out;
    
    adsr.noteOn();
    adsr.patch( *out );
    
    params.pop_front();
}

void Tone::noteOff()
{
    adsr.noteOff();
    adsr.unpatchAfterRelease( out );
    out = NULL;
}