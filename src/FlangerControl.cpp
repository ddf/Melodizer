//
//  FlangerControl.cpp
//  melodizer
//
//  Created by Damien Di Fede on 2/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "FlangerControl.h"

FlangerControl::FlangerControl( Minim::Flanger& flange )
: XYControl()
, mFlanger( flange )
, mWetLine( 0, 0, 0 )
{
    mWetLine.patch( mFlanger.wet );
    
    setXValue(mFlanger.rate.getLastValues(), 0.001f, 2.0f);
    setYValue(mFlanger.depth.getLastValues(), 10, 100 );
}

//-----------------------------------
void FlangerControl::enable()
{
    mWetLine.activate( 0.01f, mFlanger.wet.getLastValue(), 0.5f );
}

//-----------------------------------
void FlangerControl::disable()
{
    mWetLine.activate( 0.01f, mFlanger.wet.getLastValue(), 0 );
}