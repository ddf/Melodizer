//
//  ValueKnob.h
//  melodizer
//
//  Created by Damien Di Fede on 4/16/14.
//
//

#ifndef __melodizer__ValueKnob__
#define __melodizer__ValueKnob__

#include "Box.h"
#include "ofMain.h"

class ValueKnob
{
public:
    ValueKnob( const char * label, ofPoint center, const float size, const float minValue, const float maxValue, float* value  );
    
    void draw( ofTrueTypeFont& font );
    
    Box& box() { return mBox; }
    
    // returns true if handled
    bool handleTouchDown( const int id, const float x, const float y );
    bool handleTouchMoved( const int id, const float x, const float y );
    bool handleTouchUp( const int id, const float x, const float y );
    
private:
    
    int     mTouch; // the touch we are currently tracking
    float   mTouchPos; // where the last x position of our touch was
    
    string      mLabel;
    Box         mBox;   // bounding box for touches
    float       mRadius; // visible radius
    
    float       mMin, mMax, mSensitivity;
    float *     mValue;
};

#endif /* defined(__melodizer__ValueKnob__) */
