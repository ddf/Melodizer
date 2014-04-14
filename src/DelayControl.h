//
//  DelayControl.h
//  melodizer
//
//  Created by Damien Di Fede on 4/14/14.
//
//

#ifndef __melodizer__DelayControl__
#define __melodizer__DelayControl__

#include "XYControl.h"
#include "Delay.h"
#include "Line.h"

// control flanger parameters with 3 touches
class DelayControl : public XYControl
{
public:
    DelayControl( Minim::Delay& flange );
    
    ~DelayControl()
    {
        mWetLine.unpatch( mDelay );
    }
    
protected:
    
    virtual void enable();
    virtual void disable();
    
private:
    
    Minim::Line         mWetLine;
    Minim::Delay&       mDelay;
};


#endif /* defined(__melodizer__DelayControl__) */
