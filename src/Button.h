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

// just a container for positions
struct Button
{
    Button( const char * name, const float x, const float y, const float w, const float h )
    : mName(name)
    , mBox( x, y, w, h )
    {
        
    }
    
    const char * mName;
    Box          mBox;
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
