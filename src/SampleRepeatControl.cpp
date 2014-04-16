//
//  SampleRepeatControl.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "SampleRepeatControl.h"
#include "ofMain.h"
#include "Settings.h"

static const float sk_animDuration = 0.2f;

class Bounce
{
public:
    static float easeOut (float t, float b, float c, float d)
    {
        t/=d;
        if (t < (1/2.75f))
        {
            return c*(7.5625f*t*t) + b;
        }
        else if (t < (2/2.75f))
        {
            t-=(1.5f/2.75f);
            return c*(7.5625f*t*t + .75f) + b;
        }
        else if (t < (2.5f/2.75f))
        {
            t-=(2.25f/2.75f);
            return c*(7.5625f*t*t + .9375f) + b;
        }
        else
        {
            t-=(2.625f/2.75f);
            return c*(7.5625f*t*t + .984375f) + b;
        }
    }
    
    static float easeIn (float t, float b, float c, float d)
    {
        return c - easeOut (d-t, 0, c, d) + b;
    }
    
    static float easeInOut (float t, float b, float c, float d)
    {
        if (t < d/2) return easeIn (t*2, 0, c, d) * .5f + b;
        else return easeOut (t*2-d, 0, c, d) * .5f + c*.5f + b;
    }
};

//-----------------------------------
SampleRepeatControl::SampleRepeatControl( SampleRepeat& repeater )
: XYControl()
, mRepeater(repeater)
, mRepeatCenter(0.5f)
, mRepeatLength(0.5f)
, mWaveSectionAnim(0)
{
    setXValue(&mRepeatCenter, 0, 1);
    setYValue(&mRepeatLength, 0.01f, 0.52f);
}

SampleRepeatControl::~SampleRepeatControl()
{
    
}

//-----------------------------------
void SampleRepeatControl::update(const float dt)
{
    XYControl::update(dt);
    
    if ( mRepeater.getState() == SampleRepeat::ST_REPEAT && mWaveSectionAnim < sk_animDuration )
    {
        mWaveSectionAnim += dt;
        
        if ( mWaveSectionAnim > sk_animDuration )
        {
            mWaveSectionAnim = sk_animDuration;
        }
    }
}

//-----------------------------------
void SampleRepeatControl::enable()
{
    mRepeater.activate();
}

//-----------------------------------
void SampleRepeatControl::disable()
{
    mRepeater.deactivate();
    mWaveSectionAnim = 0;
}

//-----------------------------------
void SampleRepeatControl::inputChanged()
{
    float start = ofClamp(mRepeatCenter-mRepeatLength, 0, 1);
    float end   = ofClamp(mRepeatCenter+mRepeatLength, 0, 1);
    mRepeater.setRepeatSection( start, end );
}

//-----------------------------------
void SampleRepeatControl::drawBoxBackground(const Box& controlBox)
{
    // get the fill
    XYControl::drawBoxBackground(controlBox);
    
    // build peaks
    // this is super inefficient to do this every frame
    // but easier than tracking lots of state
    // and doesn't seem to impact performance at all
    {
        if ( mWavePeaks.empty() )
        {
            const int numPeaks = (int)controlBox.mW;
            mWavePeaks.resize(numPeaks);
            
            const int startX = (int)controlBox.mMinX;
            for( int i = 0; i < numPeaks; ++i )
            {
                mWaveLine.addVertex(ofPoint(startX+i, controlBox.mY));
                mWaveLine.addVertex(ofPoint(startX+i, controlBox.mY));
            }
        }
        
        // and now show sampled waveform and looping area
        mRepeater.getPeaks( mWavePeaks );
        auto& verts = mWaveLine.getVertices();
        for( int p = 0; p < mWavePeaks.size(); ++p )
        {
            const float peak = ofClamp(mWavePeaks[p]*controlBox.mH/2, 1, controlBox.mH/2);
            verts[p*2].y     = controlBox.mY - peak;
            verts[p*2 + 1].y = controlBox.mY + peak;
        }
    }
    
    ofSetColor(100);
    ofSetLineWidth(1);
    
    //mWaveLine.draw();
    // we're use ofPolyline as just a vert array container, so we do the gl calls direct
    // because ofPolyline doesn't have a GL_LINES mode
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &mWaveLine.getVertices()[0].x);
    glDrawArrays(GL_LINES, 0, mWaveLine.size());
    
    ofSetColor(0, 255, 0, 128);
    
    const float boxHeight = Bounce::easeOut( mWaveSectionAnim, 0, controlBox.mH, sk_animDuration );
    if ( boxHeight > 0 )
    {
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofRect(controlBox.mMinX + mRepeatCenter*controlBox.mW, controlBox.mY, controlBox.mW*mRepeatLength*2, boxHeight);
    }
    
}