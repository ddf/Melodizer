#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"

// forward declares
class  Screen;
@class UIActionHandler;
@class UIActionSheet;

namespace Minim 
{
	class AudioSystem;
	class AudioOutput;
	class Noise;
	class TickRate;
	class Multiplier;
	class Oscil;
	class WaveShaper;
	class SampleRecorder;
	class Pan;
}

class App : public ofxiPhoneApp 
{
	
public:
	App();
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	void touchCancelled(ofTouchEventArgs &touch);
	
	Minim::AudioOutput & Out() { return *mOutput; }
	
	void pauseAudio();
	void resumeAudio();
	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);
	void handlePinchGesture();
	
private:
	
	// INPUT
	UIPinchGestureRecognizer *	mPinchGestureRecognizer;
	
	// SOUND
	Minim::AudioSystem *	  mAudioSystem;
	Minim::AudioOutput *	  mOutput;
	
	// UI
	UIActionHandler				 * mActionHandler;
	UIActionSheet			     * mActionSheet; // so we can ask things
	
	bool						   m_bWasPlaying; // keep track of whether we were playing or not when audio got paused.
	
};

