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
    if ( mValue )
    {
        const float dval = ((*mValue) - mOutStart) / mOutRange;
        if ( dval > 0 )
        {        
            // adjust brightness based on amt of fill
            const float bright = ( 64 + 190 * dval );
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
            
            if ( mBox.mH > mBox.mW )
            {
                const float fH = mBox.mH * dval;
                ofRect( mBox.mX, mBox.mMaxY - fH/2, mBox.mW, fH);
            }
            else
            {
                const float fW = mBox.mW * dval;
                ofRect( mBox.mMinX + fW/2, mBox.mY, fW, mBox.mH );
            }
        }
    }
}

//----------------------------------
bool ValueSlider::handleTouch(const int id, const float x, const float y)
{
    // if inside our box, start tracking
    const bool bInside = mBox.contains( x, y ); 
    if ( bInside && mTouch == -1 )
    {
        //printf( "Value Slider started tracking %d.\n", id );
        mTouch = id;
    }
    
    bool bTracked = (id == mTouch);
    
    if ( bTracked )
    {
        if ( mBox.mH > mBox.mW )
        {
            *mValue = mOutStart + mOutRange * ofClamp( ( mBox.mMaxY - y) / mBox.mH, 0, 1 );
        }
        else
        {
            *mValue = mOutStart + mOutRange * ofClamp( (x - mBox.mMinX) / mBox.mW, 0, 1 );
        }
        
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
        //printf( "Value Slider stopped tracking %d.\n", id );
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
        const int res = 1;
        for( int x = 0; x < mBox.mW-res; x+=res )
        {
            const float animX = (int)(x + mBox.mW*anim) % (int)(mBox.mW);
            const float lu1   = animX / mBox.mW;
            const float lu2   = (animX+res) / mBox.mW;
            const float y1    = mBox.mY - mWave->value( lu1 )*mBox.mH*0.3f;
            const float y2    = mBox.mY - mWave->value( lu2 )*mBox.mH*0.3f;
            ofLine( mBox.mMinX + x, y1, mBox.mMinX + x+res, y2);
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
: mState(ST_SHOWN)
, mAnimTimer(0)
, mWaveformAnim(0)
, mButtonFx("FX",0,0,0,0)
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
    
    mMinX = 0;
    mMaxX = ofGetWidth();
    mMinY = 0;
    mMaxY = ofGetHeight();
    
    const float w  = mMaxX - mMinX;
    const float h  = mMaxY - mMinY;
    const float hs = h/768.0f;
    const float ws = w/1024.0f;
    const float sw = 35.f * ws;
    const float sh = 150.f * hs;
    const float waveformButtonDim = 75.0f * ws;
    
    const float melY = mMinY + sh/2 + waveformButtonDim + 40*hs;
    const float basY = melY  + sh   + waveformButtonDim + 60*hs;
    
    mToggleFont.loadFont("HelveticaBold.ttf", 30.0f*hs);
    
    // sliders for melody and bass control
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
        const float bw = waveformButtonDim;
        const float bh = waveformButtonDim;
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
    mTrebleClef.setAnchorPercent(0.5f, 0.5f);
    
    mBassClef.loadImage( "bass_clef.png" );
    mBassClef.setAnchorPercent(0.5f, 0.5f);
    
    // key chooser and scale chooser
    {
        const float kcd = 100 * hs;
        const float kcr = 600 * hs;
        const float kcx = mMinX + kcd/2 + 15*hs;
        const float kcy = mMaxY - kcd/2 - 15*hs;
        mKeyChooser.setup( kcx, kcy, kcd, kcr );

        const float scw = 300*ws;
        const float sch = kcd;
        const float scx = kcx + kcd/2 + 10*ws + scw/2;
        const float scy = kcy;
        mScaleChooser.setup( scx, scy, scw, sch );
        
        // shuffle control
        const float shw = 200*ws;
        const float shh = sch;
        const float shx = scx + scw/2 + shw/2 + 10*ws;
        const float shy = mMaxY - shh/2 - 15*hs;
        
        mSliders.push_back( ValueSlider( shx, shy, shw, shh, -1, &Settings::Shuffle ) );
        
        const float tw = 200*ws;
        const float th = shh;
        const float tx = shx + shw/2 + tw/2 + 20*ws;
        const float ty = shy;
        ValueSlider volSlider( tx, ty, tw, th, -1, &Settings::Tempo );
        volSlider.setRange(40, 160);
        mSliders.push_back(volSlider);
        
        const float fxd = kcd;
        const float fxx = mMaxX - fxd/2 - 15*hs;
        const float fxy = kcy;
        mButtonFx.setBox(fxx, fxy, fxd, fxd);
        mButtonFx.setText("FX", mScaleChooser.getFont());
    }
    
    // toggles for drum parts
    if ( 0 )
    {
        const int   dimX = 100;
        const int   dimY = 60;
        const float th  = h - dimY*0.75f;
        mToggles.push_back( Toggle(&Settings::PlayKick,  "Kick",  w/2 - 120, th, dimX, dimY) );
        mToggles.push_back( Toggle(&Settings::PlaySnare, "Snare", w/2,       th, dimX, dimY) );
        mToggles.push_back( Toggle(&Settings::PlayHat,   "Hat",   w/2 + 120, th, dimX, dimY) );
    }
    
    if ( mState == ST_SHOWN )
    {
        ofRegisterTouchEvents(this);
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
    
    mWaveformAnim += dt;
    while ( mWaveformAnim > 1 )
    {
        mWaveformAnim -= 1;
    }
    
    mKeyChooser.update( dt );
    mScaleChooser.update( dt );
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
        float voff = 0.f;
        if ( mState == ST_SHOWING )
        {
            voff = mAnimTimer / sk_AnimLength;
        }
        else if ( mState == ST_HIDING )
        {
            voff = 1 - mAnimTimer / sk_AnimLength;
        }
        
        ofPushMatrix();
        {
            ofTranslate( ofGetWidth()/2, ofGetHeight()/2 + voff*ofGetHeight() );
        
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
            
            // clefs
            {
                const int tint = 50;
                ofSetColor(tint, tint, tint);
                
                ofSetRectMode( OF_RECTMODE_CORNER );
                const float scale = (ofGetWidth()/1024.0f);
                
                const float tx = mSliders[0].box().mX - mSliders[0].box().mW;
                const float ty = mSliders[0].box().mY;
                mTrebleClef.draw( tx, ty, mTrebleClef.width*scale, mTrebleClef.height*scale );
                
                const float bx = mSliders[1].box().mX;
                const float by = mSliders[1].box().mY;
                mBassClef.draw  ( bx, by, mBassClef.width*scale, mBassClef.height*scale );
            }
            
            ofSetRectMode( OF_RECTMODE_CENTER );
            
            // sliders
            for( int i = 0; i < mSliders.size(); ++i )
            {
                mSliders[i].draw();
            }
            
            // jenky label drawing for swing slider
            {
                Box& swingBox = mSliders[mSliders.size()-2].box();
                ofTrueTypeFont& font = mScaleChooser.getFont();
                
                ofSetColor(0, 0, 0, 255);
                const float th = font.getLineHeight();
                font.drawString("Swing", swingBox.mMinX + 10.f*(ofGetHeight()/768.f), swingBox.mMinY + th);
            }
            
            // jenky label drawing for tempo slider
            {
                Box& tempoBox = mSliders[mSliders.size()-1].box();
                ofTrueTypeFont& font = mScaleChooser.getFont();
                
                ofSetColor(0, 0, 0, 255);
                const float th = font.getLineHeight();
                font.drawString("Tempo", tempoBox.mMinX + 10.f*(ofGetHeight()/768.f), tempoBox.mMinY + th);
            }
            
            {
                mButtonFx.draw(mScaleChooser.getFont(), ofColor(255,128,0), ofColor(200,200,200));
            }
            
            // waveform buttons
            for( int i = 0; i < mWaveformButtons.size(); ++i )
            {
                mWaveformButtons[i].draw( mWaveformAnim );
            }
            
            // toggles
            for( int i = 0; i < mToggles.size(); ++i )
            {
                Toggle& t = mToggles[i];
                if ( t.on() )
                {
                    ofSetColor(0, 92, 200);
                }
                else
                {
                    ofSetColor( 60, 60, 60 );
                }
                
                ofFill();
                ofRect( t.mBox.mX, t.mBox.mY, t.mBox.mW, t.mBox.mH );
                
                ofSetColor(200, 200, 200);
                const string    name = t.mName;
                const float     yoff = mToggleFont.getLineHeight() * 0.35f;
                const float     xoff = mToggleFont.stringWidth( name ) * -0.5f;
                mToggleFont.drawString( name, t.mBox.mX + xoff, t.mBox.mY + yoff );
            }
            
            if ( mScaleChooser.active() )
            {
                mKeyChooser.draw();
                mScaleChooser.draw();
            }
            else
            {
                mScaleChooser.draw();
                mKeyChooser.draw();
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
    
    if ( mKeyChooser.active() )
    {
        mKeyChooser.close();
    }
    
    if ( mScaleChooser.active() )
    {
        mScaleChooser.close();
    }
    
    mAnimTimer = sk_AnimLength;
    mState = ST_HIDING;
    
}

//----------------------------------
void SettingsScreen::touchDown( ofTouchEventArgs& touch )
{
//    printf( "Touch Down: %d, %f, %f\n", touch.id, touch.x, touch.y );
    {
        const float x = ofMap(touch.x, mMinX, mMaxX, 0, mMaxX-mMinX);
        const float y = ofMap(touch.y, mMinY, mMaxY, 0, mMaxY-mMinY);
        
        if ( !mScaleChooser.active() && mKeyChooser.handleTouch(x, y) )
        {
            return;
        }
        
        if ( !mKeyChooser.active() && mScaleChooser.handleTouch(x, y) )
        {
            return;
        }
        
        for( int i = 0; i < mToggles.size(); ++i )
        {
            if ( mToggles[i].handleTouch(x, y) )
            {
                return;
            }
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
    if ( mKeyChooser.active() || mScaleChooser.active() )
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
    if ( mKeyChooser.active() || mScaleChooser.active() )
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
        
        if ( mButtonFx.mBox.contains(x, y) )
        {
            hide();
        }
    }
}

//----------------------------------
void SettingsScreen::touchCancelled( ofTouchEventArgs& touch )
{
    
}