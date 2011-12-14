//
//  SettingsScreen.h
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_SettingsScreen_h
#define melodizer_SettingsScreen_h

#include "ofMain.h"
#include <vector>
#include "Settings.h"

// forward declair.
namespace Minim 
{
    class Waveform;
}

struct Box
{
    Box( float cx, float cy, float w, float h )
    : mX(cx)
    , mY(cy)
    , mW(w)
    , mH(h)
    , mMinX( mX - mW/2 )
    , mMaxX( mX + mW/2 )
    , mMinY( mY - mH/2 )
    , mMaxY( mY + mH/2 )
    {
        
    }
    
    float   mX, mY, mW, mH;
    float   mMinX, mMaxX, mMinY, mMaxY;
    
    bool contains( float x, float y )
    {
        return (x > mMinX && x < mMaxX && y > mMinY && y < mMaxY);
    }
};

class ValueSlider
{
public:
    ValueSlider( float x, float y, float w, float h, int hue, float* v ) 
    : mBox( x, y, w, h )
    , mHue(hue)
    , mValue(v)
    , mTouch(-1)
    {
    }
    
    void draw();
    
    Box& box() { return mBox; }
    
    // returns true if handled
    bool handleTouch( const int id, const float x, const float y );
    bool handleTouchUp( const int id, const float x, const float y );
    
private:
    
    int     mHue;
    Box     mBox;
    float*  mValue;
    int     mTouch; // the touch we are currently tracking
};

class WaveformButton
{
public:
    WaveformButton( float x, float y, float w, float h, WaveformType myType, WaveformType* refType, Minim::Waveform* wave )
    : mBox( x, y, w, h )
    , mWave(wave)
    , mTouch(-1)
    , mType( myType )
    , mRefType( refType )
    {
        
    }
    
    void draw( float anim );
    
    bool handleTouch( const int id, const float x, const float y );
    bool handleTouchUp( const int id, const float x, const float y );
    
private:
    
    int                 mTouch;
    Box                 mBox;
    WaveformType        mType;
    WaveformType*       mRefType;
    Minim::Waveform*    mWave;
    
};

// modal, non-apple UI for futzing with settings
class SettingsScreen 
{
public:
    SettingsScreen();
    ~SettingsScreen();
    
    void setup();
    void update( const float dt );
    void draw();
    
    void show();
    void hide();
    bool active() const;
    
    void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	void touchCancelled(ofTouchEventArgs &touch);
    
private:
    
    enum
    {
        ST_SHOWING, // animate in
        ST_SHOWN,
        ST_HIDING,
        ST_HIDDEN
    } mState;
    
    float mAnimTimer;
    float mMinX, mMaxX, mMinY, mMaxY;
    
    std::vector<ValueSlider>    mSliders;
    std::vector<WaveformButton> mWaveformButtons;
    
    Minim::Waveform* mWaveforms[WT_Count];
    float            mWaveformAnim;
    
    ofImage          mTrebleClef;
    ofImage          mBassClef;
};


#endif