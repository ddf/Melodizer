//
//  ValueKnob.cpp
//  melodizer
//
//  Created by Damien Di Fede on 4/16/14.
//
//

#include "ValueKnob.h"

ValueKnob::ValueKnob( const char * label, ofPoint center, const float size, const float minValue, const float maxValue, float* value  )
: mTouch(-1)
, mTouchPos(0)
, mLabel(label)
, mBox(center.x, center.y, size*1.2f, size*1.2f)
, mRadius(size/2)
, mMin(minValue)
, mMax(maxValue)
, mSensitivity((maxValue-minValue)*0.01f) // how much does the value change when we move one pixel
, mValue(value)
{
    
}


//----------------------------
void ValueKnob::draw(ofTrueTypeFont& font)
{
    ofSetLineWidth(1);
    if ( mTouch == -1 )
    {
        ofSetColor(128);
    }
    else
    {
        ofSetColor(255);
    }
    ofNoFill();
    
    ofRectangle textRect = font.getStringBoundingBox(mLabel, mBox.mMinX, mBox.mMinY);
    font.drawString(mLabel, mBox.mX - textRect.width/2, mBox.mMinY);
    
    ofCircle(mBox.mX, mBox.mY, mRadius);
    
    ofPushMatrix();
    {
        const float angle = ofMap(*mValue, mMin, mMax, 60, 300);
        ofTranslate(mBox.mX, mBox.mY);
        ofRotate(angle);
        ofLine(0, mRadius*0.2f, 0, mRadius);
    }
    ofPopMatrix();
}

//----------------------------------
bool ValueKnob::handleTouchDown(const int id, const float x, const float y)
{
    // if inside our box, start tracking
    const bool bInside = mBox.contains( x, y );
    if ( bInside && mTouch == -1 )
    {
        //printf( "Value Knob started tracking %d.\n", id );
        mTouch = id;
        mTouchPos = x;
    }
    
    return bInside;
}

bool ValueKnob::handleTouchMoved(const int id, const float x, const float y)
{
    bool bTracked = (id == mTouch);
    
    if ( bTracked )
    {
        const float dist = x - mTouchPos;
        *mValue = ofClamp(*mValue + dist*mSensitivity, mMin, mMax);
        
        mTouchPos = x;
    }
    
    return bTracked;
}

//----------------------------------
bool ValueKnob::handleTouchUp( const int id, const float x, const float y )
{
    if ( id == mTouch )
    {
        //printf( "Value Knob stopped tracking %d.\n", id );
        mTouch = -1;
        return true;
    }
    
    return false;
}