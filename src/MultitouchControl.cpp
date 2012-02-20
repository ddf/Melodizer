//
//  MultitouchControl.cpp
//  melodizer
//
//  Created by Damien Di Fede on 2/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MultitouchControl.h"
#include "ofUtils.h"

const float kActiveWindow = 0.1f;

//-----------------------------------
ofPoint MultitouchControl::getTouch(const int tid) const
{
    const ofTouchEventArgs& touch = mTouches[tid];
    return ofPoint(touch.x, touch.y);
}

//-----------------------------------
void MultitouchControl::update( const float dt )
{
    if ( mTouchTimer > 0 )
    {
        mTouchTimer -= dt;
        
        if ( mTouchTimer <= 0 )
        {
            if ( mTouches.size() == mRequiredTouches )
            {
                mbActive = true;
                setParameters();
                activate();
            }
            else
            {
                mTouches.clear();
            }
        }
    }
}

//-----------------------------------
bool MultitouchControl::touchDown( ofTouchEventArgs& touch )
{
    if ( !active() )
    {
        if ( mTouches.empty() )
        {
            mTouchTimer = kActiveWindow;
        }
        
        mTouches.push_back( touch );
    }
    
    return false;
}

//-----------------------------------
bool MultitouchControl::touchMoved( ofTouchEventArgs& touch )
{
    bool bHandled = false;

    for( int i = 0; i < mTouches.size(); ++i )
    {
        if ( mTouches[i].id == touch.id )
        {
            mTouches[i] = touch;
            bHandled = true;
            break;
        }
    }
    
    if ( active() )
    {
        setParameters();
    }
    
    return bHandled;
}

//-----------------------------------
bool MultitouchControl::touchUp( ofTouchEventArgs& touch )
{
    bool bHandled = false;

    for( std::vector<ofTouchEventArgs>::iterator iter = mTouches.begin(); iter != mTouches.end(); ++iter )
    {
        if ( iter->id == touch.id )
        {
            mTouches.erase(iter);
            bHandled = true;
            break;
        }
    }
    
    if ( active() )
    {
        deactivate();
        mbActive = false;
    }
    
    return bHandled;
}