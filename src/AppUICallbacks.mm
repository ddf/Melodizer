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
}

//--------------------------------------------------------------
void App::touchMoved(ofTouchEventArgs &touch)
{
	if ( touch.id == 0 )
    {
        mXOff = ofMap(touch.x, 0, ofGetWidth(), 55, 75);
        mYOff = ofMap(touch.y, 0, ofGetHeight(), 25, 45);
        
        mRate.value.setLastValue( ofMap(touch.x, 0, ofGetWidth(), 1.0f, 0.2f) );
        Settings::Tempo = ofMap( touch.y, 0, ofGetHeight(), 240, 40 );
    }
    else if ( touch.id == 1 )
    {
        mDelay.delTime.setLastValue( ofMap(touch.x, 0, ofGetWidth(), 0.001f, 0.625f) );
        mDelay.delAmp.setLastValue( ofMap(touch.y, 0, ofGetHeight(), 0.8f, 0.f) );
    }
    else if ( touch.id == 2 )
    {
        mFilter.frequency.setLastValue( ofMap(touch.x, 0, ofGetWidth(), 20, 14400) );
        mFilter.resonance.setLastValue( ofMap(touch.y, 0, ofGetHeight(), 0.8f, 0.f) );
    }
}

//--------------------------------------------------------------
void App::touchUp(ofTouchEventArgs &touch)
{
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
}

//--------------------------------------------------------------
void App::handlePinchGesture()
{
}