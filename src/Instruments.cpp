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
ADSR::ADSR()
: UGen()
, audio( *this, AUDIO )
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
, panner( 0 )
, adsr()
{
    adsr.setAudioChannelCount( 2 );
    oscil.patch( panner ).patch ( adsr ).patch( out );
}

Tone::~Tone()
{
	// we don't delete wave because Oscil will do it.
}

float Tone::Wave::value(const float at) const
{
	return source != nullptr ? source->value(at) : 0.0f;
}

void Tone::noteOn( Waveform* waveform, int tick, float freq, float amp, float attack, float decay, float sustain, float release, float pan )
{
	wave->source = waveform;
    oscil.frequency.setLastValue( freq );
    panner.pan.setLastValue( pan );
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
