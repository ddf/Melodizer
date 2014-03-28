//
//  FlangerControl.h
//  melodizer
//
//  Created by Damien Di Fede on 2/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_FlangerControl_h
#define melodizer_FlangerControl_h

#include "XYControl.h"
#include "Flanger.h"
#include "Line.h"

// control flanger parameters with 3 touches
class FlangerControl : public XYControl
{
public:
    FlangerControl( Minim::Flanger& flange );
    
    ~FlangerControl()
    {
        mWetLine.unpatch( mFlanger );
    }
    
private:
    
    virtual void enable();
    virtual void disable();
    
    Minim::Line         mWetLine;
    Minim::Flanger&     mFlanger;
};

#endif
