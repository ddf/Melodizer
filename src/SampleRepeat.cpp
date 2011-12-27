//
//  SampleRepeat.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "SampleRepeat.h"

#include <string.h> // mem functions
#include <cassert>

//----------------------------
SampleRepeat::SampleRepeat( const float sampleDuration )
: UGen(1)
, audio( *this, AUDIO )
, mBuffer(NULL)
, mBufferLength(0)
, mBufferDuration(sampleDuration)
, mSampleStart(0)
, mSampleEnd(0)
, mSampleHead(0)
, mState(ST_SAMPLE)
, mNextState(ST_SAMPLE)
{
    
}

//----------------------------
SampleRepeat::~SampleRepeat()
{
    if ( mBuffer )
    {
        delete [] mBuffer;
    }
}

//----------------------------
void SampleRepeat::sampleRateChanged()
{
    allocateBuffer();
}

//----------------------------
void SampleRepeat::channelCountChanged()
{
    allocateBuffer();
}

//----------------------------
void SampleRepeat::allocateBuffer()
{
    if ( mBuffer )
    {
        delete [] mBuffer;
        mBuffer = NULL;
        mBufferLength = 0;
    }
    
    if ( sampleRate() > 0 && getAudioChannelCount() > 0 )
    {
        mBufferLength = (int)(mBufferDuration*sampleRate()) * getAudioChannelCount();
        mBuffer = new float[ mBufferLength ];
    }
}

//----------------------------
void SampleRepeat::setRepeatSection(const float start, const float end)
{
    assert( start < end );
    mSampleStart = (int)(start*mBufferLength);
    mSampleEnd   = (int)(end*mBufferLength);
}

//----------------------------
void SampleRepeat::activate()
{
    mNextState = ST_REPEAT;
}

//----------------------------
void SampleRepeat::deactivate()
{
    mNextState = ST_SAMPLE;
}

//----------------------------
void SampleRepeat::uGenerate(float * out, const int numChannels)
{
    if ( !mBuffer )
    {
        UGen::fill(out, 0, numChannels);
        return;
    }
    
    switch (mState) 
    {
        case ST_SAMPLE:
        {
            if ( mSampleHead >= mBufferLength )
            {
                mSampleHead = 0;
            }
            
            memcpy( mBuffer + mSampleHead, audio.getLastValues(), sizeof(float)*numChannels );
            // output what we just sampled
            memcpy( out, mBuffer + mSampleHead, sizeof(float)*numChannels );
        }
        break;
            
        case ST_REPEAT:
        {
            if ( mSampleHead >= mSampleEnd )
            {
                mSampleHead = mSampleStart;
            }
            
            memcpy( out, mBuffer + mSampleHead, sizeof(float)*numChannels );
        }
        break;
            
        default:
            break;
    }
    
    mSampleHead += getAudioChannelCount();
    mState       = mNextState;
}

