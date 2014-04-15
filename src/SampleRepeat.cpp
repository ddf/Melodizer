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
#include <math.h>

//----------------------------
SampleRepeat::SampleRepeat( const float sampleDuration )
: UGen()
, audio( *this, AUDIO )
, mBuffer(NULL)
, mBufferLength(0)
, mBufferDuration(sampleDuration)
, mSampleStart(0)
, mSampleEnd(0)
, mSampleHead(0)
, mState(ST_INACTIVE)
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
        memset(mBuffer, 0, sizeof(float)*mBufferLength);
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
void SampleRepeat::getPeaks( std::vector<float>& outPeaks )
{
    int hop       = getAudioChannelCount();
    int sliceSize = (mBufferLength/hop) / outPeaks.size();
    
    for( int p = 0; p < outPeaks.size(); ++p )
    {
        float peak = 0;
        for( int s = 0; s < sliceSize; ++s )
        {
            int offset = p * sliceSize * hop + s * hop;
            float mono = 0;
            for ( int c = 0; c < hop; ++c )
            {
                mono += mBuffer[offset+c];
            }
            mono /= hop;
            peak += (mono*mono);
        }
        outPeaks[p] = sqrtf(peak/sliceSize);
    }
}

//----------------------------
void SampleRepeat::activate()
{
    mSampleHead = 0;
    mState      = ST_SAMPLE;
}

//----------------------------
void SampleRepeat::deactivate()
{
    mState = ST_INACTIVE;
    memset(mBuffer, 0, sizeof(float)*mBufferLength);
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
                mSampleHead = mSampleStart;
                mState      = ST_REPEAT;
            }
            else
            {
                memcpy( mBuffer + mSampleHead, audio.getLastValues(), sizeof(float)*numChannels );
            }
            
            memcpy( out, mBuffer + mSampleHead, sizeof(float)*numChannels );
            
            mSampleHead += getAudioChannelCount();
        }
        break;
            
        case ST_REPEAT:
        {
            if ( mSampleHead >= mSampleEnd )
            {
                mSampleHead = mSampleStart;
            }
            
            memcpy( out, mBuffer + mSampleHead, sizeof(float)*numChannels );
            
            mSampleHead += getAudioChannelCount();
        }
        break;
            
        case ST_INACTIVE:
        {
            memcpy( out, audio.getLastValues(), sizeof(float)*numChannels );
        }
        break;
            
        default:
            break;
    }
    

}

