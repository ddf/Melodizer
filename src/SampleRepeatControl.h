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
#include "XYControl.h"

class SampleRepeatControl : public XYControl
{
public:
    SampleRepeatControl( SampleRepeat& repeater );
    virtual ~SampleRepeatControl();
    
protected:
    
    virtual void enable();
    virtual void disable();
    virtual void inputChanged();
    
private:
    
    SampleRepeat&       mRepeater;
    float               mRepeatStart;
    float               mRepeatEnd;
    
    
};

#endif
