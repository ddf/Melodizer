//
//  SampleRepeatControl.h
//  melodizer
//
//  Created by Damien Di Fede on 12/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_SampleRepeatControl_h
#define melodizer_SampleRepeatControl_h

#include "ofEvents.h"
#include "SampleRepeat.h"

class SampleRepeatControl
{
public:
    SampleRepeatControl( SampleRepeat& repeater )
    : mRepeater( repeater )
    {
        mTouch1.id = -1;
        mTouch2.id = -1;
    }
    
    bool    active() const;
    
    ofPoint touchPosition1() const;
    ofPoint touchPosition2() const;
    
    // all return true if the touch was handled.
	bool touchDown(ofTouchEventArgs &touch);
	bool touchMoved(ofTouchEventArgs &touch);
	bool touchUp(ofTouchEventArgs &touch);
    
private:
    
    void setSection();
    
    SampleRepeat&       mRepeater;
    ofTouchEventArgs    mTouch1;
    ofTouchEventArgs    mTouch2;
    
    
};

#endif
