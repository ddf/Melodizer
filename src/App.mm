
#include "App.h"

// sound stuff
#include "AudioSystem.h"
#include "TouchServiceProvider.h"
#include "TouchAudioFormat.h"
#include "AudioOutput.h"
#include "SampleRecorder.h"
#include "Noise.h"
#include "Multiplier.h"
#include "Waves.h"
#include "Wavetable.h"
#include "WaveShaper.h"
#include "Oscil.h"
#include "TickRate.h"
#include "Pan.h"

// ui stuff
#include "ofxiPhoneExtras.h"
#include "UIActionHandler.h"
#include <math.h>

App* gApp = NULL;

// global tweaks
const int kOutputBufferSize = 1024;

//--------------------------------------------------------------
App::App()
: ofxiOSApp()
, mFlanger( 5, 0.1f, 2, 0.3f, 0.8f, 0.0f )
, mDelay( 2, 0, true, true )
, mRepeater(1.0f) // repeater that can repeat a max of one second
, mSampleRepeatControl( mRepeater )
, mFlangerControl( mFlanger )
, mDelayControl( mDelay )
, mButtonOpenSettings("Settings",0,0,0,0)
{
	gApp = this;
}

//--------------------------------------------------------------
void App::setup()
{	
	mAudioSystem = NULL;
	mOutput = NULL;
	
	ofSetBackgroundAuto(false);
	
	ofEnableAlphaBlending();
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// iPhoneAlerts will be sent to this.
	ofxiOSAlerts.addListener(this);
	
	// might want to do this at some point.
	ofSetOrientation( OF_ORIENTATION_90_RIGHT );
	
	//-- AUDIO --------------------------------------
	{
        extern void InterruptionListener ( void *inClientData, Minim::InterruptionState inInterruptionState );
		mAudioSystem = new Minim::AudioSystem( kOutputBufferSize, &InterruptionListener, this );
		
		extern void AudioRouteChangeListener( void *inClientData, AudioSessionPropertyID inID, UInt32 inPropertySize, const void *inPropertyValue );
		AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, &AudioRouteChangeListener, this);
		
		// turn off the wavetable optimisation that skips interpolation.
		Minim::Wavetable::s_opt = false;
		
		mOutput = mAudioSystem->getAudioOutput( TouchAudioFormat(2), kOutputBufferSize );
        
        mMelodyBus.patch( mMixBus );
        mBassBus.patch( mMixBus );
        
        //mNoteBus.patch( mMixBus );
        
        mMixBus.patch( mRepeater ).patch( mFlanger ).patch( mDelay ).patch( *mOutput );
        //mMixBus.patch( *mOutput );
        
        SetupInstruments();
        
        mOutput->pauseNotes();
        mOutput->setTempo( 120 );
        mOutput->playNote(1.0f, 0.25f, mLooper);
        mOutput->resumeNotes();
	}
	
	//-- UI ---------------------------
	{
		mActionHandler = [[UIActionHandler alloc] init:this];
		
		
		// action sheet shown sometimes
		{
			NSString * actionTitle = NSLocalizedString(@"A false truth?", @"");
			NSString * actionYes   = NSLocalizedString(@"Yes", @"");
			NSString * actionNo    = NSLocalizedString(@"Yes", @"");
			mActionSheet = [[UIActionSheet alloc] initWithTitle:actionTitle delegate:mActionHandler cancelButtonTitle:nil destructiveButtonTitle:actionNo otherButtonTitles:actionYes,nil];
			mActionSheet.actionSheetStyle = UIActionSheetStyleBlackTranslucent;
			mActionSheet.delegate = mActionHandler;
		}
		
		// pinch gesture mostly for zooming in and out
        if ( 0 )
		{
			mPinchGestureRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:mActionHandler action:@selector(handlePinchGesture:)];
			[ofxiOSGetUIWindow() addGestureRecognizer:mPinchGestureRecognizer];
		}
	}
    
    //-- VIS ----------------------------
    {
        mXOff = 65;
        mYOff = 35;
    }
	
	//-- DONE ---------------------------
	m_bWasPlaying			= false;
}

//--------------------------------------------------------------
void App::update() 
{
    if ( mSettingsScreen.needsSetup() )
    {
        mSettingsScreen.setup();

        const float screenScale = (float)ofGetHeight() / 768.0f;
        const float xydim = 300*screenScale;
        
        const float rcx = ofGetWidth()/2 - xydim - 40*screenScale;
        const float rcy = ofGetHeight()/2;
        mSampleRepeatControl.setup("Stutter", rcx, rcy, xydim);
        
        const float fcx   = ofGetWidth()/2;
        const float fcy   = ofGetHeight()/2;
        mFlangerControl.setup("Flanger", fcx, fcy, xydim);
        
        const float dcx = ofGetWidth()/2 + xydim + 40*screenScale;
        const float dcy = ofGetHeight()/2;
        mDelayControl.setup("Delay", dcx, dcy, xydim);
        
        const float sbw = 200*screenScale;
        const float sbh = 100*screenScale;
        const float sbx = ofGetWidth() - sbw/2 - 15*screenScale;
        const float sby = ofGetHeight() - sbh/2 - 15*screenScale;
        mButtonOpenSettings.setBox(sbx, sby, sbw, sbh);
        mButtonOpenSettings.setText("Settings", mSettingsScreen.UIFont());
    }

    const float fps = ofGetFrameRate();
    
    // protect against divide by zero
    if ( fps > 0 )
    {
        const float dt  = 1.0f / fps;
        
        mMelodyBus.volume.setLastValue( Settings::MelodyVolume );
        mBassBus.volume.setLastValue( Settings::BassVolume );
        
        if ( mSettingsScreen.active() )
        {
            mSettingsScreen.update( dt );
        }
    }
}

//--------------------------------------------------------------
void App::draw() 
{	
    ofBackground(20, 20, 20);
    
//    ofSetColor(20, 20, 20, 40);
//    ofFill();
//    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    ofSetColor(200, 0, 128);
    ofNoFill();
    
//    const int bufferSize = mOutput->buffer().getBufferSize();
//    for( int i = 0; i < bufferSize; ++i )
//    {
//        int x = 15 + mXOff * (i % 20);
//        int y = 15 + mYOff * (i / 20);
//        
//        // offset triangle based on how close it is to the xy control position
//        ofVec2f offDir( x - xyPos.x, y - xyPos.y );
//        const float offset = mXYControl.radius() * fminf( mXYControl.radius() / offDir.length(), 1 );
//        offDir.scale( offset );
//        x += offDir.x;
//        y += offDir.y;
//        
//        const float sampleL = mOutput->buffer().getSample(0, i);
//        const float sampleR = fabs( mOutput->buffer().getSample(1, i ) );
//        ofPushMatrix();
//        {
//            ofTranslate(x, y);
//            ofRotate( 90 * sampleL );
//            ofTriangle( 0, -20*sampleR, -30*sampleR, 30*sampleR, 30*sampleR, 30*sampleR );
//        }
//        ofPopMatrix();
//    }
    
    // SETTINGS
    if ( mSettingsScreen.active() )
    {
        mSettingsScreen.draw();
    }
    else
    {
        mFlangerControl.draw();
        mSampleRepeatControl.draw();
        mDelayControl.draw();
        mButtonOpenSettings.draw(mSettingsScreen.UIFont(), ofColor(255,128,0), ofColor(200,200,200));
    }
    
    ofSetColor(255, 255, 255);
    string fps("FPS: ");
    fps += ofToString( ofGetFrameRate() );
    ofDrawBitmapString(fps, 5, 15);
}

//--------------------------------------------------------------
void App::exit() 
{
	[mActionHandler release];
	
	if ( mOutput )
	{
		mOutput->close();
		delete mOutput;
	}
    
    ClearInstruments();
	
	if ( mAudioSystem )
	{
		delete mAudioSystem;
	}
}


