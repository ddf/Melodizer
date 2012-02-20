//
//  FlangerControl.h
//  melodizer
//
//  Created by Damien Di Fede on 2/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_FlangerControl_h
#define melodizer_FlangerControl_h

#include "MultitouchControl.h"
#include "Flanger.h"
#include "Line.h"

// control flanger parameters with 3 touches
class FlangerControl : public MultitouchControl
{
public:
    FlangerControl( Minim::Flanger& flange )
    : MultitouchControl(3)
    , mFlanger( flange )
    , mWetLine( 0, 0, 0 )
    {
        mWetLine.patch( mFlanger.wet );
    }
    
    ~FlangerControl()
    {
        mWetLine.unpatch( mFlanger );
    }
    
private:
    
    virtual void activate();
    virtual void deactivate();
    virtual void setParameters();
    
    Minim::Line         mWetLine;
    Minim::Flanger&     mFlanger;
};

#endif
