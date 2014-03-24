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
    
    void setup();
    
    const ofPoint& position() const { return mP; }
    float          radius()   const { return mR; }
    
    void setPosition( const float x, const float y );
    
    void setXControl( ValueMapper mapper );
    void setYControl( ValueMapper mapper );
    
    // all return true if the touch was handled.
	bool touchDown(ofTouchEventArgs &touch);
	bool touchMoved(ofTouchEventArgs &touch);
	bool touchUp(ofTouchEventArgs &touch);
    
private:
    
    int      mTouchID;
    ofPoint  mLastTouch;
    ofPoint  mP; // position
    float    mR; // radius
    
    ValueMapper    mXControl;
    ValueMapper    mYControl;
};

#endif
