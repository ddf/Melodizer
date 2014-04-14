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
static std::list<Tone*>        tones[WT_Count];
    

void SetupInstruments()
{
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
}

void Looper::noteOff()
{
    Out().setTempo( Settings::Tempo );
    Out().playNote( 0.f, 0.25f, *this );
    tick = (tick+1)%16;
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