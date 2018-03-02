//
//  Instruments.h
//  melodizer
//
//  Created by Damien Di Fede on 12/7/11.
//  Copyright (c) 2011 Damien Di Fede. All rights reserved.
//

#ifndef melodizer_Instruments_h
#define melodizer_Instruments_h

#include "Waveform.h"
#include "Oscil.h"
#include "Summer.h"
#include "Pan.h"
#include "ADSR.h"

using namespace Minim;

class Tone
{
public:
    Tone( Summer& out );
	~Tone();
       
	void noteOn(float dur, Waveform* waveform, int tick, float freq, float amp, float pan);
	void noteOff();

    int  getTick() const { return tick; }
    
private:

	// Waveform implementation that wraps another Waveform.
	// This allows us to construct an Oscil with an instance of this
	// and be able to swap the *actual* waveform without making a new Oscil.
	class Wave : public Waveform
	{
	public:
		Wave() : source(nullptr) {}

		Waveform* source;

		// Waveform
		float value(const float at) const override;
	};
    
    Summer&		out;
	// we need to make a new one of these because, stupidly, Oscil deletes the Waveform it is constructed with
	Wave*		wave;
    Oscil		oscil;
    ADSR		adsr;
    Pan			panner;
    int			tick;
};

#endif
