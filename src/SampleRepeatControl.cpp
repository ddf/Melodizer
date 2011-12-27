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

//-----------------------------------
bool SampleRepeatControl::active() const
{
    return ( mTouch1.id != -1 && mTouch2.id != -1 );
}

//-----------------------------------
ofPoint SampleRepeatControl::touchPosition1() const
{
    return ofPoint( mTouch1.x, mTouch1.y );
}

//-----------------------------------
ofPoint SampleRepeatControl::touchPosition2() const
{
    return ofPoint( mTouch2.x, mTouch2.y );
}

//-----------------------------------
void SampleRepeatControl::setSection()
{
    float s = ofClamp( -0.001f + mTouch1.x / ofGetWidth(), 0, 1 );
    float e = ofClamp( 0.001f + mTouch2.x / ofGetWidth(), 0, 1 );
    if ( s > e ) std::swap(s,e);
    if ( e-s < 0.001f ) e = s+0.001f;
    
    
    mRepeater.setRepeatSection( s, e );    
}

//-----------------------------------
bool SampleRepeatControl::touchDown( ofTouchEventArgs& touch )
{
    if ( mTouch1.id == -1 )
    {
        mTouch1 = touch;
    }
    else if ( mTouch2.id == -1 )
    {
        mTouch2 = touch;
    }
    
    if ( active() )
    {
        setSection();
        
        mRepeater.activate();
    }
    
    return (mTouch1.id != -1 || mTouch2.id != -1);
}

//-----------------------------------
bool SampleRepeatControl::touchMoved( ofTouchEventArgs& touch )
{
    if ( mTouch1.id == touch.id )
    {
        mTouch1 = touch;
    }
    else if ( mTouch2.id == touch.id )
    {
        mTouch2 = touch;
    }
    
    if ( active() )
    {
        setSection();
    }
    
    return active();
}

//-----------------------------------
bool SampleRepeatControl::touchUp( ofTouchEventArgs& touch )
{
    if ( mTouch1.id == touch.id )
    {
        mTouch1.id = -1;
    }
    else if ( mTouch2.id == touch.id )
    {
        mTouch2.id = -1;
    }
    
    if ( !active() )
    {
        mRepeater.deactivate();
    }
    
    return active();
}
