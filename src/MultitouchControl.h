//
//  MultitouchControl.h
//  melodizer
//
//  Created by Damien Di Fede on 2/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_MultitouchControl_h
#define melodizer_MultitouchControl_h

#include "ofEvents.h"
#include <vector>

// base class for doing multi-touch control of audio parameters.
// handles tracking touches, and calling setParameters when it goes active.
// works by taking the number of touches required to activate the control
// and then only going active if that number of touches go down within a small timeframe
class MultitouchControl
{
public:
    MultitouchControl( const int requiredTouches )
    : mRequiredTouches( requiredTouches )
    , mTouchTimer( 0 )
    , mbActive( false )
    {
        
    }
    
    void    update( const float dt );
    bool    active() const { return mbActive; }
    
    ofPoint getTouch( const int tid ) const;
    
    // all return true if the touch was handled.
	bool touchDown(ofTouchEventArgs &touch);
	bool touchMoved(ofTouchEventArgs &touch);
	bool touchUp(ofTouchEventArgs &touch);
    
protected:
    
    // derived classes should do their work here
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void setParameters() = 0;
    
private:
    
    int                           mRequiredTouches;
    float                         mTouchTimer;
    bool                          mbActive;
    std::vector<ofTouchEventArgs> mTouches;
    
};

#endif
