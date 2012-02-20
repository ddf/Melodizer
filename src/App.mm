
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
const int kStreamBufferSize = 512;

//--------------------------------------------------------------
App::App()
: ofxiPhoneApp()
, mFlanger( 5, 0.1f, 2, 0.3f, 0.8f, 0.0f )
, mRepeater(1.0f) // repeater that can repeat a max of one second
, mSampleRepeatControl( mRepeater )
, mFlangerControl( mFlanger )
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
	
	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	// might want to do this at some point.
	ofxiPhoneSetOrientation( OF_ORIENTATION_90_RIGHT );
	
	//-- AUDIO --------------------------------------
	{
		TouchServiceProvider::AudioSessionParameters sessionParams;
		sessionParams.outputBufferDuration = (float)kOutputBufferSize / 44100.f;
		
		extern void AudioInterruptionListener ( void *inClientData, UInt32 inInterruptionState );
		sessionParams.interruptListener = &AudioInterruptionListener;
		sessionParams.interruptUserData = this;
		
		mAudioSystem = new Minim::AudioSystem( new TouchServiceProvider(sessionParams) );
		
		extern void AudioRouteChangeListener( void *inClientData, AudioSessionPropertyID inID, UInt32 inPropertySize, const void *inPropertyValue );
		AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, &AudioRouteChangeListener, this);
		
		// turn off the wavetable optimisation that skips interpolation.
		Minim::Wavetable::s_opt = false;
		
		mOutput = mAudioSystem->getAudioOutput( TouchAudioFormat(2), kOutputBufferSize );
        
        mMelodyBus.patch( mMixBus );
        mBassBus.patch( mMixBus );
        
        //mNoteBus.patch( mMixBus );
        mDrumBus.patch( mMixBus );
        
        mRate.setInterpolation(true);
        mMixBus.patch( mRepeater ).patch( mRate ).patch( mFlanger ).patch( *mOutput );
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
			[ofxiPhoneGetUIWindow() addGestureRecognizer:mPinchGestureRecognizer];
		}
	}
    
    //-- VIS ----------------------------
    {
        mXOff = 65;
        mYOff = 35;
        
        mSettingsScreen.setup();
        
        mXYControl.setXControl( ValueMapper(&Settings::Tempo, 0, ofGetWidth(), 40, 160) );
        mXYControl.setYControl( ValueMapper(mRate.value.getLastValues(), 0, ofGetHeight(), 1.0f, 0.2f) );
        mXYControl.setPosition( ofGetWidth()/2, ofGetHeight()/2 );
    }
	
	//-- DONE ---------------------------
	m_bWasPlaying			= false;
}

//--------------------------------------------------------------
void App::update() 
{
    const float dt  = 1.0f / ofGetFrameRate();
    
    mMelodyBus.volume.setLastValue( Settings::MelodyVolume );
    mBassBus.volume.setLastValue( Settings::BassVolume );
    
    mSampleRepeatControl.update( dt );
    mFlangerControl.update( dt );
    
    if ( mSettingsScreen.active() )
    {
        mSettingsScreen.update( dt );
        
        if ( !mSettingsScreen.active() )
        {
            ofRegisterTouchEvents( this );
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
    
    const ofPoint& xyPos = mXYControl.position();
    
    ofSetColor(21, 21, 21);
    ofFill();
    ofCircle(xyPos.x, xyPos.y, mXYControl.radius());
    
    ofSetColor(200, 0, 128);
    ofNoFill();
    
    const int bufferSize = mOutput->buffer().getBufferSize();
    for( int i = 0; i < bufferSize; ++i )
    {
        int x = 15 + mXOff * (i % 20);
        int y = 15 + mYOff * (i / 20);
        
        // offset triangle based on how close it is to the xy control position
        ofVec2f offDir( x - xyPos.x, y - xyPos.y );
        const float offset = mXYControl.radius() * fminf( mXYControl.radius() / offDir.length(), 1 );
        offDir.scale( offset );
        x += offDir.x;
        y += offDir.y;
        
        const float sampleL = mOutput->buffer().getSample(0, i);
        const float sampleR = fabs( mOutput->buffer().getSample(1, i ) );
        ofPushMatrix();
        {
            ofTranslate(x, y);
            ofRotate( 90 * sampleL );
            ofTriangle( 0, -20*sampleR, -30*sampleR, 30*sampleR, 30*sampleR, 30*sampleR );
        }
        ofPopMatrix();
    }
    
    // sample repeater control viz
    if ( mSampleRepeatControl.active() )
    {
        ofSetRectMode( OF_RECTMODE_CENTER );
        ofSetColor(128, 200, 0, 128);
        ofFill();
        
        ofRect( mSampleRepeatControl.getTouch(0).x, ofGetHeight()/2, 40, ofGetHeight() );
        ofRect( mSampleRepeatControl.getTouch(1).x, ofGetHeight()/2, 40, ofGetHeight() );
    }
    
    // flanger control viz
    if ( mFlangerControl.active() )
    {
        // pale yellow
        ofSetColor(255, 255, 0, 64);
        ofFill();
        
        ofPoint touch1 = mFlangerControl.getTouch(0);
        ofPoint touch2 = mFlangerControl.getTouch(1);
        ofPoint touch3 = mFlangerControl.getTouch(2);
        
        const float touchRadius = 65;
        ofCircle( touch1.x, touch1.y, touchRadius );
        ofCircle( touch2.x, touch2.y, touchRadius );
        ofCircle( touch3.x, touch3.y, touchRadius );
        
        // connect dots with lines, somehow try to communicate that the lines are the control values
        // idea: show pulsing circle in center of the dot to visualize the state of the
        //       flanger's lfo. maybe adjust triangle fill color to represent feedback.
    }
    
    // SETTINGS
    if ( mSettingsScreen.active() )
    {
        mSettingsScreen.draw();
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


