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
#include "Settings.h"

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

// our drum beat
struct DrumNote
{
    float prob;
    float amp;
};

DrumNote hatNotes[] = {
    {0.0f, 0.0f}, {0.0f, 0.00f}, {1.0f, 0.5f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.00f}, {1.0f, 0.5f}, {1.0f, 0.25f},
    {0.0f, 0.0f}, {0.0f, 0.00f}, {1.0f, 0.5f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {1.0f, 0.25f}, {0.0f, 0.0f}, {1.0f, 0.5f}
};

DrumNote snareNotes[] = {
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.5f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}
};

DrumNote kickNotes[] = { 
    {1.0f, 0.75f}, {0.0f, 0.0f}, {0.3f, 0.5f}, {0.0f, 0.0f},
    {1.0f, 0.75f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {1.0f, 0.75f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
    {1.0f, 0.75f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.2f, 0.3f}
};
    

void SetupInstruments()
{
    kicks.push_back( new Kick );
    snares.push_back( new Snare );
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

static void kick( float amp )
{
    kick( 0, amp );
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

static void generateNote( Minim::Summer& bus,
                          float time,
                          WaveformType type, 
                          const Scale* notes, 
                          int lowOctave, 
                          int hiOctave, 
                          float panRange, 
                          int& previousNoteIndex
                         )
{
    int nextNoteIndex = notes->nextNoteIndex( previousNoteIndex );
    int baseNote      = notes->scale[nextNoteIndex][0] + Settings::Key;
    int octave        = (int)ofRandom(lowOctave, hiOctave);
    int note          = baseNote + octave * 12;
    float freq        = Minim::Frequency::ofMidiNote( note ).asHz();
    float amp         = ofRandom(0.41f, 0.61f);
    float dur         = ofRandom(1) > 0.2f ? 0.04f : 0.25f;
    float pan         = 0.f;
    
    if ( panRange != 0 )
    {
        pan = ofRandom(panRange) + 0.2f;
        if ( ofRandom(1) < 0.5f )
        {
            pan *= -1;
        }
    }
    
    tone( &bus, time, type, freq, amp, dur, ofRandom(-pan, pan) );
    
    previousNoteIndex = nextNoteIndex;    
}

//-----------------------------------------
//-- LOOPER
//-----------------------------------------
Looper::Looper()
: tick(0)
{
    
}

void Looper::noteOn(float dur)
{
    float time = (tick%2) * Settings::Shuffle * 0.08f;
    
    // first melody and bass
    if ( ofRandom(1) < Settings::MelodyProbablities[tick] )
    {
        generateNote(Melody(), time, Settings::MelodyWave, Scales[ Settings::Scale ], 5, 7, 0.8f, Settings::PreviousMelodyNoteIndex );
    }
    
    if ( ofRandom(1) < Settings::BassProbabilities[tick] )
    {
        generateNote(Bass(), time, Settings::BassWave, Scales[ Settings::Scale ], 2, 5, 0, Settings::PreviousBassNoteIndex );
    }
    
    // hat
    if ( ofRandom(1) < hatNotes[tick].prob )
    {
        hat( time, hatNotes[tick].amp );
    }
    
    // snare
    if ( ofRandom(1) < snareNotes[tick].prob )
    {
        snare( time, snareNotes[tick].amp );
    }
    
    // kick
    if ( ofRandom(1) < kickNotes[tick].prob )
    {
        kick( time, kickNotes[tick].amp );
    }
}

void Looper::noteOff()
{
    Out().setTempo( Settings::Tempo );
    Out().playNote( 0.f, 0.25f, *this );
    tick = (tick+1)%16;
}

//--------------------------------------
//-- KICK
//--------------------------------------
Kick::Kick()
: osc( 80.f, 0.f, Minim::Waves::SINE() )
, freqSweep()
, ampSweep( 0, 0, 0 )
, bPatched(false)
{
    freqSweep.patch( osc.frequency );
    ampSweep.patch( osc.amplitude );
    osc.patch( Drums() );
}

void Kick::init( float amp )
{
    amplitudes.push_back(amp);
}

void Kick::noteOn(float dur)
{
    if ( Settings::PlayKick )
    {
        ampSweep.activate( 0.1f, amplitudes.front(), 0 );
        freqSweep.activate( 0.1f, 120.f, 40.f );
    }
    
    amplitudes.pop_front();
}

void Kick::noteOff()
{
}

//--------------------------------------
//-- SNARE
//--------------------------------------
Snare::Snare()
: noize( 1, Minim::Noise::eTintPink )
, ampSweep( 0, 0, 0 )
, filter( 200.f, 0.5f, Minim::MoogFilter::HP )
, bPatched(false)
{
    noize.patch( filter );
    ampSweep.patch( noize.amplitude );
    filter.patch( Drums() );
}

void Snare::init( float amp )
{
    amplitudes.push_back(amp);
}

void Snare::noteOn(float dur)
{
    if ( Settings::PlaySnare )
    {
        ampSweep.activate( 0.05f, amplitudes.front(), 0 );
    }
    
    amplitudes.pop_front();
}

void Snare::noteOff()
{
}

//--------------------------------------
//-- HAT
//--------------------------------------
Hat::Hat()
: noize( 1, Minim::Noise::eTintWhite )
, ampSweep( 0, 0, 0 )
, filter( 10000.0f, 0.1f, Minim::MoogFilter::HP )
, bPatched(false)
{
    noize.patch( filter );
    ampSweep.patch( noize.amplitude );
    filter.patch( Drums() );
}

void Hat::init( float amp )
{
    amplitudes.push_back( amp );
}

void Hat::noteOn(float dur)
{
    if ( Settings::PlayHat )
    {
        ampSweep.activate( 0.05f, amplitudes.front() * 0.8f, 0 );
    }
    
    amplitudes.pop_front();
}

void Hat::noteOff()
{
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
    adsr.setParameters( param.amp,          // max amp
                        0.01f,              // attack time
                        0.01f,              // decay time
                        0.7f,               // sustain level
                        dur*0.45f,          // release time
                        0,                  // before amp
                        0                   // after amp
                       );
    panner.pan.setLastValue( param.pan );
    out = param.out;
    
    adsr.noteOn();
    adsr.patch( *out );
    
    out->printInputs();
    
    params.pop_front();
}

void Tone::noteOff()
{
    adsr.noteOff();
    adsr.unpatchAfterRelease( out );
    out = NULL;
}