//
//  SettingsScreen.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "SettingsScreen.h"
#include "Waves.h"
#include "Audio.h"

static const float sk_AnimLength = 0.25f;
static const float sk_noteAnimLength = 0.45f;

class Expo
{
public:
    static float easeIn (float t, float b, float c, float d)
    {
        return (t==0) ? b : c * powf(2, 10 * (t/d - 1)) + b;
    }
    
    static float easeOut (float t, float b, float c, float d)
    {
        return (t==d) ? b+c : c * (-powf(2, -10 * t/d) + 1) + b;
    }
    
    static float easeInOut (float t, float b, float c, float d)
    {
        if (t==0) return b;
        if (t==d) return b+c;
        if ((t/=d/2) < 1) return c/2 * powf(2, 10 * (t - 1)) + b;
        return c/2 * (-powf(2, -10 * --t) + 2) + b;
    }
};

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
WaveformButton::WaveformButton( float x, float y, float w, float h, WaveformType myType, WaveformType* refType, Minim::Waveform* wave )
: mBox( x+0.5f, y+0.5f, w, h )
, mWave(wave)
, mTouch(-1)
, mType( myType )
, mRefType( refType )
{
    for( int lx = mBox.mMinX; lx < mBox.mMinX+mBox.mW; ++lx )
    {
        mLine.addVertex(ofPoint(lx+0.5f,0));
    }
}

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
    
    ofSetLineWidth(1);
    
    // first draw the waveform in our box (x,y) is center
    {
        auto& verts = mLine.getVertices();
        for( int x = 0; x < verts.size(); ++x )
        {
            const float animX = (int)(x + mBox.mW*anim) % (int)(mBox.mW);
            const float lu1   = animX / mBox.mW;
            verts[x].y        = mBox.mY - mWave->value( lu1 )*mBox.mH*0.3f;
        }
        
        mLine.draw();
    }
    
    // now draw the box
    {
        ofSetRectMode( OF_RECTMODE_CORNER );
        ofNoFill();
        ofRect( mBox.mMinX, mBox.mMinY, mBox.mW, mBox.mH );
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
    
    mToggleFont.loadFont("HelveticaBold.ttf", 20.0f*hs);
    
    // sliders for melody and bass control
    {
        
        float x  = (w - (sw*1.5f*17)) / 2 + sw*0.5f;
        
        // -1 for hue means white for me
        mMelodySliders.push_back( ValueSlider( x, melY, sw, sh, -1, &Settings::MelodyVolume ) );
        mBassSliders.push_back( ValueSlider( x, basY, sw, sh, -1, &Settings::BassVolume ) );
        
        x += sw * 1.75f;
        
        for ( int i = 0; i < 16; ++i )
        {
            mMelodySliders.push_back( ValueSlider( x, melY, sw, sh, 200, &Settings::MelodyProbablities[i] ) );
            mBassSliders.push_back( ValueSlider( x, basY, sw, sh, 150, &Settings::BassProbabilities[i] ) );
            x += sw*1.5f;
        }
        
        mMelodyAnims.resize(16,0);
        mBassAnims.resize(16,0);
        
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
        
        // slider height here same as slider width for the probability sliders above
        const float sliderH = sw;
        
        // shuffle control
        const float shw = 300*ws;
        const float shh = sliderH;
        const float shx = scx + scw/2 + shw/2 + 10*ws;
        const float shy = 2000; // mMaxY - sch - 15*hs + shh/2;
        
        mOtherSliders.push_back( ValueSlider( shx, shy, shw, shh, -1, &Settings::Shuffle ) );
        
        const float tw = 300*ws;
        const float th = sliderH;
        const float tx = shx; // shx + shw/2 + tw/2 + 20*ws;
        const float ty = 2000; // shy + th + 15*hs;
        ValueSlider tempoSlider( tx, ty, tw, th, -1, &Settings::Tempo );
        tempoSlider.setRange(40, 160);
        mOtherSliders.push_back(tempoSlider);
        
        const float knobDim = kcd;
        
        // shuffle knob
        const float skx = scx + scw/2 + 50*ws + knobDim/2;
        const float sky = scy;
        mKnobs.push_back( ValueKnob("Swing", ofPoint(skx,sky), knobDim, 0, 1, &Settings::Shuffle) );
        
        // tempo knob
        const float tkx = skx + knobDim + 50*ws;
        const float tky = sky;
        mKnobs.push_back( ValueKnob("Tempo", ofPoint(tkx,tky), knobDim, 40, 160, &Settings::Tempo) );
        
        // decay knob
        const float dkx = tkx + knobDim + 50*ws;
        const float dky = sky;
        mKnobs.push_back( ValueKnob("Decay", ofPoint(dkx,dky), knobDim, 0.01f, 0.15f, &Settings::Duration) );
        
        const float fxd = kcd;
        const float fxx = mMaxX - fxd/2 - 15*hs;
        const float fxy = kcy;
        mButtonFx.setBox(fxx, fxy, fxd, fxd);
        mButtonFx.setText("FX", mScaleChooser.getFont());
    }
    
    if ( mState == ST_SHOWN )
    {
        ofRegisterTouchEvents(this);
    }
}

//----------------------------------
void SettingsScreen::toneBegan(Minim::Summer* onBus, const int onTick)
{
    if ( onBus == &Audio::Melody() )
    {
        mMelodyAnims[onTick] = sk_noteAnimLength;
    }
    else if ( onBus == &Audio::Bass() )
    {
        mBassAnims[onTick] = sk_noteAnimLength;
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
    
    for( int i = 0; i < 16; ++i )
    {
        mMelodyAnims[i] = ofClamp(mMelodyAnims[i]-dt, 0, 1);
        mBassAnims[i] = ofClamp(mBassAnims[i]-dt, 0, 1);
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
                
                const float tx = mMelodySliders[0].box().mX - mMelodySliders[0].box().mW;
                const float ty = mMelodySliders[0].box().mY;
                mTrebleClef.draw( tx, ty, mTrebleClef.width*scale, mTrebleClef.height*scale );
                
                const float bx = mBassSliders[0].box().mX;
                const float by = mBassSliders[0].box().mY;
                mBassClef.draw  ( bx, by, mBassClef.width*scale, mBassClef.height*scale );
            }
            
            ofSetRectMode( OF_RECTMODE_CENTER );
            
            // sliders
            for( int i = 0; i < mMelodySliders.size(); ++i )
            {
                auto& slider = mMelodySliders[i];
                slider.draw();
                
                const int tick = i-1;
                if ( tick >= 0 )
                {
                    const float alpha = Expo::easeIn(sk_noteAnimLength - mMelodyAnims[tick], 255, -255, sk_noteAnimLength);
                    ofSetColor(255, alpha);
                    ofSetLineWidth(4);
                    const float hwid = slider.box().mW*0.35;
                    const float posY = ofMap(mMelodyAnims[tick], 0, sk_noteAnimLength, slider.box().mMinY, slider.box().mMaxY);
                    ofLine(slider.box().mX-hwid, posY, slider.box().mX+hwid, posY);
                }
            }
            
            for( int i = 0; i < mBassSliders.size(); ++i )
            {
                auto& slider = mBassSliders[i];
                slider.draw();
                
                const int tick = i-1;
                if ( tick >= 0 )
                {
                    const float alpha = Expo::easeIn(sk_noteAnimLength - mBassAnims[tick], 255, -255, sk_noteAnimLength);
                    ofSetColor(255, alpha);
                    ofSetLineWidth(4);
                    const float hwid = slider.box().mW*0.35;
                    const float posY = ofMap(mBassAnims[tick], 0, sk_noteAnimLength, slider.box().mMinY, slider.box().mMaxY);
                    ofLine(slider.box().mX-hwid, posY, slider.box().mX+hwid, posY);
                }
            }
            
            for ( auto& slider : mOtherSliders )
            {
                slider.draw();
            }
            
            for ( auto& knob : mKnobs )
            {
                knob.draw(UIFont());
            }
            
            // jenky label drawing for swing slider
            if ( 0 )
            {
                Box& swingBox = mOtherSliders[0].box();
                ofTrueTypeFont& font = UIFont();
                
                ofSetColor(0, 0, 0, 255);
                const float th = font.getLineHeight();
                font.drawString("Swing", swingBox.mMinX + 10.f*(ofGetHeight()/768.f), swingBox.mMinY + th);
            }
            
            // jenky label drawing for tempo slider
            if ( 0 )
            {
                Box& tempoBox = mOtherSliders[1].box();
                ofTrueTypeFont& font = UIFont();
                
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
            
            // sequence position
            {
                const int sliderID = Audio::CurrentTick() + 1;
                const float screenScale = (ofGetHeight()/768.0f);
                const float tickW     = mMelodySliders[1].box().mW;
                const float tickH     = tickW/4.0f;
                
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofFill();
                ofSetColor(128);
                const float posX = mMelodySliders[sliderID].box().mX;
                const float posYMel = mMelodySliders[sliderID].box().mMaxY + 5*screenScale + tickH/2;
                const float posYBass = mBassSliders[sliderID].box().mMaxY + 5*screenScale + tickH/2;
                ofRect(posX, posYMel, tickW, tickH);
                ofRect(posX, posYBass, tickW, tickH);
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
        
        for( auto& slider : mMelodySliders )
        {
            if ( slider.handleTouch(touch.id, x, y) )
            {
                return;
            }
        }
        
        for( auto& slider : mBassSliders )
        {
            if ( slider.handleTouch(touch.id, x, y) )
            {
                return;
            }
        }
        
        for( auto& slider : mOtherSliders )
        {
            if ( slider.handleTouch(touch.id, x, y) )
            {
                return;
            }
        }
        
        for( auto& knob : mKnobs )
        {
            if ( knob.handleTouchDown(touch.id, x, y) )
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
        
        for( auto& slider : mMelodySliders )
        {
            if ( slider.handleTouch(touch.id, x, y) )
            {
                return;
            }
        }
        
        for( auto& slider : mBassSliders )
        {
            if ( slider.handleTouch(touch.id, x, y) )
            {
                return;
            }
        }
        
        for( auto& slider : mOtherSliders )
        {
            if ( slider.handleTouch(touch.id, x, y) )
            {
                return;
            }
        }
        
        for( auto& knob : mKnobs )
        {
            if ( knob.handleTouchMoved(touch.id, x, y) )
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
        
        for( auto& slider : mMelodySliders )
        {
            if ( slider.handleTouchUp(touch.id, x, y) )
            {
                return;
            }
        }
        
        for( auto& slider : mBassSliders )
        {
            if ( slider.handleTouchUp(touch.id, x, y) )
            {
                return;
            }
        }
        
        for( auto& slider : mOtherSliders )
        {
            if ( slider.handleTouchUp(touch.id, x, y) )
            {
                return;
            }
        }
        
        for( auto& knob : mKnobs )
        {
            if ( knob.handleTouchUp(touch.id, x, y) )
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
    const float x = ofMap(touch.x, mMinX, mMaxX, 0, mMaxX-mMinX);
    const float y = ofMap(touch.y, mMinY, mMaxY, 0, mMaxY-mMinY);
    
    for( auto& knob : mKnobs )
    {
        knob.handleTouchUp(touch.id, x, y);
    }
}