//
//  SampleRepeat.h
//  melodizer
//
//  Created by Damien Di Fede on 12/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_SampleRepeat_h
#define melodizer_SampleRepeat_h

#include "UGen.h"

class SampleRepeat : public Minim::UGen
{
public:
    // we constantly sample this duration
    // and when activated begin repeating 
    // only as much of the sampled buffer as 
    // is specified in setRepeatDuration
    SampleRepeat( const float sampleDuration );
    virtual ~SampleRepeat();
    
    float   getBufferDuration() const { return mBufferDuration; }
    
    // normalized
    void    setRepeatSection( const float start, const float end );
    
    void    activate();
    void    deactivate();
    
    
    UGenInput audio;
    
protected:
    
    virtual void sampleRateChanged();
    virtual void channelCountChanged();
    virtual void uGenerate( float * out, const int numChannels );
    
private:
    
    void        allocateBuffer();
    
    // where we store what we sample
    float *     mBuffer;
    int         mBufferLength;
    
    float       mBufferDuration; // total length of buffer in seconds
    
    int         mSampleStart;    // what sample we start repeating at
    int         mSampleEnd;      // what sample we wrap at when repeating
    int         mSampleHead;     // where we read/write
    
    enum State
    {
        ST_SAMPLE,
        ST_REPEAT
    };
    
    State mState;
    State mNextState;
};


#endif
