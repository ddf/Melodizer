//
//  XYControl.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "XYControl.h"

#include "ofMain.h"
#include "ofTrueTypeFont.h"

static ofTrueTypeFont xyFont;

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
        *value = ofMap( input, inputMin, inputMax, outputMin, outputMax, true );
    }
}

//--- XYControl ------------------------
XYControl::XYControl()
: mTouchID(-1)
, mEnabled(false)
, mLabel("")
, mControlBox(0,0,0,0)
, mButtonPower("",0,0,0,0)
{
    
}

//--------------------------------------
void XYControl::setup(const char * label, const float cx, const float cy, const float dim)
{
    mLabel         = label;
    
    mControlBox.mW = mControlBox.mH = dim;
    
    const float screenScale = (float)ofGetHeight() / 768.0f;
    mButtonPower.mBox.mW    = 64*screenScale;
    mButtonPower.mBox.mH    = 64*screenScale;
    
    setPosition(cx, cy);
    
    if ( !xyFont.isLoaded() )
    {
        xyFont.loadFont( "HelveticaBold.ttf", 24*screenScale );
    }
}

//--------------------------------------
void XYControl::draw()
{
    const float screenScale = ((float)ofGetHeight() / 768.f);
    
    ofNoFill();
    ofRect(mControlBox.mX, mControlBox.mY, mControlBox.mW, mControlBox.mH);
    
    ofFill();
    ofSetColor(255, 0, 0);
    ofCircle( mControlPoint.x, mControlPoint.y, 32.f * screenScale );
    
    if ( mEnabled )
    {
        ofSetColor(255, 128, 0);
    }
    else
    {
        ofSetColor(200, 200, 200);
    }
    
    ofRect(mButtonPower.mBox.mX, mButtonPower.mBox.mY, mButtonPower.mBox.mW, mButtonPower.mBox.mH);
    
    ofSetColor(255, 255, 255);
    xyFont.drawString(mLabel, mButtonPower.mBox.mMaxX +  5.f*screenScale, mButtonPower.mBox.mMaxY );
}

//--------------------------------------
void XYControl::setPosition( const float x, const float y )
{
    const float screenScale = ((float)ofGetHeight() / 768.f);
    
    mControlBox.setCenter(x, y);
    mButtonPower.mBox.setCenter( mControlBox.mMinX + mButtonPower.mBox.mW/2, mControlBox.mMinY - mButtonPower.mBox.mH/2 - 10.f*screenScale );
    
    mXControl.inputMin = mControlBox.mMinX;
    mXControl.inputMax = mControlBox.mMaxX;
    
    mYControl.inputMin = mControlBox.mMaxY;
    mYControl.inputMax = mControlBox.mMinY;
    
    mapInput(x, y);
}

//--------------------------------------
void XYControl::setXValue( float * value, const float valueMin, const float valueMax )
{
    mXControl.outputMin = valueMin;
    mXControl.outputMax = valueMax;
    mXControl.value     = value;
}

//--------------------------------------
void XYControl::setYValue( float * value, const float valueMin, const float valueMax )
{
    mYControl.outputMin = valueMin;
    mYControl.outputMax = valueMax;
    mYControl.value     = value;
}

//--------------------------------------
void XYControl::mapInput( float x, float y)
{
    x = ofClamp(x, mControlBox.mMinX, mControlBox.mMaxX);
    y = ofClamp(y, mControlBox.mMinY, mControlBox.mMaxY);
    
    mXControl.map(x);
    mYControl.map(y);
    mControlPoint.set(x,y);
    
    inputChanged();
}

//--------------------------------------
bool XYControl::touchDown( ofTouchEventArgs& touch )
{
    if ( mTouchID == -1 && mControlBox.contains(touch.x, touch.y) )
    {
        mTouchID = touch.id;
        mapInput(touch.x, touch.y);
    }
    
    if ( mButtonPower.mBox.contains(touch.x, touch.y) )
    {
        mEnabled = !mEnabled;
        if ( mEnabled )
        {
            enable();
        }
        else
        {
            disable();
        }
    }
    
    return (mTouchID == touch.id);
}

//--------------------------------------
bool XYControl::touchMoved( ofTouchEventArgs& touch )
{
    bool bMine = (touch.id == mTouchID);
    
    if ( bMine )
    {
        mapInput(touch.x, touch.y);
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

