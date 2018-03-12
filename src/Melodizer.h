#pragma  once

#include "IPlug_include_in_plug_hdr.h"

#include "Params.h"
#include "Interface.h"

#include <vector>
#include <random>

#include "Summer.h"
#include "Multiplier.h"
#include "Line.h"
#include "Delay.h"
#include "Flanger.h"

#include "IMidiQueue.h"

class Tone;
class Scale;

namespace Minim
{
	class Waveform;
}

class Melodizer : public IPlug
{
public:
  Melodizer(IPlugInstanceInfo instanceInfo);
  ~Melodizer();

  void Reset() override;

  // we override this method in plugins so that we can know when
  // a user is trying to change play state from the UI.
  // this is because we ignore parameter changes in OnParamChanged
  // if the Host is not in play mode.
#ifndef SA_API
  void SetParameterFromGUI(int idx, double normalizedValue) override;
#endif
	
  void OnParamChange(int paramIdx) override;
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames) override;

  void BeginMIDILearn(int param1, int param2, int x, int y);

  // called from the UI for the Load and Save buttons.
  // we need to wrap LoadProgramFromFXP and SaveProgramAsFXP
  // to prevent our presets from getting overwritten.
  void HandleSave(WDL_String* fileName);
  void HandleLoad(WDL_String* fileName);


  virtual void ProcessMidiMsg(IMidiMsg* pMsg) override;
  virtual void ProcessSysEx(ISysEx* pSysEx) override;

private:
	void MakePresets();
	void InitRandomizerParam(const int paramIdx, const char * paramName);
	void GenerateNote(int tick, unsigned int waveformIdx, const Scale* notes, unsigned int key, int lowOctave, int hiOctave, unsigned int& previousNoteIndex);
	int RandomRange(int low, int hi);
	float RandomRange(float low, float hi);

	void StopSequencer();
	void ChangePlayState(PlayState toState);
	void SetPlayStateFromMidi(PlayState state);
	void HandleMidiControlChange(IMidiMsg* pMsg);
	void SetControlChangeForParam(const IMidiMsg::EControlChangeMsg cc, const int paramIdx);
	void SetBeatIncrement(const double tempo);
	float CalcDelayDuration(const StepLength stepLength, const double tempo);
	void SetDelayDuration(const double tempo, const double crossfade);

	Interface mInterface;

	IMidiQueue mMidiQueue;
	IMidiMsg::EControlChangeMsg mControlChangeForParam[kNumParams];
	int mMidiLearnParamIdx;

	// param values, cached in OnParamChange.
	// added these here because MIDI control changes can update params mid-process frame.
	// so this is better than having locals in that loop.
	float        mTempo; // in BPM
	PlayState	 mPlayState;
	unsigned int mWaveFormIdx;
	unsigned int mScaleIdx;
	unsigned int mKeyIdx;
	unsigned int mLowOctave;
	unsigned int mHiOctave;
	
	// will be true if we automatically entered play mode in Reset
	bool mAutoPlayed;

	// where are we in a "beat", which is two steps in the sequencer.
	// this is [0,1]
	double mBeatTime;
	// how much we should increment beat time every audio sample.
	// this will change depending one Tempo and Step Length.
	double mBeatInc;
	// is the tick even or odd,
	// so we know when to change to the next tick based on shuffle.
	bool mOddTick;
	// the step in the sequence we are at
	unsigned int mTick;
	unsigned int mPreviousNoteIndex;
	// true if we should crossfade between delays at the beginning of Process
	bool mCrossfadeDelays;

	// random number generation for testing against probabilities and generating random numbers for tones, used by RandomRange methods
	std::default_random_engine mRandomGen;

	Minim::Summer mMelodyBus;
	Minim::Multiplier mMelodyVolume;
	Minim::Line mMelodyVolumeLine;

	// we use two delays so we can crossfade between them when delay duration changes
	// in order to mask pitch-warble artifacts and clicks that would happen when using only one.
	Minim::Delay mDelayA;
	Minim::Delay mDelayB;
	// this is not patched to anything, we tick it explicitly in Process
	// and then set each Delay's dryMix and delAmp based on this value and current param values.
	Minim::Line mDelayCrossfade;
	// these are patched to both delays because they can be the same regardless of crossfade state
	Minim::Line mDelayFeedback;
	Minim::Line mDelayWetMix;
	// this is not patched to anything, we tick it explicitly in Process
	// and then set dry mix on each delay using the crossfade value.
	Minim::Line mDelayDryMix;

	// both Delays are patched to this, which then connects to the Flanger
	Minim::Summer mDelayBus;

	Minim::Flanger mFlanger;
	Minim::Line    mFlangerTime;
	Minim::Line	   mFlangerDepth;

	// we'll have one Tone for each voice that will be reconfigured before being triggered
	std::vector<Tone*> mTones;
	unsigned int mActiveTone;

	// we'll have a number of Waveforms to choose from
	std::vector<Minim::Waveform*> mWaveforms;

public:
	void DumpPresetSrc();
};
