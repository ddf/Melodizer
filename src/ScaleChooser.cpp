//
//  ScaleChooser.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/23/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "ScaleChooser.h"
#include "ofMain.h"
#include "Scales.h"
#include "Settings.h"

//--------------------------------
ScaleChooser::ScaleChooser()
: mState(ST_CLOSED)
, mBox(0, 0, 400, 45)
, mAnim(0)
{
    
}

//--------------------------------
ScaleChooser::~ScaleChooser()
{
    for( int i = 0; i < mButtons.size(); ++i )
    {
        Button* b = mButtons[i];
        if ( b )
        {
            delete b;
        }
    }
    
    if ( mButtonFlyout )
    {
        delete [] mButtonFlyout;
    }
}

//--------------------------------
void ScaleChooser::setup( const float x, const float y, const float w, const float h )
{
    mBox.mW = w;
    mBox.mH = h;
    mBox.setCenter(x, y);
    mFont.loadFont( "HelveticaBold.ttf", 24*(ofGetWidth()/1024.0f) );
    
    mButtonFlyout = new float[ScalesLength];
    
    float bx = x;
    float sp = h + 5.0f * (ofGetHeight()/768.0f);
    float by = y;
    for( int i = 0; i < ScalesLength; ++i )
    {
        mButtonFlyout[i] = by;
        Button* b        = new Button( Scales[i]->name(), bx, y, mBox.mW, mBox.mH );
        
        mButtons.push_back(b);
        
        by += sp;
    }
}

//--------------------------------
void ScaleChooser::update( const float dt )
{
    switch (mState)
    {
        case ST_OPENING:
            mAnim += dt*4;
            if ( mAnim >= 1 )
            {
                mAnim = 1;
                mState = ST_OPEN;
            }
            break;
            
        case ST_CLOSING:
            mAnim -= dt*4;
            if ( mAnim <= 0 )
            {
                mAnim = 0;
                mState = ST_CLOSED;
            }
            break;
            
        default:
            break;
    }
}

//--------------------------------
void ScaleChooser::draw()
{
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    switch (mState) 
    {
        case ST_CLOSED:
        {
            ofSetColor(255, 128, 0, 255*0.95f);
            ofFill();
            
            ofRect( mBox.mX, mBox.mY, mBox.mW, mBox.mH );
            
            ofSetColor(200, 200, 200);
            const string    name = Scales[ Settings::Scale ]->name();
            const float     yoff = mFont.getLineHeight() * 0.35f;
            const float     xoff = mFont.stringWidth( name ) * -0.5f;
            mFont.drawString( name, mBox.mX + xoff, mBox.mY + yoff );
        }
        break;
            
        case ST_CLOSING:
        case ST_OPENING:
        case ST_OPEN:
        {
            ofSetColor(0, 0, 0, 200.0f * mAnim);
            ofRect(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
            
            for( int i = 0; i < mButtons.size(); ++i )
            {
                Button* b = mButtons[i];
                
                if ( i == Settings::Scale )
                {
                    continue;
                }
                else
                {
                    ofSetColor(80, 80, 80, 255*0.95f);
                }
                
                ofFill();
                
                const float by = ofMap(mAnim, 0, 1, mBox.mY, mButtonFlyout[i]);
                b->mBox.setCenter( mBox.mX, by );
                
                ofRect( b->mBox.mX, b->mBox.mY, b->mBox.mW, b->mBox.mH );
                
                ofSetColor(200, 200, 200);
                const string    name = Scales[ i ]->name();
                const float     yoff = mFont.getLineHeight() * 0.35f;
                const float     xoff = mFont.stringWidth( name ) * -0.5f;
                mFont.drawString( name, b->mBox.mX + xoff, b->mBox.mY + yoff );
            }
            
            // draw active scale last
            Button* b = mButtons[Settings::Scale];
            
            ofSetColor(255, 128, 0, 255*0.95f);
            
            const float by = ofMap(mAnim, 0, 1, mBox.mY, mButtonFlyout[Settings::Scale]);
            b->mBox.setCenter( mBox.mX, by );
            
            ofRect( b->mBox.mX, b->mBox.mY, b->mBox.mW, b->mBox.mH );
            
            ofSetColor(200, 200, 200);
            const string    name = Scales[ Settings::Scale ]->name();
            const float     yoff = mFont.getLineHeight() * 0.35f;
            const float     xoff = mFont.stringWidth( name ) * -0.5f;
            mFont.drawString( name, b->mBox.mX + xoff, b->mBox.mY + yoff );
        }
        break;
        
        default:
            break;
    }
}

//--------------------------------
void ScaleChooser::open()
{
    mState = ST_OPENING;
    
    const float sp = mBox.mH + 5.0f * (ofGetHeight()/768.0f);
    float by = mBox.mY - sp;
    for( int i = 0; i < ScalesLength; ++i )
    {
        if ( Settings::Scale == i )
        {
            mButtonFlyout[i] = mBox.mY;
        }
        else
        {
            mButtonFlyout[i] = by;
            by -= sp;
        }
    }
}

//--------------------------------
void ScaleChooser::close()
{
    mState = ST_CLOSING;
}

//--------------------------------
bool ScaleChooser::active()
{
    return mState != ST_CLOSED;
}

//--------------------------------
bool ScaleChooser::handleTouch( const float x, const float y )
{
    if ( mState == ST_CLOSED && mBox.contains(x, y) )
    {
        open();
    }
    else if ( mState == ST_OPEN )
    {
        for( int i = 0; i < mButtons.size(); ++i )
        {
            if ( mButtons[i]->mBox.contains(x, y) )
            {
                Settings::Scale = i;
                Settings::PreviousBassNoteIndex   = 0;
                Settings::PreviousMelodyNoteIndex = 0;
                break;
            }
        }
        
        close();
    }
    
    return active();
}