//
//  SettingsScreen.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "SettingsScreen.h"
#include "Settings.h"

static const float sk_AnimLength = 0.25f;

/////////////////////////////////////////
//
// SLIDER 
// 
/////////////////////////////////////////
void ValueSlider::draw()
{
    // background
    {
        ofSetColor(60, 60, 60);
        ofRect( mX, mY, mW, mH );
    }
    
    // fill
    if ( mValue > 0 )
    {        
        // adjust brightness based on amt of fill
        mColor.setBrightness( 64 + 190 * (*mValue) );
        ofSetColor(mColor);
        
        float fH = mH * (*mValue);
        
        ofRect(mX, mMaxY - fH/2, mW, fH);
    }
}

//----------------------------------
bool ValueSlider::handleTouch(const float x, const float y)
{
    // inside our box?
    if ( x > mMinX && x < mMaxX && y > mMinY && y < mMaxY )
    {
        *mValue = (mMaxY - y) / mH;
        return true;
    }
    
    return false;
}

////////////////////////////////////
//
// SCREEN
//
///////////////////////////////////

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
    
    const float w  = mMaxX - mMinX;
    const float h  = mMaxY - mMinY;
    const float sw = w/24;
    const float sh = 75.f;
    // melody sliders
    {
        ofColor color;
        color.setHsb(200, 255, 255);
        float x = sw * 0.75f;
        float y = h * 0.2f;
        
        for ( int i = 0; i < 16; ++i )
        {
            mSliders.push_back( ValueSlider( x, y,         sw, sh, color, &Settings::MelodyProbablities[i] ) );
            mSliders.push_back( ValueSlider( x, y+sh*1.5f, sw, sh, color, &Settings::BassProbabilities[i] ) );
            x += sw*1.5f;
        }
    }
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
        float scale = 1.0f;
        if ( mState == ST_SHOWING )
        {
            scale -= mAnimTimer / sk_AnimLength;
        }
        else if ( mState == ST_HIDING )
        {
            scale = mAnimTimer / sk_AnimLength;
        }
        
        ofPushMatrix();
        {
            ofTranslate( ofGetWidth()/2, ofGetHeight()/2 );
            ofScale( scale, scale );
        
            const float w = mMaxX - mMinX;
            const float h = mMaxY - mMinY;
            // background
            {
                ofSetRectMode( OF_RECTMODE_CENTER );
                ofFill();
                
                ofSetColor( 10, 10, 10, 128 );
                ofRect( 4, 4, w, h );
                
                ofSetColor( 30, 30, 30 );
                ofRect( 0, 0, w, h );
            }
            
            ofTranslate( -w/2, -h/2 );
            
            // sliders
            for( int i = 0; i < mSliders.size(); ++i )
            {
                mSliders[i].draw();
            }
        }
        ofPopMatrix();
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
    // outside the background?
    if ( touch.x < mMinX || touch.x > mMaxX || touch.y < mMinY || touch.y > mMaxY )
    {
        hide();
    }
    else // transform to local space and hand to our elements
    {
        const float x = ofMap(touch.x, mMinX, mMaxX, 0, mMaxX-mMinX);
        const float y = ofMap(touch.y, mMinY, mMaxY, 0, mMaxY-mMinY);
        
        for( int i = 0; i < mSliders.size(); ++i )
        {
            if ( mSliders[i].handleTouch(x, y) )
            {
                return;
            }
        }
    }
}

//----------------------------------
void SettingsScreen::touchMoved( ofTouchEventArgs& touch )
{
    // transform to local space and hand to our elements
    {
        const float x = ofMap(touch.x, mMinX, mMaxX, 0, mMaxX-mMinX);
        const float y = ofMap(touch.y, mMinY, mMaxY, 0, mMaxY-mMinY);
        
        for( int i = 0; i < mSliders.size(); ++i )
        {
            if ( mSliders[i].handleTouch(x, y) )
            {
                return;
            }
        }
    }   
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