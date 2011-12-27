/*
 *  AppUICallbacks.mm
 *  noiseShaper
 *
 *  Created by Damien Di Fede on 6/25/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "App.h"

#include "ofxiPhoneExtras.h"

//--------------------------------------------------------------
void App::touchDown(ofTouchEventArgs &touch)
{
    if ( mXYControl.touchDown(touch) )
        return;
 
    if ( mSampleRepeatControl.touchDown(touch) )
        return;
}

//--------------------------------------------------------------
void App::touchMoved(ofTouchEventArgs &touch)
{
	if ( mXYControl.touchMoved(touch) )
    {
        mXOff = ofMap(touch.x, 0, ofGetWidth(), 55, 75);
        mYOff = ofMap(touch.y, 0, ofGetHeight(), 25, 45);
        
        return;
    }
    
    if ( mSampleRepeatControl.touchMoved(touch) )
        return;
}

//--------------------------------------------------------------
void App::touchUp(ofTouchEventArgs &touch)
{
    if ( mXYControl.touchUp(touch) )
        return;
    
    if ( mSampleRepeatControl.touchUp(touch) )
        return;
}

//--------------------------------------------------------------
void App::touchDoubleTap(ofTouchEventArgs &touch)
{
    mSettingsScreen.show();
    ofUnregisterTouchEvents( this );
}

//--------------------------------------------------------------
void App::touchCancelled(ofTouchEventArgs &touch)
{
    if ( mXYControl.touchUp(touch) )
        return;
    
    if ( mSampleRepeatControl.touchUp(touch) )
        return;
}

//--------------------------------------------------------------
void App::handlePinchGesture()
{
}