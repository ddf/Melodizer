//
//  Button.h
//  melodizer
//
//  Created by Damien Di Fede on 12/23/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_Button_h
#define melodizer_Button_h

#include "Box.h"
#include "ofMain.h"
#include "ofTrueTypeFont.h"

// just a container for positions
struct Button
{
    Button( const char * name, const float x, const float y, const float w, const float h )
    : mName(name)
    , mBox( x, y, w, h )
    {
        
    }
    
    void draw( ofTrueTypeFont& font, ofColor boxColor, ofColor textColor )
    {
        ofSetColor(boxColor);
        ofRect(mBox.mX, mBox.mY, mBox.mW, mBox.mH);
        
        ofSetColor(200, 200, 200);
        font.drawString(mName, mTextRect.getMinX(), mTextRect.getMaxY());
    }
    
    void setBox( const float x, const float y, const float w, const float h )
    {
        mBox.mW = w;
        mBox.mH = h;
        mBox.setCenter( x, y );
    }
    
    void setText( const char * text, ofTrueTypeFont& usingFont )
    {
        mName = text;
        mTextRect = usingFont.getStringBoundingBox(mName, mBox.mX, mBox.mY);
        mTextRect.setFromCenter(mBox.mX, mBox.mY, mTextRect.getWidth(), mTextRect.getHeight());
    }
    
    string       mName;
    Box          mBox;
    ofRectangle  mTextRect;
};

struct Toggle : public Button
{
    Toggle( bool* bValue, const char * name, const float x, const float y, const float w, const float h ) 
    : Button( name, x, y, w, h )
    , mbValue( bValue )
    {
        
    }
    
    bool on() const { return *mbValue; }
    
    bool handleTouch( const float x, const float y )
    {
        if ( mBox.contains( x, y ) )
        {
            *mbValue = !(*mbValue);
            return true;
        }
        
        return false;
    }

private:
    
    bool* mbValue;
};

#endif
