
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
		{
			mPinchGestureRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:mActionHandler action:@selector(handlePinchGesture:)];
			[ofxiPhoneGetUIWindow() addGestureRecognizer:mPinchGestureRecognizer];
		}
	}
	
	//-- DONE ---------------------------
	m_bWasPlaying			= false;
}

//--------------------------------------------------------------
void App::update() 
{
	const float dt  = 1.0f / ofGetFrameRate();
}

//--------------------------------------------------------------
void App::draw() 
{	

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
	
	if ( mAudioSystem )
	{
		delete mAudioSystem;
	}
}


