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
#include "Line.h"

using namespace Minim;

// Minim's ADSR is kiiiinda fucked, so decided to write our own for now
class ADSR : public UGen
{
public:
	ADSR(UGen& out);

	bool isOn() const { return mState == kAttack || mState == kDecay || mState == kSustain; }

	void noteOn(float amp, float attack, float decay, float sustain, float release);
	void noteOff();

	UGenInput audio;

protected:
	virtual void uGenerate(float * channels, const int numChannels) override;

	virtual void sampleRateChanged() override;

private:

	// will patch to this in noteOn if not already patched
	// and automatically unpatch when Release finishes.
	UGen& mOut;

	enum
	{
		kOff,
		kAttack,
		kDecay,
		kSustain,
		kRelease
	} mState;

	bool mAutoRelease;
	float mAmp, mAttack, mDecay, mSustain, mRelease;
	float mTime, mStep;
};

class Tone
{
public:
    Tone( Summer& out );
	~Tone();
	
	void setPulseWidth(float pw);
       
	void noteOn(Waveform* waveform, int tick, float fromFreq, int toNote, float glide, float amp, float attack, float decay, float sustain, float release, float fromPan, float toPan, float panDur);
	void noteOff();

	bool isOn() const { return adsr.isOn(); }
    int  getTick() const { return tick; }
	int  getMidiNote() const { return midiNote; }
	float getFrequency() const { return oscil.frequency.getLastValue(); }
	float getPan() const { return panner.pan.getLastValue(); }
    
private:

	// Waveform implementation that wraps another Waveform.
	// This allows us to construct an Oscil with an instance of this
	// and be able to swap the *actual* waveform without making a new Oscil.
	class Wave : public Waveform
	{
	public:
		Wave() : source(nullptr) {}

		Waveform* source;
		float 	  pw;
		// these are used in value, so we precompute whenever pw is set
		float 	  invpw; // 1/pw
		float     invwp; // 1/(1 - pw)

		// Waveform
		float value(const float at) const override;
	};
    
    Summer&		out;
	// we need to make a new one of these because, stupidly, Oscil deletes the Waveform it is constructed with
	Wave*		wave;
    Oscil		oscil;
	Line 		frequency;
    ADSR		adsr;
    Pan			panner;
	Line		pan;
    int			tick;
	int			midiNote;
};

#endif
