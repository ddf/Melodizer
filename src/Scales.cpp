//
//  Scales.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "Scales.h"

// this is defined simply to make our scale declarations easier to read
// and to avoid 1-off errors in specifying notes in the scales.
enum Notes
{
	C = 1, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B,
};

const Scale Monotonic = {
	"Monotonic",
	{
		{ C }
	}
};

const Scale Pentatonic = {
	"Pentatonic",
	{
		{ D, G }, // C
		{	   }, // Db
		{ E, A }, // D
		{	   }, // Eb
		{ C, G }, // E
		{	   }, // F
		{	   }, // Gb
		{ C, D }, // G
		{	   }, // Ab
		{ D, E }, // A
		{      }, // Bb
		{      }, // B
	}
};

const Scale Major = {
	"Major",
	{
		{ D, E, F, G, A, B }, // C
		{}, // Db
		{ E, F, G, B }, // D
		{}, // Eb
		{ C, F, G }, // E
		{ D, E, A }, // F
		{}, // Gb
		{ E, F, A, B }, // G
		{}, // Ab
		{ F, G, B }, // A
		{}, // Bb
		{ D, G, A }, // B
	}
};

const Scale PentatonicMinor = {
	"PentatonicMinor",
	{
		{ Eb, G }, // C
		{       }, // Db
		{       }, // D
		{ F, Bb }, // Eb
		{       }, // E
		{ C, G  }, // F
		{       }, // Gb
		{ C, Eb }, // G
		{       }, // Ab
		{       }, // A
		{ Eb, F }, // Bb
		{       }, // B
	}
};

const Scale NaturalMinor = {
	"NaturalMinor",
	{
		{ D, Eb, F, G, Ab, Bb }, // C
		{}, // Db
		{ F, G, Bb }, // D
		{ C, F, G, Bb }, // Eb
		{}, // E
		{ D, Eb, G, Bb }, // F
		{}, // Gb
		{ Eb, Ab, Bb }, // G
		{ F, G, Ab }, // Ab
		{}, // A
		{ C, D, G, Ab }, // Bb
		{}, // B
	}
};

const Scale HarmonicMinor = {
	"HarmonicMinor",
	{
		{ Eb, G, Ab }, // C
		{}, // Db
		{ F, G, B }, // D
		{ C, G, Ab }, // Eb
		{}, // E
		{ D, Ab }, // F
		{}, // Gb
		{ C, D, Ab, B }, // G
		{ C, D, F }, // Ab
		{}, // A
		{}, // Bb
		{ D, G, Ab }, // B
	}
};

const Scale Blues = {
	"Blues",
	{
		{ Eb, Gb, }, // C
		{}, // Db
		{}, // D
		{ C, F }, // Eb
		{}, // E
		{ Eb, Gb }, // F
		{ G }, // Gb
		{ Eb, Bb }, // G
		{}, // Ab
		{}, // A
		{ C, Gb, G }, // Bb
		{}, // B
	}
};

const Scale Chromatic = {
	"Chromatic",
	{
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // C
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // Db
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // D
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // Eb
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // E
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // F
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // Gb
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // G
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // Ab
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // A
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // Bb
		{ C, Db, D, Eb, E, F, Gb, G, Gb, A, Ab, B, }, // B
	}
};

const Scale Dorian = {
	"Dorian",
	{
		{ D, Eb, F, G, A, Bb }, // C
		{}, // Db
		{ F, G, Bb }, // D
		{ C, F, G, Bb }, // Eb
		{}, // E
		{ D, Eb, G, Bb }, // F
		{}, // Gb
		{ Eb, A, Bb }, // G
		{}, // Ab
		{ F, G, Bb }, // A
		{ C, D, G, A }, // Bb
		{}, // B
	}
};

const Scale Mixolydian = {
	"Mixolydian",
	{
		{ D, E, F, G, A, Bb }, // C
		{}, // Db
		{ E, F, G, Bb }, // D
		{}, // Eb
		{ C, F, G }, // E
		{ D, E, A }, // F
		{}, // Gb
		{ E, F, A, Bb }, // G
		{}, // Ab
		{ F, G, Bb }, // A
		{ D, G, A }, // Bb
		{}, // B
	}
};

const Scale* const Scales[] = { &Monotonic, &Pentatonic, &Major, &PentatonicMinor, &NaturalMinor, &HarmonicMinor, &Blues, &Chromatic, &Dorian, &Mixolydian };
const int ScalesLength = sizeof( Scales ) / sizeof( Scale* );