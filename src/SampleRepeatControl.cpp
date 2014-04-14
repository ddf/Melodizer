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
SampleRepeatControl::SampleRepeatControl( SampleRepeat& repeater )
: XYControl()
, mRepeater(repeater)
, mRepeatStart(0)
, mRepeatEnd(1)
{
    setXValue(&mRepeatStart, 0, 0.48f);
    setYValue(&mRepeatEnd, 1.0f, 0.5f );
}

SampleRepeatControl::~SampleRepeatControl()
{
    
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
}

//-----------------------------------
void SampleRepeatControl::inputChanged()
{
    mRepeater.setRepeatSection( mRepeatStart, mRepeatEnd );
}