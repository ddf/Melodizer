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

class ValueSlider
{
public:
    ValueSlider( float x, float y, float w, float h, ofColor c, float* v ) 
    : mX(x)
    , mY(y)
    , mW(w)
    , mH(h)
    , mMinX( mX - mW/2 )
    , mMaxX( mX + mW/2 )
    , mMinY( mY - mH/2 )
    , mMaxY( mY + mH/2 )
    , mColor(c)
    , mValue(v) 
    {
    }
    
    void draw();
    
    // returns true if handled
    bool handleTouch( const float x, const float y );
    
private:
    
    ofColor mColor;
    float   mX, mY, mW, mH;
    float   mMinX, mMaxX, mMinY, mMaxY;
    float*  mValue;
};

// modal, non-apple UI for futzing with settings
class SettingsScreen 
{
public:
    SettingsScreen();
    
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
    
    std::vector<ValueSlider> mSliders;
};


#endif
