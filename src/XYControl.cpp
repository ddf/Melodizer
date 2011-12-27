//
//  XYControl.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "XYControl.h"

#include "ofMain.h"

//--- ValueMapper ----------------------
ValueMapper::ValueMapper()
: value(NULL)
, inputMin(0)
, inputMax(0)
, outputMin(0)
, outputMax(0)
{
    
}

//--------------------------------------
ValueMapper::ValueMapper( float * v, float imin, float imax, float omin, float omax )
: value( v )
, inputMin( imin )
, inputMax( imax )
, outputMin( omin )
, outputMax( omax )
{
    
}

//--------------------------------------
void ValueMapper::map( const float input )
{
    if ( value )
    {
        *value = ofMap( input, inputMin, inputMax, outputMin, outputMax );
    }
}

//--- XYControl ------------------------
XYControl::XYControl()
: mTouchID(-1)
, mP(ofGetWidth()/2, ofGetHeight()/2)
, mR(60)
{
    
}

//--------------------------------------
void XYControl::setPosition( const float x, const float y )
{
    mP.x = x;
    mP.y = y;
    
    mXControl.map( x );
    mYControl.map( y );
}

//--------------------------------------
void XYControl::setXControl( ValueMapper mapper )
{
    mXControl = mapper;
}

//--------------------------------------
void XYControl::setYControl( ValueMapper mapper )
{
    mYControl = mapper;
}

//--------------------------------------
bool XYControl::touchDown( ofTouchEventArgs& touch )
{
    if ( mTouchID == -1 && ofDist(touch.x, touch.y, mP.x, mP.y) < mR )
    {
        mTouchID = touch.id;
        mLastTouch.x = touch.x;
        mLastTouch.y = touch.y;
    }
    
    return (mTouchID == touch.id);
}

//--------------------------------------
bool XYControl::touchMoved( ofTouchEventArgs& touch )
{
    bool bMine = (touch.id == mTouchID);
    
    if ( bMine )
    {
        mP.x += touch.x - mLastTouch.x;
        mP.y += touch.y - mLastTouch.y;
        mLastTouch.x = touch.x;
        mLastTouch.y = touch.y;
        
        mXControl.map( mP.x );
        mYControl.map( mP.y );
    }
    
    return bMine;
}

//--------------------------------------
bool XYControl::touchUp( ofTouchEventArgs& touch )
{
    bool bMine = (touch.id == mTouchID );
    
    if ( bMine )
    {
        mTouchID = -1;
    }
    
    return bMine;
}

