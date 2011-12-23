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

#endif
