//
//  ControlPoint.h
//  melodizer
//
//  Created by Damien Di Fede on 12/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_ControlPoint_h
#define melodizer_ControlPoint_h

#include "ofEvents.h"
#include "Button.h"

struct ValueMapper
{
    float * value;
    float   inputMin, inputMax;
    float   outputMin, outputMax;
    
            ValueMapper();
            ValueMapper( float * v, float imin, float imax, float omin, float omax );
    
    void    map( const float input );
};

class XYControl
{
public:
    XYControl();
    
    void setup( const char * label, const float cx, const float cy, const float dim );
    void draw();
    
    void setPosition( const float cx, const float cy );
    void setXValue( float * value, const float valueMin, const float valueMax );
    void setYValue( float * value, const float valueMin, const float valueMax );
    
    // all return true if the touch was handled.
	bool touchDown(ofTouchEventArgs &touch);
	bool touchMoved(ofTouchEventArgs &touch);
	bool touchUp(ofTouchEventArgs &touch);
    
private:
    
    virtual void enable() {}
    virtual void disable() {}
    
    void mapInput( float x, float y );
    
    int      mTouchID;
    bool     mEnabled;
    
    string         mLabel;
    Button         mButtonPower;
    Box            mControlBox;
    ofPoint        mControlPoint;
    ValueMapper    mXControl;
    ValueMapper    mYControl;
};

#endif
