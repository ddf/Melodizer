//
//  Settings.h
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_Settings_h
#define melodizer_Settings_h

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

class Settings
{
public:
    
    // global
    static float        Tempo;
    static int          Scale;
    static int          Key;
    static float        Shuffle;
    static int          PreviousNoteIndex; // used to determine what next melody or bass note is. need it here because it needs to be reset when scale is changed.
    
    // melody
    static float        MelodyProbablities[16];
    static float        MelodyVolume;
    static WaveformType MelodyWave;

    // bass
    static float        BassProbabilities[16];
    static float        BassVolume;
    static WaveformType BassWave;
    
    // drums
    static bool         PlayKick;
    static bool         PlaySnare;
    static bool         PlayHat;

};

#endif
