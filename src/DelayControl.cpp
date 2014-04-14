//
//  DelayControl.cpp
//  melodizer
//
//  Created by Damien Di Fede on 4/14/14.
//
//

#include "DelayControl.h"

DelayControl::DelayControl( Minim::Delay& delay )
: XYControl()
, mDelay( delay )
, mWetLine( 0, 0, 0 )
{
    mWetLine.patch( mDelay.wetMix );
    
    setXValue(mDelay.delTime.getLastValues(), 0.1f, 0.5f);
    setYValue(mDelay.delAmp.getLastValues(), 0, 0.9f );
}

//-----------------------------------
void DelayControl::enable()
{
    mWetLine.activate( 0.01f, mDelay.wetMix.getLastValue(), 1.0f );
}

//-----------------------------------
void DelayControl::disable()
{
    mWetLine.activate( 0.01f, mDelay.wetMix.getLastValue(), 0 );
}