//
//  KeyChooser.h
//  melodizer
//
//  Created by Damien Di Fede on 12/14/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_KeyChooser_h
#define melodizer_KeyChooser_h

#include "ofMain.h"
#include "Box.h"
#include "Button.h"

// one letter in the key chooser, just a container for positions
struct KeyButton : public Button
{
    KeyButton( const char * name, const float ox, const float oy )
    : Button( name, 0, 0, 70, 70 )
    , mOpenX(ox)
    , mOpenY(oy)
    {
        
    }
    
    const float  mOpenX, mOpenY;
};

// UI for setting the key the app generates music in
// looks like the circle of fifths with open
class KeyChooser
{
public:
    KeyChooser();
    ~KeyChooser();
    
    void setup( const float cx, const float cy, const float radius );
    void update( const float dt );
    void draw();
    
    void open();
    void close();
    bool active();
    
    bool handleTouch( const float x, const float y );
    
private:
    
    void drawButton( KeyButton* button, float alpha );
    
    float               mX, mY, mR;
    KeyButton*          mKeys[12];
    ofTrueTypeFont      mFont;
    
    enum
    {
        ST_OPENING,
        ST_OPEN,
        ST_CLOSING,
        ST_CLOSED,
    } mState;
    
    float               mAnim;
};

#endif
