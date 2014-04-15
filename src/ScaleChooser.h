//
//  ScaleChooser.h
//  melodizer
//
//  Created by Damien Di Fede on 12/23/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_ScaleChooser_h
#define melodizer_ScaleChooser_h

#include "Box.h"
#include "ofTrueTypeFont.h"
#include "Button.h"
#include <vector>
#include "Scales.h"

// object used to choose which scale to use for melody generation
class ScaleChooser
{
public:
    ScaleChooser();
    ~ScaleChooser();
    
    void setup( const float x, const float y, const float w, const float h );
    void update( const float dt );
    void draw();
    
    void open();
    void close();
    bool active();
    
    bool handleTouch( const float x, const float y );
    
    ofTrueTypeFont& getFont() { return mFont; }
    
private:
    
    Box                     mBox;
    ofTrueTypeFont          mFont;
    std::vector<Button*>    mButtons;
    float                   mAnim;
    float*                  mButtonFlyout;
    
    enum
    {
        ST_OPEN,
        ST_OPENING,
        ST_CLOSED,
        ST_CLOSING
    } mState;
};

#endif