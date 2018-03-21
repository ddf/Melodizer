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
		// totally random any scale degree can jump to any other
		{   D, E, G, A }, // C
		{              }, // Db
		{ C,   E, G, A }, // D
		{              }, // Eb
		{ C, D,   G, A }, // E
		{              }, // F
		{              }, // Gb
		{ C, D, E,   A }, // G
		{              }, // Ab
		{ C, D, E, G,  }, // A
		{              }, // Bb
		{              }, // B

		// trying to make "good" melodies
		//{ D, G }, // C
		//{}, // Db
		//{ E, A }, // D
		//{}, // Eb
		//{ C, G }, // E
		//{}, // F
		//{}, // Gb
		//{ C, D }, // G
		//{}, // Ab
		//{ D, E }, // A
		//{}, // Bb
		//{}, // B
	}
};

const Scale Major = {
	"Major",
	{
		// totally random any scale degree can jump to any other
		{    D, E, F, G, A, B }, // C
		{                     }, // Db
		{ C,    E, F, G, A, B }, // D
		{                     }, // Eb
		{ C, D,    F, G, A, B }, // E
		{ C, D, E,    G, A, B }, // F
		{                     }, // Gb
		{ C, D, E, F,    A, B }, // G
		{                     }, // Ab
		{ C, D, E, F, G,    B }, // A
		{                     }, // Bb
		{ C, D, E, F, G, A,   }, // B

		// trying to maek "good" melodies
		//{ E, G }, // C
		//{}, // Db
		//{ C, F, A }, // D
		//{}, // Eb
		//{ F, G, }, // E
		//{ C, A }, // F
		//{}, // Gb
		//{ D, F, B }, // G
		//{}, // Ab
		//{ C, F, E }, // A
		//{}, // Bb
		//{ D, F, G }, // B
	}
};

const Scale PentatonicMinor = {
	"Pentatonic Minor",
	{
		// totally random any scale degree can jump to any other
		{    Eb, F, G, Bb }, // C
		{                 }, // Db
		{                 }, // D
		{ C,     F, G, Bb }, // Eb
		{                 }, // E
		{ C, Eb,    G, Bb }, // F
		{                 }, // Gb
		{ C, Eb, F,    Bb }, // G
		{                 }, // Ab
		{                 }, // A
		{ C, Eb, F, G     }, // Bb
		{                 }, // B

		// trying to make "good" melodies
		//{ Eb, G }, // C
		//{       }, // Db
		//{       }, // D
		//{ F, Bb }, // Eb
		//{       }, // E
		//{ C, G  }, // F
		//{       }, // Gb
		//{ C, Eb }, // G
		//{       }, // Ab
		//{       }, // A
		//{ Eb, F }, // Bb
		//{       }, // B
	}
};

const Scale NaturalMinor = {
	"Natural Minor",
	{
		// totally random any scale degree can jump to any other
		{    D, Eb, F, G, Ab, Bb }, // C
		{                        }, // Db
		{ C,    Eb, F, G, Ab, Bb }, // D
		{ C, D,     F, G, Ab, Bb }, // Eb
		{                        }, // E
		{ C, D, Eb,    G, Ab, Bb }, // F
		{                        }, // Gb
		{ C, D, Eb, F,    Ab, Bb }, // G
		{ C, D, Eb, F, G,     Bb }, // Ab
		{                        }, // A
		{ C, D, Eb, F, G, Ab,    }, // Bb
		{                        }, // B

		// trying to make "good" melodies
		//{ D, Eb, Ab }, // C
		//{}, // Db
		//{ C, Eb, Bb }, // D
		//{ F, Ab }, // Eb
		//{}, // E
		//{ D, G, }, // F
		//{}, // Gb
		//{ C, D, Ab }, // G
		//{ F, G }, // Ab
		//{}, // A
		//{ C, Eb, G }, // Bb
		//{}, // B
	}
};

const Scale HarmonicMinor = {
	"Harmonic Minor",
	{
		// totally random any scale degree can jump to any other
		{    D, Eb, F, G, Ab, B }, // C
		{                       }, // Db
		{ C,    Eb, F, G, Ab, B }, // D
		{ C, D,     F, G, Ab, B }, // Eb
		{                       }, // E
		{ C, D, Eb,    G, Ab, B }, // F
		{                       }, // Gb
		{ C, D, Eb, F,    Ab, B }, // G
		{ C, D, Eb, F, G,     B }, // Ab
		{                       }, // A
		{                       }, // Bb
		{ C, D, Eb, F, G, Ab    }, // B

		// trying to make "good" melodies
		//{ Eb, G, Ab }, // C
		//{}, // Db
		//{ F, G, B }, // D
		//{ C, G, Ab }, // Eb
		//{}, // E
		//{ D, Ab }, // F
		//{}, // Gb
		//{ C, D, Ab, B }, // G
		//{ C, D, F }, // Ab
		//{}, // A
		//{}, // Bb
		//{ D, G, Ab }, // B
	}
};

const Scale Blues = {
	"Blues",
	{
		// totally random any scale degree can jump to any other
		{    Eb, F, Gb, G, Bb }, // C
		{                     }, // Db
		{                     }, // D
		{ C,     F, Gb, G, Bb }, // Eb
		{                     }, // E
		{ C, Eb,    Gb, G, Bb }, // F
		{ C, Eb, F,     G, Bb }, // Gb
		{ C, Eb, F, Gb,    Bb }, // G
		{                     }, // Ab
		{                     }, // A
		{ C, Eb, F, Gb, G,    }, // Bb
		{                     }, // B

		// trying to make "good" melodies
		//{ Eb, Gb, }, // C
		//{}, // Db
		//{}, // D
		//{ C, F }, // Eb
		//{}, // E
		//{ Eb, Gb }, // F
		//{ G }, // Gb
		//{ Eb, Bb }, // G
		//{}, // Ab
		//{}, // A
		//{ C, Gb, G }, // Bb
		//{}, // B
	}
};

const Scale Chromatic = {
	"Chromatic",
	{
		{    Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B, }, // C
		{ C,     D, Eb, E, F, Gb, G, Ab, A, Bb, B, }, // Db
		{ C, Db,    Eb, E, F, Gb, G, Ab, A, Bb, B, }, // D
		{ C, Db, D,     E, F, Gb, G, Ab, A, Bb, B, }, // Eb
		{ C, Db, D, Eb,    F, Gb, G, Ab, A, Bb, B, }, // E
		{ C, Db, D, Eb, E,    Gb, G, Ab, A, Bb, B, }, // F
		{ C, Db, D, Eb, E, F,     G, Ab, A, Bb, B, }, // Gb
		{ C, Db, D, Eb, E, F, Gb,    Ab, A, Bb, B, }, // G
		{ C, Db, D, Eb, E, F, Gb, G,     A, Bb, B, }, // Ab
		{ C, Db, D, Eb, E, F, Gb, G, Ab,    Bb, B, }, // A
		{ C, Db, D, Eb, E, F, Gb, G, Ab, A,     B, }, // Bb
		{ C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb,    }, // B
	}
};

const Scale Dorian = {
	"Dorian",
	{
		// totally random any scale degree can jump to any other
		{    D, Eb, F, G, A, Bb }, // C
		{                       }, // Db
		{ C,    Eb, F, G, A, Bb }, // D
		{ C, D,     F, G, A, Bb }, // Eb
		{                       }, // E
		{ C, D, Eb,    G, A, Bb }, // F
		{                       }, // Gb
		{ C, D, Eb, F,    A, Bb }, // G
		{                       }, // Ab
		{ C, D, Eb, F, G,    Bb }, // A
		{ C, D, Eb, F, G, A,    }, // Bb
		{                       }, // B

		// trying to make "good" melodies
		//{ D, Eb, G, A }, // C
		//{}, // Db
		//{ C, F, A }, // D
		//{ C, G }, // Eb
		//{}, // E
		//{ Eb, G }, // F
		//{}, // Gb
		//{ D, A }, // G
		//{}, // Ab
		//{ F, Bb }, // A
		//{ D, G }, // Bb
		//{}, // B
	}
};

const Scale Mixolydian = {
	"Mixolydian",
	{
		// totally random any scale degree can jump to any other
		{    D, E, F, G, A, Bb }, // C
		{                      }, // Db
		{ C, E,    F, G, A, Bb }, // D
		{                      }, // Eb
		{ C, D,    F, G, A, Bb }, // E
		{ C, D, E,    G, A, Bb }, // F
		{                      }, // Gb
		{ C, D, E, F,    A, Bb }, // G
		{                      }, // Ab
		{ C, D, E, F, G,    Bb }, // A
		{ C, D, E, F, G, A     }, // Bb
		{                      }, // B

		// trying to make "good" melodies
		//{ D, E, A, Bb }, // C
		//{}, // Db
		//{ C, E, F, Bb }, // D
		//{}, // Eb
		//{ F, G }, // E
		//{ D, E, A }, // F
		//{}, // Gb
		//{ C, E, F, A, Bb }, // G
		//{}, // Ab
		//{ F, G, Bb }, // A
		//{ E, F }, // Bb
		//{}, // B
	}
};

const Scale* const Scales[] = { &Monotonic, &Pentatonic, &Major, &PentatonicMinor, &NaturalMinor, &HarmonicMinor, &Blues, &Chromatic, &Dorian, &Mixolydian };
const int ScalesLength = sizeof( Scales ) / sizeof( Scale* );