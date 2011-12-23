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
{
    
}

//--------------------------------
ScaleChooser::~ScaleChooser()
{
    
}

//--------------------------------
void ScaleChooser::setup( const float x, const float y )
{
    mBox.setCenter(x, y);
    mFont.loadFont( "HelveticaBold.ttf", 24 );
    
    float bx = x;
    float sp = mBox.mH*1.1f;
    float by = y - (ScalesLength*sp) / 2;
    for( int i = 0; i < ScalesLength; ++i )
    {
        Button* b = new Button( Scales[i]->name(), bx, by, mBox.mW, mBox.mH );
        mButtons.push_back(b);
        by += sp;
    }
}

//--------------------------------
void ScaleChooser::update( const float dt )
{
    
}

//--------------------------------
void ScaleChooser::draw()
{
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
            
        case ST_OPEN:
        {
            for( int i = 0; i < mButtons.size(); ++i )
            {
                Button* b = mButtons[i];
                
                if ( i == Settings::Scale )
                {
                    ofSetColor(255, 128, 0, 255*0.95f);
                }
                else
                {
                    ofSetColor(80, 80, 80, 255*0.95f);
                }
                
                ofFill();
                
                ofRect( b->mBox.mX, b->mBox.mY, b->mBox.mW, b->mBox.mH );
                
                ofSetColor(200, 200, 200);
                const string    name = Scales[ i ]->name();
                const float     yoff = mFont.getLineHeight() * 0.35f;
                const float     xoff = mFont.stringWidth( name ) * -0.5f;
                mFont.drawString( name, b->mBox.mX + xoff, b->mBox.mY + yoff );
            }
        }
        break;
        
        default:
            break;
    }
}

//--------------------------------
void ScaleChooser::open()
{
    mState = ST_OPEN;
}

//--------------------------------
void ScaleChooser::close()
{
    mState = ST_CLOSED;
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