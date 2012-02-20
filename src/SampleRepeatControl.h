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
#include "MultitouchControl.h"

class SampleRepeatControl : public MultitouchControl
{
public:
    SampleRepeatControl( SampleRepeat& repeater )
    : MultitouchControl( 2 )
    , mRepeater( repeater )
    {
    }
    
private:
    
    virtual void activate();
    virtual void deactivate();
    virtual void setParameters();
    
    SampleRepeat&       mRepeater;
    
    
};

#endif
