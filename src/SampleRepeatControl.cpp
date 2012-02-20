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
void SampleRepeatControl::activate()
{
    mRepeater.activate();
}

//-----------------------------------
void SampleRepeatControl::deactivate()
{
    mRepeater.deactivate();
}

//-----------------------------------
void SampleRepeatControl::setParameters()
{
    float s = ofClamp( -0.001f + getTouch(0).x / ofGetWidth(), 0, 1 );
    float e = ofClamp(  0.001f + getTouch(1).x / ofGetWidth(), 0, 1 );
    if ( s > e ) std::swap(s,e);
    if ( e-s < 0.001f ) e = s+0.001f;
    
    
    mRepeater.setRepeatSection( s, e );
}