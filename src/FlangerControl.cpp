//
//  FlangerControl.cpp
//  melodizer
//
//  Created by Damien Di Fede on 2/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "FlangerControl.h"
#include "ofMath.h"

//-----------------------------------
void FlangerControl::activate()
{
    mWetLine.activate( 0.01f, mFlanger.wet.getLastValue(), 0.5f );
}

//-----------------------------------
void FlangerControl::deactivate()
{
    mWetLine.activate( 0.01f, mFlanger.wet.getLastValue(), 0 );
}

//-----------------------------------
void FlangerControl::setParameters()
{
    const float rate    = ofMap( getTouch(0).distance( getTouch(1) ), 0, 1024, 2.0f, 0.001f );
    const float depth   = ofMap( getTouch(1).distance( getTouch(2) ), 0, 1024, 10, 100 );
    const float feed    = ofMap( getTouch(2).distance( getTouch(0) ), 0, 1024, 0.1f, 0.85f );
    
    mFlanger.rate.setLastValue( rate );
    mFlanger.depth.setLastValue( depth );
    mFlanger.feedback.setLastValue( feed );
}