//
//  Settings.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "Settings.h"


// global
float        Settings::Tempo( 120 );
int          Settings::Scale( 0 );
int          Settings::Key( 0 );
float        Settings::Shuffle( 0 );
int          Settings::PreviousNoteIndex( 0 );

// melody
float        Settings::MelodyProbablities[16] = { 1.0f, 0.3f, 0.1f, 0.f,  0.8f, 0.34f, 0.6f, 0.11f,  0.7f, 0.24f, 0.6f, 0.7f,  0.12f, 0.3f, 0.23f, 0.9f };
float        Settings::MelodyVolume( 0.8f );
WaveformType Settings::MelodyWave( WT_Sine8 );

float        Settings::BassProbabilities[16] = { 1.0f, 0.3f, 0.1f, 0.f,  0.8f, 0.34f, 0.6f, 0.11f,  0.7f, 0.24f, 0.6f, 0.7f,  0.12f, 0.3f, 0.23f, 0.9f };
float        Settings::BassVolume( 0.6f );
WaveformType Settings::BassWave( WT_Square );

bool         Settings::PlayKick( true );
bool         Settings::PlaySnare( true );
bool         Settings::PlayHat( true );