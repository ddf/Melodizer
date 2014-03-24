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
float        Settings::Shuffle( 1.0f );
int          Settings::PreviousMelodyNoteIndex( 0 );
int          Settings::PreviousBassNoteIndex( 0 );

// melody
float        Settings::MelodyProbablities[16] = { 1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f };
float        Settings::MelodyVolume( 0.8f );
WaveformType Settings::MelodyWave( WT_Sine8 );

float        Settings::BassProbabilities[16] = { 1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f };
float        Settings::BassVolume( 0.6f );
WaveformType Settings::BassWave( WT_Square );

bool         Settings::PlayKick( false );
bool         Settings::PlaySnare( false );
bool         Settings::PlayHat( false );