
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

App* gApp = NULL;

// global tweaks
const int kOutputBufferSize = 1024;
const int kStreamBufferSize = 512;

//--------------------------------------------------------------
App::App()
: ofxiPhoneApp()
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
        mDrumBus.patch( mMixBus );
        
        mRate.setInterpolation(true);
        mMixBus.patch( mRate ).patch( *mOutput );
        
        SetupInstruments();
        
        mOutput->pauseNotes();
        mOutput->setTempo( 120 );
        mOutput->playNote(0.001f, 0.25f, mLooper);
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
    
    ofSetColor(200, 0, 128);
    ofNoFill();
    
    const int bufferSize = mOutput->buffer().getBufferSize();
    for( int i = 0; i < bufferSize; ++i )
    {
        int x = 15 + mXOff * (i % 20);
        int y = 15 + mYOff * (i / 20);
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
    
    // SETTINGS
    if ( mSettingsScreen.active() )
    {
        mSettingsScreen.draw();
    }
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


