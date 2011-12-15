//
//  SettingsScreen.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "SettingsScreen.h"
#include "Waves.h"

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
        ofRect( mBox.mX, mBox.mY, mBox.mW, mBox.mH );
    }
    
    // fill
    if ( mValue > 0 )
    {        
        // adjust brightness based on amt of fill
        float bright = ( 64 + 190 * (*mValue) );
        ofColor color;
        
        if ( mHue >= 0 )
        {
            color.setHsb( mHue, 255, bright );
        }
        else
        {
            color.set( bright, bright, bright );
        }
        
        ofSetColor(color);
        
        float fH = mBox.mH * (*mValue);
        
        ofRect( mBox.mX, mBox.mMaxY - fH/2, mBox.mW, fH);
    }
}

//----------------------------------
bool ValueSlider::handleTouch(const int id, const float x, const float y)
{
    // if inside our box, start tracking
    const bool bInside = mBox.contains( x, y ); 
    if ( bInside && mTouch == -1 )
    {
        printf( "Value Slider started tracking %d.\n", id );
        mTouch = id;
    }
    
    bool bTracked = (id == mTouch);
    
    if ( bTracked )
    {
        *mValue = ofClamp( ( mBox.mMaxY - y) / mBox.mH, 0, 1 );
        
        if ( !bInside )
        {
            mTouch = -1;
            bTracked = false;
        }
    }
    
    return bTracked;
}

//----------------------------------
bool ValueSlider::handleTouchUp( const int id, const float x, const float y )
{
    if ( id == mTouch )
    {
        printf( "Value Slider stopped tracking %d.\n", id );
        mTouch = -1;
        return true;
    }
    
    return false;
}

////////////////////////////////////
//
// WAVEFORM BUTTON
//
////////////////////////////////////
void WaveformButton::draw( float anim )
{
    if ( *mRefType == mType )
    {
        ofSetColor(192, 192, 192);
    }
    else
    {
        ofSetColor(128, 128, 128);
        // ignore anim
        anim = 0;
    }
    
    // first draw the waveform in our box (x,y) is center
    {
        for( int x = 0; x < mBox.mW-1; ++x )
        {
            const float animX = (int)(x + mBox.mW*anim) % (int)(mBox.mW);
            const float lu1   = animX / mBox.mW;
            const float lu2   = (animX+1) / mBox.mW;
            const float y1    = mBox.mY - mWave->value( lu1 )*mBox.mH*0.3f;
            const float y2    = mBox.mY - mWave->value( lu2 )*mBox.mH*0.3f;
            ofLine( mBox.mMinX + x, y1, mBox.mMinX + x+1, y2);
        }
    }
    
    // now draw the box
    {
        ofSetRectMode( OF_RECTMODE_CENTER );
        ofNoFill();
        ofRect( mBox.mX, mBox.mY, mBox.mW, mBox.mH );
    }
}

//----------------------------------
bool WaveformButton::handleTouch( const int id, const float x, const float y )
{
    if ( mBox.contains( x, y ) )
    {
        *mRefType = mType;
        return true;
    }
    
    return false;
}

//----------------------------------
bool WaveformButton::handleTouchUp( const int id, const float x, const float y )
{
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
, mWaveformAnim(0)
{
}

SettingsScreen::~SettingsScreen()
{
    for( int i = 0; i < WT_Count; ++i )
    {
        delete mWaveforms[i];
    }
}

//----------------------------------
void SettingsScreen::setup()
{
    mMinX = ofGetWidth() * 0.05f;
    mMaxX = ofGetWidth() - mMinX;
    mMinY = ofGetHeight() * 0.1f;
    mMaxY = ofGetHeight() - mMinY;
    
    const float w  = mMaxX - mMinX;
    const float h  = mMaxY - mMinY;
    const float melY = h * 0.23f;
    const float basY = h * 0.78f;
    const float sw = 35.f;
    const float sh = 75.f;
    
    // sliders
    {
        
        float x  = (w - (sw*1.5f*17)) / 2 + sw*0.5f;
        
        // -1 for hue means white for me
        mSliders.push_back( ValueSlider( x, melY, sw, sh, -1, &Settings::MelodyVolume ) );
        mSliders.push_back( ValueSlider( x, basY, sw, sh, -1, &Settings::BassVolume ) );
        
        x += sw * 1.75f;
        
        for ( int i = 0; i < 16; ++i )
        {
            mSliders.push_back( ValueSlider( x, melY, sw, sh, 200, &Settings::MelodyProbablities[i] ) );
            mSliders.push_back( ValueSlider( x, basY, sw, sh, 150, &Settings::BassProbabilities[i] ) );
            x += sw*1.5f;
        }
    }
    
    // generate waveforms so we can drawr them
    {
        mWaveforms[WT_Sine]         = Minim::Waves::SINE();
        mWaveforms[WT_Triangle]     = Minim::Waves::TRIANGLE();
        mWaveforms[WT_Saw]          = Minim::Waves::SAW();
        mWaveforms[WT_Square]       = Minim::Waves::SQUARE();
        mWaveforms[WT_Quarterpulse] = Minim::Waves::QUARTERPULSE();
        mWaveforms[WT_Sine4]        = Minim::Waves::randomNHarms(4);
        mWaveforms[WT_Sine8]        = Minim::Waves::randomNHarms(8);
        mWaveforms[WT_Sine16]       = Minim::Waves::randomNHarms(16);
        mWaveforms[WT_Sine32]       = Minim::Waves::randomNHarms(32);
    }
    
    // make waveform buttons
    {
        const float bw = 60.f;
        const float bh = 60.f;
        const float my = melY - sh/2 - bh/2 - 10;
        const float by = basY - sh/2 - bh/2 - 10;
        
        float x = (w - (bw*1.5f*WT_Count)) / 2 + bw*0.75f;
        
        for( int i = 0; i < WT_Count; ++i )
        {
            mWaveformButtons.push_back( WaveformButton( x, my, bw, bh, (WaveformType)i, &Settings::MelodyWave, mWaveforms[i] ) );
            mWaveformButtons.push_back( WaveformButton( x, by, bw, bh, (WaveformType)i, &Settings::BassWave,   mWaveforms[i] ) );
            x += bw * 1.5f;
        }
    }
    
    mTrebleClef.loadImage( "treble_clef.png" );
    mBassClef.loadImage( "bass_clef.png" );
    
    // key chooser
    mKeyChooser.setup( w/2, h/2 - 30, 200 );
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
    
    mWaveformAnim += dt;
    while ( mWaveformAnim > 1 )
    {
        mWaveformAnim -= 1;
    }
    
    mKeyChooser.update( dt );
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
                
                // drop shadow
                ofSetColor( 10, 10, 10, 128 );
                ofRect( 4, 4, w, h );
                
                // box
                ofSetColor( 30, 30, 30 );
                ofRect( 0, 0, w, h );
            }
            
            // translate to top corner so elements can render in the correct space
            ofTranslate( -w/2, -h/2 );
            
            const int tint = 50;
            ofSetColor(tint, tint, tint);
            mTrebleClef.draw( mSliders[2].box().mX - mSliders[2].box().mW, mSliders[2].box().mY );
            mBassClef.draw  ( mSliders[3].box().mX, mSliders[3].box().mY );
            
            // sliders
            for( int i = 0; i < mSliders.size(); ++i )
            {
                mSliders[i].draw();
            }
            
            // waveform buttons
            for( int i = 0; i < mWaveformButtons.size(); ++i )
            {
                mWaveformButtons[i].draw( mWaveformAnim );
            }
            
            mKeyChooser.draw();
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
    
    if ( mKeyChooser.active() )
    {
        mKeyChooser.close();
    }
    
    mAnimTimer = sk_AnimLength;
    mState = ST_HIDING;
    
}

//----------------------------------
void SettingsScreen::touchDown( ofTouchEventArgs& touch )
{
//    printf( "Touch Down: %d, %f, %f\n", touch.id, touch.x, touch.y );
    // outside the background?
    if ( touch.x < mMinX || touch.x > mMaxX || touch.y < mMinY || touch.y > mMaxY )
    {
        hide();
    }
    else // transform to local space and hand to our elements
    {
        const float x = ofMap(touch.x, mMinX, mMaxX, 0, mMaxX-mMinX);
        const float y = ofMap(touch.y, mMinY, mMaxY, 0, mMaxY-mMinY);
        
        if ( mKeyChooser.handleTouch(x, y) )
        {
            return;
        }
        
        for( int i = 0; i < mSliders.size(); ++i )
        {
            if ( mSliders[i].handleTouch(touch.id, x, y) )
            {
                return;
            }
        }
        
        for ( int i = 0; i < mWaveformButtons.size(); ++i )
        {
            if ( mWaveformButtons[i].handleTouch(touch.id, x, y) )
            {
                return;
            }
        }
    }
}

//----------------------------------
void SettingsScreen::touchMoved( ofTouchEventArgs& touch )
{
    if ( mKeyChooser.active() )
    {
        return;
    }
    
//    printf( "Touch Moved: %d, %f, %f\n", touch.id, touch.x, touch.y );
    // transform to local space and hand to our elements
    {
        const float x = ofMap(touch.x, mMinX, mMaxX, 0, mMaxX-mMinX);
        const float y = ofMap(touch.y, mMinY, mMaxY, 0, mMaxY-mMinY);
        
        for( int i = 0; i < mSliders.size(); ++i )
        {
            if ( mSliders[i].handleTouch(touch.id, x, y) )
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
    if ( mKeyChooser.active() )
    {
        return;
    }
    
//    printf( "Touch Up: %d, %f, %f\n", touch.id, touch.x, touch.y );
    // transform to local space and hand to our elements
    {
        const float x = ofMap(touch.x, mMinX, mMaxX, 0, mMaxX-mMinX);
        const float y = ofMap(touch.y, mMinY, mMaxY, 0, mMaxY-mMinY);
        
        for( int i = 0; i < mSliders.size(); ++i )
        {
            if ( mSliders[i].handleTouchUp( touch.id, x, y ) )
            {
                return;
            }
        }
    }
}

//----------------------------------
void SettingsScreen::touchCancelled( ofTouchEventArgs& touch )
{
    
}