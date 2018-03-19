//
//  Instruments.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/7/11.
//  Copyright (c) 2011 Damien Di Fede. All rights reserved.
//

#include "Instruments.h"

#include "Wavetable.h"
#include "Scales.h"

//---------------------------------------
//-- ADSR
//---------------------------------------
ADSR::ADSR(UGen& out)
: UGen()
, audio( *this, AUDIO )
, mOut(out)
, mState(kOff)
, mAutoRelease(false)
, mAmp(0)
, mAttack(0)
, mDecay(0)
, mSustain(0)
, mRelease(0)
, mStep(0)
, mTime(0)
{

}

void ADSR::sampleRateChanged()
{
	mStep = 1.0f / sampleRate();
}

void ADSR::noteOn(float amp, float attack, float decay, float sustain, float release)
{
	mTime = 0;
	mAmp = amp;
	mAttack = attack;
	mDecay = decay;
	mSustain = sustain;
	mRelease = release;
	mAutoRelease = false;

	if (mAttack > 0) mState = kAttack;
	else if (mDecay > 0) mState = kDecay;
	else mState = kSustain;

	if (!isPatched())
	{
		patch(mOut);
	}
}

void ADSR::noteOff()
{
	if (mState == kSustain)
	{
		mState = kRelease;
		mTime = 0;
	}
	else
	{
		mAutoRelease = true;
	}
}

void ADSR::stop()
{
	mState = kOff;
	mAmp = 0;
	if (isPatched())
	{
		unpatch(mOut);
	}
}

void ADSR::uGenerate(float * channels, const int numChannels)
{
	// when we are off, our amplitude should be zero, so we start with that and don't bother handling that case
	float amp = 0;
	switch (mState)
	{
	case kAttack:
		if (mTime >= mAttack)
		{
			amp = mAmp;
			mState = kDecay;
			mTime = 0;
		}
		else
		{
			amp = mAmp * (mTime / mAttack);
		}
		break;

	case kDecay:
		if (mTime >= mDecay)
		{
			amp = mAmp * mSustain;
			mState = kSustain;
		}
		else
		{
			const float t = mTime / mDecay;
			amp = mAmp + t * (mAmp*mSustain - mAmp);
		}
		break;

	case kSustain:
		amp = mAmp * mSustain;		
		if (mAutoRelease)
		{
			mState = kRelease;
			mTime = 0;
		}
		break;

	case kRelease:
		if (mTime >= mRelease)
		{
			amp = 0;
			mState = kOff;
			unpatch(mOut);
		}
		else
		{
			const float t = mTime / mRelease;
			const float rAmp = mAmp*mSustain;
			amp = rAmp - t*rAmp;
		}
		break;
	}

	mTime += mStep;

	for (int i = 0; i < numChannels; ++i)
	{
		channels[i] = audio.getLastValues()[i] * amp;
	}
}

//---------------------------------------
//-- TONE
//---------------------------------------
Tone::Tone( Summer& inSummer )
: out( inSummer )
, wave(new Wave())
, oscil( 0, 1, wave )
, frequency(0,0,0)
, panner( 0 )
, pan(0,0,0)
, adsr(inSummer)
, midiNote(0)
{
	frequency.patch( oscil.frequency );
	pan.patch(panner.pan);
    adsr.setAudioChannelCount( 2 );
    oscil.patch( panner ).patch ( adsr );
}

Tone::~Tone()
{
	// we don't delete wave because Oscil will do it.
}

void Tone::setPulseWidth(float pw)
{
	wave->pw = pw;
	wave->invpw = 1.0f / pw;
	wave->invwp = 1.0f / (1-pw);
}

float Tone::Wave::value(const float at) const
{
	if ( source == nullptr ) return 0;
	const float lu = at <= pw ? at*invpw*0.5f : 0.5f + (at-pw)*invwp*0.5f;
	return source->value(lu);
}

void Tone::noteOn( Waveform* waveform, int tick, float fromFreq, int toNote, float glide, float amp, float attack, float decay, float sustain, float release, float fromPan, float toPan, float panDur )
{
	wave->source = waveform;
	midiNote = toNote;
	const float toFreq = Frequency::ofMidiNote(midiNote).asHz();
	if (fromFreq == 0)
	{
		frequency.activate(0, toFreq, toFreq);
	}
	else
	{
		frequency.activate(glide, fromFreq, toFreq);
	}
	pan.activate(panDur, fromPan, toPan);
    tick = tick;
    
    adsr.noteOn( amp, attack, decay, sustain, release );
}

void Tone::noteOff()
{
	if (adsr.isOn())
	{
		adsr.noteOff();
	}
}

void Tone::stop()
{
	adsr.stop();
	// reset frequency and pan so that Glide and Move behave as expected when Playing from Stopped state
	frequency.activate(0, 0, 0);
	oscil.frequency.setLastValue(0);
	pan.activate(0, 0, 0);
	panner.pan.setLastValue(0);
}