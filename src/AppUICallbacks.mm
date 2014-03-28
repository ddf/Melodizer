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
    if ( mSettingsScreen.active() ) return;
    
    if ( mFlangerControl.touchDown(touch) )
        return;
 
//    if ( mSampleRepeatControl.touchDown(touch) )
//        return;
    
    if ( mButtonOpenSettings.mBox.contains(touch.x, touch.y) )
    {
        mSettingsScreen.show();
    }
}

//--------------------------------------------------------------
void App::touchMoved(ofTouchEventArgs &touch)
{
    if ( mSettingsScreen.active() ) return;
    
    if ( mFlangerControl.touchMoved(touch) )
        return;
    
//    if ( mSampleRepeatControl.touchMoved(touch) )
//        return;
}

//--------------------------------------------------------------
void App::touchUp(ofTouchEventArgs &touch)
{
    if ( mSettingsScreen.active() ) return;
    
    if ( mFlangerControl.touchUp(touch) )
        return;
    
//    if ( mSampleRepeatControl.touchUp(touch) )
//        return;
}

//--------------------------------------------------------------
void App::touchDoubleTap(ofTouchEventArgs &touch)
{
    if ( mSettingsScreen.active() ) return;
}

//--------------------------------------------------------------
void App::touchCancelled(ofTouchEventArgs &touch)
{
    if ( mSettingsScreen.active() ) return;
    
//    if ( mSampleRepeatControl.touchUp(touch) )
//        return;
    
    if ( mFlangerControl.touchUp(touch) )
        return;
}

//--------------------------------------------------------------
void App::handlePinchGesture()
{
}