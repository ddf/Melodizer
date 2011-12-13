//
//  SettingsScreen.h
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_SettingsScreen_h
#define melodizer_SettingsScreen_h

#include "ofEvents.h"

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
};


#endif
