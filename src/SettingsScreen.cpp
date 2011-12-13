//
//  SettingsScreen.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "SettingsScreen.h"

#include "ofMain.h"

static const float sk_AnimLength = 0.5f;

SettingsScreen::SettingsScreen()
: mState(ST_HIDDEN)
, mAnimTimer(0)
{
}

//----------------------------------
void SettingsScreen::setup()
{
    mMinX = ofGetWidth() * 0.1f;
    mMaxX = ofGetWidth() - mMinX;
    mMinY = ofGetHeight() * 0.1f;
    mMaxY = ofGetHeight() - mMinY;
}

//----------------------------------
void SettingsScreen::update( const float dt )
{
    if ( mAnimTimer > 0 )
    {
        mAnimTimer -= dt;
        if ( mAnimTimer < 0 ) mAnimTimer = 0;
    }
    
    switch ( mState )
    {
        case ST_SHOWING:
        {
            if ( mAnimTimer == 0 )
            {
                ofRegisterTouchEvents(this);
                mState = ST_SHOWN;
            }
        }
        break;
            
        case ST_SHOWN:
        {
            
        }
        break;
            
        case ST_HIDING:
        {
            if ( mAnimTimer == 0 )
            {
                mState = ST_HIDDEN;
            }
        }
        break;
            
        case ST_HIDDEN:
        {
        }
        break;
    }
}

//----------------------------------
bool SettingsScreen::active() const
{
    return mState != ST_HIDDEN;
}

//----------------------------------
void SettingsScreen::draw()
{
    //ofPushStyle();
    {
        ofSetRectMode( OF_RECTMODE_CENTER );
        
        ofSetColor( 30, 30, 30 );
        ofFill();
        
        float scale = 1.0f;
        if ( mState == ST_SHOWING )
        {
            scale -= mAnimTimer / sk_AnimLength;
        }
        else if ( mState == ST_HIDING )
        {
            scale = mAnimTimer / sk_AnimLength;
        }
        ofRect( ofGetWidth() / 2, ofGetHeight() / 2, (mMaxX - mMinX)*scale, (mMaxY - mMinY)*scale );
    }
    //ofPopStyle();
}

//----------------------------------
void SettingsScreen::show()
{
    mAnimTimer = sk_AnimLength;
    mState = ST_SHOWING;
}

//----------------------------------
void SettingsScreen::hide()
{
    ofUnregisterTouchEvents(this);
    mAnimTimer = sk_AnimLength;
    mState = ST_HIDING;
    
}

//----------------------------------
void SettingsScreen::touchDown( ofTouchEventArgs& touch )
{
    if ( touch.x < mMinX || touch.x > mMaxX || touch.y < mMinY || touch.y > mMaxY )
    {
        hide();
        
    }
}

//----------------------------------
void SettingsScreen::touchMoved( ofTouchEventArgs& touch )
{
    
}

//----------------------------------
void SettingsScreen::touchDoubleTap( ofTouchEventArgs& touch )
{
    
}

//----------------------------------
void SettingsScreen::touchUp( ofTouchEventArgs& touch )
{
    
}

//----------------------------------
void SettingsScreen::touchCancelled( ofTouchEventArgs& touch )
{
    
}