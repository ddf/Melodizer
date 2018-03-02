//
//  Scales.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "Scales.h"

#define DEFINE_SCALE( scaleName, scaleDegrees ) Scale scaleName( #scaleName, scaleDegrees, sizeof(scaleDegrees) / sizeof(int*) );

int p1[] = { 0, 1, 3, -1 };
int p2[] = { 2, 2, 4, -1 };
int p3[] = { 4, 0, 3, -1 };
int p4[] = { 7, 0, 1, -1 };
int p5[] = { 9, 1, 2, -1 };
int* plist[] = { p1, p2, p3, p4, p5 };

DEFINE_SCALE( Pentatonic, plist );

int mj1[] = { 0, 1, 2, 3, 4, 5, 6, -1 }; // C
int mj2[] = { 2, 3, 4, 6, -1 };          // D
int mj3[] = { 4, 0, 3, 4, -1 };          // E
int mj4[] = { 5, 1, 2, 5, -1 };          // F
int mj5[] = { 7, 2, 3, 5, 6, -1 };       // G
int mj6[] = { 9, 3, 4, 6, -1 };          // A
int mj7[] = { 11, 1, 4, 5, -1 };          // B
int* mjlist[] = { mj1, mj2, mj3, mj4, mj5, mj6, mj7 };

DEFINE_SCALE( Major, mjlist );

int pm1[] = { 0, 1, 3, -1 };
int pm2[] = { 3, 2, 4, -1 };
int pm3[] = { 5, 0, 3, -1 };
int pm4[] = { 7, 0, 1, -1 };
int pm5[] = { 10, 1, 2, -1 };
int* pmlist[] = { pm1, pm2, pm3, pm4, pm5 };

DEFINE_SCALE( PentatonicMinor, pmlist );

int nm1[] = { 0, 1, 2, 3, 4, 5, 6, -1 }; // C
int nm2[] = { 2, 3, 4, 6, -1 };          // D
int nm3[] = { 3, 0, 3, 4, 6, -1 };       // Eb
int nm4[] = { 5, 1, 2, 5, 6, -1 };       // F
int nm5[] = { 7, 3, 5, 6, -1 };           // G
int nm6[] = { 8, 3, 4, 5, -1 };           // Ab
int nm7[] = { 10, 0, 1, 4, 5, -1 };      // Bb
int* nmlist[] = { nm1, nm2, nm3, nm4, nm5, nm6, nm7 };

DEFINE_SCALE( NaturalMinor, nmlist );

int hm1[] = { 0, 2, 4, 5, -1 };         // C
int hm2[] = { 2, 3, 4, 6, -1 };         // D
int hm3[] = { 3, 0, 4, 5, -1 };         // Eb
int hm4[] = { 5, 1, 5, -1 };            // F
int hm5[] = { 7, 0, 1, 2, 5, 6, -1 };   // G
int hm6[] = { 8, 0, 1, 3, -1 };         // Ab
int hm7[] = { 11, 1, 4, 5, -1 };        // B
int* hmlist[] = { hm1, hm2, hm3, hm4, hm5, hm6, hm7 };

DEFINE_SCALE( HarmonicMinor, hmlist );

int b1[] = { 0, 1, 5, -1 };         // C
int b2[] = { 3, 0, 2, -1 };         // Eb
int b3[] = { 5, 1, 3, -1 };         // F
int b4[] = { 6, 4, -1 };            // F#
int b5[] = { 7, 1, 5, -1 };         // G
int b6[] = { 10, 0, 3, 4, -1 };     // Bb
int* blist[] = { b1, b2, b3, b4, b5, b6 };

DEFINE_SCALE( Blues, blist );

const Scale* const Scales[]  = { &Pentatonic, &Major, &PentatonicMinor, &NaturalMinor, &HarmonicMinor, &Blues };
const int ScalesLength = sizeof( Scales ) / sizeof( Scale* );
