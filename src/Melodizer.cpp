#include "Melodizer.h"
#include "IPlug_include_in_plug_src.h"
#include "resource.h"
#include "Params.h"

#include "Scales.h"

#include "Waves.h"
#include "Instruments.h" // contains Tone

#if SA_API
extern char * gINIPath;
#endif

using namespace Minim;

const int kNumPrograms = 1;

// name of the section of the INI file we save midi cc mappings to
const char * kMidiControlIni = "midicc";

const char * kStepLengthDisplay[SL_Count] = {
	"1/64T", "1/64", "1/32T",
	"1/64.", "1/32", "1/16T",
	"1/32.", "1/16", "1/8T",
	"1/16.", "1/8",  "1/4T",
	"1/8.",  "1/4",  "1/4.",
};

const double kFlangerTimeMinMs = 0.01f;
const double kFlangerTimeMaxMs = 50.0f;

const double kFlangerRateMinHz = 0.01;
const double kFlangerRateMaxHz = 20;

const double kDelayCrossfadeDuration = 0.1;

Melodizer::Melodizer(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
	, mInterface(this)
	, mMidiLearnParamIdx(-1)
	, mTempo(DEFAULT_TEMPO)
	, mSamplesPerBeat(0)
	, mPlayState(PS_Stop)
	, mWaveFormIdx(0)
	, mScaleIdx(0)
	, mKeyIdx(0)
	, mLowOctave(4)
	, mHiOctave(0)
	, mTick(0)
	, mPreviousNoteIndex(0)
	, mCrossfadeDelays(false)
	, mSampleCount(0)
	, mOddTick(false)
	, mMelodyBus()
	, mMelodyVolume(0)
	, mMelodyVolumeLine(0,0,0)
	, mDelayA(kDelayDurationMaxSeconds, 0.5f, true)
	, mDelayB(kDelayDurationMaxSeconds, 0.5f, true)
	, mDelayCrossfade(0,0,0)
	, mDelayFeedback(0, 0.5f, 0.5f)
	, mDelayDryMix(0, 1, 1)
	, mDelayWetMix(0, 0, 0)
	, mDelayBus()
	, mFlanger(kFlangerTimeMinMs, kFlangerRateMinHz, 0, 0, 1, 1)
	, mFlangerTime(0, kFlangerTimeMinMs, kFlangerTimeMinMs)
	, mFlangerDepth(0, 0, 0)
	, mRandomGen(0)
	, mTones(kVoicesMax)
	, mActiveTone(0)
{
	TRACE;

	memset(mControlChangeForParam, 0, sizeof(mControlChangeForParam));
	
	// setup dsp chain
	{
		mDelayFeedback.patch(mDelayA.feedback);
		mDelayFeedback.patch(mDelayB.feedback);
		mDelayWetMix.patch(mDelayA.wetMix);
		mDelayWetMix.patch(mDelayB.wetMix);

		mFlangerTime.patch(mFlanger.delay);
		mFlangerDepth.patch(mFlanger.depth);

		mMelodyVolumeLine.patch(mMelodyVolume.amplitude);

		// melody goes into both delays, which connect to the delay bus
		mMelodyBus.patch(mDelayA).patch(mDelayBus);
		mMelodyBus.patch(mDelayB).patch(mDelayBus);

		// the output of the delay bus is sent to the flanger
		// and on to the main volume.
		mDelayBus.patch(mFlanger).patch(mMelodyVolume);

		// configure default audio settings (this is also done in Reset)
		mMelodyVolume.setAudioChannelCount(2);
		mMelodyVolume.setSampleRate(44100);
		mDelayCrossfade.setSampleRate(44100);
		mDelayDryMix.setSampleRate(44100);
	}

	// setup Waveforms
	{
		const unsigned int kWaveformLength = 8192;
		// seed the random gen with 0 so we always get the same harmonics
		mRandomGen.seed(0);
		std::uniform_real_distribution<> dist(-0.5f, 0.5f);
		// generate 32 harmonic amplitudes that will be used to generate some of our waveforms.
		float harmonics[32];
		for (int i = 0; i < 32; ++i)
		{
			harmonics[i] = dist(mRandomGen);
		}

		mWaveforms.resize(WT_Count);

		IParam* param = GetParam(kWaveform);
		param->InitEnum("Waveform", WT_Sine4, WT_Count);

		for (int i = 0; i < WT_Count; ++i)
		{
			WaveformType type = (WaveformType)i;
			switch (type)
			{
			case WT_Sine:
				param->SetDisplayText(i, "SIN");
				mWaveforms[i] = Waves::SINE();
				break;
			case WT_Triangle:
				param->SetDisplayText(i, "TRI");
				mWaveforms[i] = Waves::TRIANGLE();
				break;
			case WT_Saw:
				param->SetDisplayText(i, "SAW");
				mWaveforms[i] = Waves::SAW();
				break;
			case WT_Square:
				param->SetDisplayText(i, "SQR");
				mWaveforms[i] = Waves::SQUARE();
				break;
			case WT_Sine4:
				param->SetDisplayText(i, "SIN4");
				mWaveforms[i] = Waves::gen10(kWaveformLength, harmonics, 4);
				break;
			case WT_Sine8:
				param->SetDisplayText(i, "SIN8");
				mWaveforms[i] = Waves::gen10(kWaveformLength, harmonics, 8);
				break;
			case WT_Sine16:
				param->SetDisplayText(i, "SIN16");
				mWaveforms[i] = Waves::gen10(kWaveformLength, harmonics, 16);
				break;
			case WT_Sine32:
				param->SetDisplayText(i, "SIN32");
				mWaveforms[i] = Waves::gen10(kWaveformLength, harmonics, 32);
				break;
			case WT_Count:
				break;
			default:
				break;
			}
		}
	}
	
	// synth settings
	{
		GetParam(kPulseWidth)->InitDouble("Pulse Width", 0.5, 0.01, 0.5, 0.01);
		GetParam(kVoices)->InitInt("Max Voices", 16, kVoicesMin, kVoicesMax);
		GetParam(kVolume)->InitDouble("Volume", -6, -48, 6, 0.1f, "db");
		GetParam(kWidth)->InitDouble("Width", 100, 0, 100, 0.5, "%");
		GetParam(kGlide)->InitDouble("Glide", 0, 0, 2, 0.01, "seconds");
		GetParam(kMovement)->InitDouble("Movement", 0, 0, 2, 0.01, "seconds");
		
		// make a Tone for each voice we can have
		for(int i = 0; i < kVoicesMax; ++i)
		{
			mTones[i] = new Tone(mMelodyBus);
		}
	}

	// scales
	{
		IParam* param = GetParam(kScale);
		param->InitEnum("Scale", 0, ScalesLength);
		for (int i = 0; i < ScalesLength; ++i)
		{
			param->SetDisplayText(i, Scales[i]->name);
		}
	}

	// key (ie root note)
	{
		const char * keys[] = { "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" };
		const int keyCount = 12;
		IParam* param = GetParam(kKey);
		param->InitEnum("Key", 0, keyCount);
		for (int i = 0; i < keyCount; ++i)
		{
			param->SetDisplayText(i, keys[i]);
		}
	}

	// tempo / step
	{
		IParam* param = GetParam(kClockSource);
		param->InitEnum("Clock Source", CS_Internal, CS_Count);
		param->SetDisplayText(CS_Internal, "Int.");
		param->SetDisplayText(CS_External, "Ext.");

		GetParam(kTempo)->InitDouble("Tempo", DEFAULT_TEMPO, kTempoMin, kTempoMax, 0.01, "bpm");
		
		param = GetParam(kStepLength);
		param->InitEnum("Step Length", SL_8, SL_Count);
		for(int i = 0; i < SL_Count; ++i)
		{
			param->SetDisplayText(i, kStepLengthDisplay[i]);
		}

		param = GetParam(kPlayState);
		param->InitEnum("Play State", PS_Stop, PS_Count);
		for(int i = 0; i < PS_Count; ++i)
		{
			switch(i)
			{
				case PS_Stop: param->SetDisplayText(i, "Stop"); break;
				case PS_Pause: param->SetDisplayText(i, "Pause"); break;
				case PS_Play: param->SetDisplayText(i, "Play"); break;
			}
		}
	}
	
	// octave + range
	{
		GetParam(kOctave)->InitInt("Octave", 4, kOctaveMin, kOctaveMax);
		GetParam(kRange)->InitInt("Range", 0, kRangeMin, kRangeMax);
	}

	// shuffle
	{
		GetParam(kShuffle)->InitDouble("Shuffle", 0.5, 0.1, 0.9, 0.01f);
	}

	// seed
	{
		GetParam(kSeed)->InitInt("Seed", 0, kSeedMin, kSeedMax);
	}

	// ADSR
	{
		const double secondsStep = 0.001f;
		const double minEnv = 0.005f;
		GetParam(kEnvAttack)->InitDouble("Attack", minEnv, minEnv, 2, secondsStep, "seconds");
		GetParam(kEnvDecay)->InitDouble("Decay", minEnv, minEnv, 2, secondsStep, "seconds");
		GetParam(kEnvSustain)->InitDouble("Sustain", 0.5, minEnv, 1, 0.01);
		GetParam(kEnvRelease)->InitDouble("Release", 0.25, minEnv, 5, secondsStep, "seconds");
	}

	// Effects 
	{
		IParam* param = GetParam(kDelayDuration);
		param->InitEnum("Delay Duration", SL_8, SL_Count);
		for (int i = 0; i < SL_Count; ++i)
		{
			param->SetDisplayText(i, kStepLengthDisplay[i]);
		}

		GetParam(kDelayFeedback)->InitDouble("Delay Feedback", 20, 0, 100, 0.5f, "%");
		GetParam(kDelayMix)->InitDouble("Delay Mix", 50, 0, 100, 0.5f, "%");

		GetParam(kFlangerTime)->InitDouble("Flanger Delay", mFlanger.delay.getLastValue(), kFlangerTimeMinMs, kFlangerTimeMaxMs, 0.01, "ms");
		GetParam(kFlangerRate)->InitDouble("Flanger Rate", mFlanger.rate.getLastValue(), kFlangerRateMinHz, kFlangerRateMaxHz, 0.01, "Hz");
		GetParam(kFlangerDepth)->InitDouble("Flanger Depth", 10, 0, 100, 0.5, "%");
		GetParam(kFlangerFeedback)->InitDouble("Flanger Feedback", mFlanger.feedback.getLastValue() * 100, 0, 100, 0.5, "%");
		GetParam(kFlangerMix)->InitDouble("Flanger Mix", 0, 0, 100, 0.5, "%");
	}

	// knob bank
	{
		//arguments are: name, defaultVal, minVal, maxVal, step, label
		char paramName[32];
		const double percentStep = 0.5f;
		for (int i = 0; i < kSequencerSteps; ++i)
		{
			sprintf(paramName, "Step %d Mode", i);
			GetParam(kStepModeFirst + i)->InitEnum(paramName, SM_Norm, SM_Count);
			for (int mode = SM_Norm; mode < SM_Count; ++mode)
			{
				IParam* param = GetParam(kStepModeFirst + i);
				switch (mode)
				{
				case SM_Norm: param->SetDisplayText(mode, "NORM"); break;
				case SM_Skip: param->SetDisplayText(mode, "SKIP"); break;
				case SM_Loop: param->SetDisplayText(mode, "LOOP"); break;
				}
			}

			sprintf(paramName, "Step %d Probability", i);
			GetParam(kProbabilityFirst + i)->InitDouble(paramName, 50, 0, 100, percentStep, "%");
			
			sprintf(paramName, "Step %d Pan", i);
			GetParam(kPanFirst + i)->InitDouble(paramName, 0, -1, 1, 0.1);
			
			sprintf(paramName, "Step %d Velocity", i);
			GetParam(kVelocityFirst + i)->InitDouble(paramName, 100, 0, 100, percentStep, "%");

			sprintf(paramName, "Step %d Attack", i);
			GetParam(kAttackFirst + i)->InitDouble(paramName, 100, 1, 100, percentStep, "%");

			sprintf(paramName, "Step %d Decay", i);
			GetParam(kDecayFirst + i)->InitDouble(paramName, 100, 1, 100, percentStep, "%");

			sprintf(paramName, "Step %d Sustain", i);
			GetParam(kSustainFirst + i)->InitDouble(paramName, 100, 1, 100, percentStep, "%");

			sprintf(paramName, "Step %d Release", i);
			GetParam(kReleaseFirst + i)->InitDouble(paramName, 100, 1, 100, percentStep, "%");
		}
	}

	// randomizers
	InitRandomizerParam(kProbabilityRandomize, "Randomize Probabilities");
	InitRandomizerParam(kPanRandomize, "Randomize Pans");
	InitRandomizerParam(kVelocityRandomize, "Randomize Velocities");
	InitRandomizerParam(kAttackRandomize, "Randomize Attacks");
	InitRandomizerParam(kDecayRandomize, "Randomize Decays");
	InitRandomizerParam(kSustainRandomize, "Randomize Sustain");
	InitRandomizerParam(kReleaseRandomize, "Randomize Releases");

	// interface!
	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
	mInterface.CreateControls(pGraphics);
	AttachGraphics(pGraphics);

	//MakePreset("preset 1", ... );
	MakeDefaultPreset((char *) "Init", kNumPrograms);
}

Melodizer::~Melodizer()
{
	for (int i = 0; i < mWaveforms.size(); ++i)
	{
		if (mWaveforms[i] != nullptr)
		{
			delete mWaveforms[i];
		}
	}
	mWaveforms.clear();

	for (int i = 0; i < mTones.size(); ++i)
	{
		delete mTones[i];
	}
	mTones.clear();
}

void Melodizer::InitRandomizerParam(const int paramIdx, const char *paramName)
{
	GetParam(paramIdx)->InitEnum(paramName, 0, 2);
	GetParam(paramIdx)->SetDisplayText(0, "Off");
	GetParam(paramIdx)->SetDisplayText(1, "On");
}

void Melodizer::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	if (GetParam(kClockSource)->Int() == CS_External)
	{
		OnParamChange(kTempo);
	}

	if (mCrossfadeDelays && mDelayCrossfade.isAtEnd())
	{
		SetDelayDuration(mTempo, kDelayCrossfadeDuration);
		mCrossfadeDelays = false;
	}

	// Mutex is already locked for us.
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	float result[2];
	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		while (!mMidiQueue.Empty())
		{
			IMidiMsg* pMsg = mMidiQueue.Peek();
			if (pMsg->mOffset > s) break;

			switch (pMsg->StatusMsg())
			{
			case IMidiMsg::kControlChange:
				HandleMidiControlChange(pMsg);
				break;

			case IMidiMsg::kNoteOn:
				// -1 because MIDI note 0 is considered Octave -1.
				GetParam(kOctave)->Set(pMsg->NoteNumber() / 12 - 1);
				InformHostOfParamChange(kOctave, GetParam(kOctave)->GetNormalized());
				OnParamChange(kOctave);
				GetGUI()->SetParameterFromPlug(kOctave, GetParam(kOctave)->Value(), false);

				GetParam(kKey)->Set(pMsg->NoteNumber() % 12);
				InformHostOfParamChange(kKey, GetParam(kKey)->GetNormalized());
				OnParamChange(kKey);
				GetGUI()->SetParameterFromPlug(kKey, GetParam(kKey)->Value(), false);
				break;
			}

			mMidiQueue.Remove();
		}

		if (mSampleCount == 0 && mPlayState == PS_Play)
		{
			mTones[mActiveTone]->noteOff();

			// advance one tick
			int nextTick = (mTick + 1) % kSequencerSteps;

			// figure out what the next actual tick should be based on StepModes
			// if the next tick in sequence is marked Loop we jump back to the beginning instead of processing it
			if (GetParam(kStepModeFirst + nextTick)->Int() == SM_Loop)
			{
				nextTick = 0;
			}

			// if the next tick in the sequence is marked Skip,
			// we look at the one after that, stopping if we loop back around to our current tick
			// in order to prevent an infinite loop.
			while (GetParam(kStepModeFirst + nextTick)->Int() == SM_Skip && nextTick != mTick)
			{
				nextTick = (nextTick + 1) % kSequencerSteps;
				// again, if we encounter a Loop, we reset the sequence,
				// we don't break because the first step might be marked Skip
				if (GetParam(kStepModeFirst + nextTick)->Int() == SM_Loop)
				{
					nextTick = 0;
				}
			}
			
			mTick = nextTick;

			if (mOddTick)
			{
				mSampleCount = (unsigned long)round(mSamplesPerBeat*GetParam(kShuffle)->Value());
			}
			else
			{
				mSampleCount = (unsigned long)round(mSamplesPerBeat*(1.0 - GetParam(kShuffle)->Value()));
			}

			mOddTick = !mOddTick;

			const bool noteOn = RandomRange(0.f, 100.f) <= GetParam(kProbabilityFirst + mTick)->Value();
			mInterface.OnTick(mTick, noteOn);
			if (noteOn)
			{
				GenerateNote(mTick, mWaveFormIdx, Scales[mScaleIdx], mKeyIdx, mLowOctave, mHiOctave, mPreviousNoteIndex);
			}
		}

		// tick our crossfade line and put the result into both channels
		mDelayCrossfade.tick(result, 1);
		// then tick our dry mix and put the result into only one channel, 
		// which means we can still get at the result of ticking crossfade.
		mDelayDryMix.tick(result, 1);

		// use the results to set delAmp and dryMix on both delays
		const float fade = mDelayCrossfade.getAmp();
		const float mix = mDelayDryMix.getAmp();

		mDelayA.delAmp.setLastValue(1.0 - fade);
		mDelayB.delAmp.setLastValue(fade);			
		mDelayA.dryMix.setLastValue((1 - fade) * mix);
		mDelayB.dryMix.setLastValue(fade * mix);

		mMelodyVolume.tick(result, 2);
		*out1 = result[0];
		*out2 = result[1];

		if ( mSampleCount > 0 )
		{
			--mSampleCount;
		}
	}

	mMidiQueue.Flush(nFrames);
}

void Melodizer::BeginMIDILearn(int paramIdx1, int paramIdx2, int x, int y)
{
	if (GetAPI() == kAPIVST3)
	{
		// in Reaper on OSX the popup's Y position is inverted.
		// not sure if this is true in other hosts on OSX, so we only modify for Reaper.
#ifdef OS_OSX
		if (GetHost() == kHostReaper)
		{
			y = GUI_HEIGHT - y;
		}
#endif
		PopupHostContextMenuForParam(paramIdx1, x, y);
	}
	else if (GetAPI() == kAPISA)
	{
		IPopupMenu menu;
		menu.SetMultiCheck(true);
		WDL_String str;
		if (paramIdx1 != -1)
		{
			str.SetFormatted(64, "MIDI Learn: %s", GetParam(paramIdx1)->GetNameForHost());
			int flags = mControlChangeForParam[paramIdx1] ? IPopupMenuItem::kChecked : IPopupMenuItem::kNoFlags;
			menu.AddItem(str.Get(), -1, flags);
		}
		if (paramIdx2 != -1)
		{
			str.SetFormatted(64, "MIDI Learn: %s", GetParam(paramIdx2)->GetNameForHost());
			int flags = mControlChangeForParam[paramIdx2] ? IPopupMenuItem::kChecked : IPopupMenuItem::kNoFlags;
			menu.AddItem(str.Get(), -1, flags);
		}
		if (GetGUI()->CreateIPopupMenu(&menu, x, y))
		{
			const int chosen = menu.GetChosenItemIdx();
			if (chosen == 0)
			{
				if (menu.GetItem(chosen)->GetChecked())
				{
					SetControlChangeForParam((IMidiMsg::EControlChangeMsg)0, paramIdx1);
				}
				else
				{
					mMidiLearnParamIdx = paramIdx1;
				}
			}
			else if (chosen == 1)
			{
				if (menu.GetItem(chosen)->GetChecked())
				{
					SetControlChangeForParam((IMidiMsg::EControlChangeMsg)0, paramIdx2);
				}
				else
				{
					mMidiLearnParamIdx = paramIdx2;
				}
			}
			else
			{
				mMidiLearnParamIdx = -1;
			}
		}
	}
}

void Melodizer::ProcessMidiMsg(IMidiMsg* pMsg)
{
#ifdef TRACER_BUILD
	pMsg->LogMsg();
#endif

	const IMidiMsg::EStatusMsg status = pMsg->StatusMsg();
	if (status == IMidiMsg::kControlChange || status == IMidiMsg::kNoteOn )
	{
		if (status == IMidiMsg::kControlChange)
		{
			const IMidiMsg::EControlChangeMsg cc = pMsg->ControlChangeIdx();
			if (mMidiLearnParamIdx != -1)
			{
				SetControlChangeForParam(cc, mMidiLearnParamIdx);
				mMidiLearnParamIdx = -1;
			}
		}

		mMidiQueue.Add(pMsg);
	}
}

void Melodizer::SetControlChangeForParam(const IMidiMsg::EControlChangeMsg cc, const int paramIdx)
{
	mControlChangeForParam[paramIdx] = cc;
#if SA_API
	char ccString[100];
	sprintf(ccString, "%u", (unsigned)cc);
	WritePrivateProfileString(kMidiControlIni, GetParam(paramIdx)->GetNameForHost(), ccString, gINIPath);
#endif
}

void Melodizer::SetSamplesPerBeat(const double tempo)
{
	// one "beat" is two steps, which allows us to use kShuffle directly to calculate mSampleCount.
	unsigned int samplesPerBeat = (unsigned int)(GetSampleRate() * 60.0 / tempo);
	switch (GetParam(kStepLength)->Int())
	{
	case SL_4:  samplesPerBeat *= 2; break;
	case SL_8:  break;
	case SL_16: samplesPerBeat /= 2; break;
	case SL_32: samplesPerBeat /= 4; break;
	case SL_64: samplesPerBeat /= 8; break;

	case SL_4T:  samplesPerBeat = samplesPerBeat * 2 * 2 / 3; break;
	case SL_8T:  samplesPerBeat = samplesPerBeat * 2 / 3; break;
	case SL_16T: samplesPerBeat = samplesPerBeat / 2 * 2 / 3; break;
	case SL_32T: samplesPerBeat = samplesPerBeat / 4 * 2 / 3; break;
	case SL_64T: samplesPerBeat = samplesPerBeat / 8 * 2 / 3; break;

	case SL_4D:  samplesPerBeat = samplesPerBeat * 2 * 6 / 4; break;
	case SL_8D:  samplesPerBeat = samplesPerBeat * 6 / 4; break;
	case SL_16D: samplesPerBeat = samplesPerBeat / 2 * 6 / 4; break;
	case SL_32D: samplesPerBeat = samplesPerBeat / 4 * 6 / 4; break;
	case SL_64D: samplesPerBeat = samplesPerBeat / 8 * 6 / 4; break;
	}

	mSamplesPerBeat = samplesPerBeat;
}

float Melodizer::CalcDelayDuration(const StepLength stepLength, const double tempo)
{
	float beatDuration = 60.0 / tempo;
	switch (stepLength)
	{
	case SL_4:  break;
	case SL_8:  beatDuration /= 2; break;
	case SL_16: beatDuration /= 4; break;
	case SL_32: beatDuration /= 8; break;
	case SL_64: beatDuration /= 16; break;

	case SL_4T:  beatDuration = beatDuration * 2 / 3; break;
	case SL_8T:  beatDuration = beatDuration / 2 / 3; break;
	case SL_16T: beatDuration = beatDuration / 4 * 2 / 3; break;
	case SL_32T: beatDuration = beatDuration / 8 * 2 / 3; break;
	case SL_64T: beatDuration = beatDuration / 16 * 2 / 3; break;

	case SL_4D:  beatDuration = beatDuration * 1.5f; break;
	case SL_8D:  beatDuration = beatDuration / 2 * 1.5f; break;
	case SL_16D: beatDuration = beatDuration / 4 * 1.5f; break;
	case SL_32D: beatDuration = beatDuration / 8 * 1.5f; break;
	case SL_64D: beatDuration = beatDuration / 16 * 1.5f; break;
	}

	return beatDuration;
}

void Melodizer::SetDelayDuration(const double tempo, const double crossfade)
{
	IParam* delayParam = GetParam(kDelayDuration);
	int delayParamValue = delayParam->Int();
	float delayDuration = CalcDelayDuration((StepLength)delayParamValue, tempo);

	// if our Delay Duration param would create a longer delay than we can handle
	// we need to change it to largest value that generates a delay we *can* handle.
	while (delayDuration > kDelayDurationMaxSeconds)
	{
		// shorter durations are smaller values in the enum
		--delayParamValue;
		delayDuration = CalcDelayDuration((StepLength)delayParamValue, tempo);
	}

	// we need to change it, so inform the host and the UI.
	// we don't call OnParamChange from here because 
	// this function is called from OnParamChange for both kTempo and kDelayDuration.
	if (delayParam->Int() != delayParamValue)
	{
		BeginInformHostOfParamChange(kDelayDuration);
		delayParam->Set(delayParamValue);
		InformHostOfParamChange(kDelayDuration, delayParam->GetNormalized());
		EndInformHostOfParamChange(kDelayDuration);
		GetGUI()->SetParameterFromPlug(kDelayDuration, delayParamValue, false);
	}

	if (mDelayCrossfade.getAmp() < 0.001f )
	{
		Trace(TRACELOC, "Crossfade to Delay B");
		mDelayB.delTime.setLastValue(delayDuration);
		mDelayCrossfade.activate(crossfade, 0, 1);
	}
	else
	{
		Trace(TRACELOC, "Crossfade to Delay A");
		mDelayA.delTime.setLastValue(delayDuration);
		mDelayCrossfade.activate(crossfade, 1, 0);
	}
}

void Melodizer::ProcessSysEx(ISysEx* pSysEx)
{
#ifdef TRACER_BUILD
	pSysEx->LogMsg();
#endif
	if (pSysEx->IsMMC())
	{
		switch (pSysEx->MMCCommand())
		{
		case ISysEx::kPlay: SetPlayStateFromMidi(PS_Play); break;
		case ISysEx::kPause: SetPlayStateFromMidi(PS_Pause); break;
		case ISysEx::kStop: SetPlayStateFromMidi(PS_Stop); break;
		}
	}
}

void Melodizer::Reset()
{
	TRACE;
	IMutexLock lock(this);

	mMidiQueue.Resize(GetBlockSize());

#if SA_API
	for (int i = 0; i < kNumParams; ++i)
	{
		mControlChangeForParam[i] = (IMidiMsg::EControlChangeMsg)GetPrivateProfileInt(kMidiControlIni, GetParam(i)->GetNameForHost(), 0, gINIPath);
	}
#endif

	// reseed the random generator
	OnParamChange(kSeed);

	// initialize the sequencer
	StopSequencer();

	mMelodyVolume.setAudioChannelCount(2);
	mMelodyVolume.setSampleRate(GetSampleRate());
	mDelayCrossfade.setSampleRate(GetSampleRate());
	mDelayDryMix.setSampleRate(GetSampleRate());
}

void Melodizer::StopSequencer()
{
	mTick = -1;
	mPreviousNoteIndex = 0;
	mSampleCount = 0;
	mOddTick = false;

	mActiveTone = 0;
	for (int i = 0; i < mTones.size(); ++i)
	{
		mTones[i]->noteOff();
	}
}

void Melodizer::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	const IParam* param = GetParam(paramIdx);
	switch (paramIdx)
	{
	case kVolume:
	{
		const float volume = GetParam(kVolume)->DBToAmp();
		mMelodyVolumeLine.activate(0.001f, mMelodyVolume.amplitude.getLastValue(), volume);
	}
	break;

	case kClockSource:
	{
		mInterface.OnClockSourceChanged(param->Int());
	}
	// fall thru so tempo gets set

	case kTempo:
	{
		float tempo = mTempo;
		switch (GetParam(kClockSource)->Int())
		{
		case CS_Internal:
			tempo = GetParam(kTempo)->Value();
			break;

		case CS_External:
			tempo = GetTempo();
			// we call SetParameterFromPlug here because we will still get OnParamChanged
			// calls if a user has automated Tempo, but has ClockSource set to Ext.
			// So we need make sure that the UI is always displaying the correct thing.
			GetGUI()->SetParameterFromPlug(kTempo, mTempo, false);
			break;
		}		

		if (tempo != mTempo)
		{
			mTempo = tempo;
			SetSamplesPerBeat(mTempo);
			mCrossfadeDelays = true;			
		}
	}
	break;

	case kStepLength:
	{
		SetSamplesPerBeat(mTempo);
	}
	break;

	case kDelayDuration:
	{
		mCrossfadeDelays = true;
	}
	break;

	case kDelayFeedback:
	{
		const float feed = GetParam(kDelayFeedback)->Value() / 100;
		mDelayFeedback.activate(0.01f, mDelayFeedback.getAmp(), feed);
	}
	break;

	case kDelayMix:
	{
		const double mix = GetParam(kDelayMix)->Value() / 100.0;
		// mix [0.5,1] => dry [1,0]
		const double dry = BOUNDED(2 - mix * 2, 0, 1);
		// mix [0, 0.5] => wet [0,1]
		const double wet = BOUNDED(mix * 2, 0, 1);
		mDelayDryMix.activate(0.001f, mDelayDryMix.getAmp(), dry);
		mDelayWetMix.activate(0.001f, mDelayWetMix.getAmp(), wet);
	}
	break;

	case kFlangerTime:
		mFlangerTime.activate(0.2f, mFlanger.delay.getLastValue(), param->Value());
		// fallthru cuz depth is relative to time

	case kFlangerDepth:
	{
		//const float amt = kFlangerDepthMinMs + (kFlangerDepthMaxMs - kFlangerDepthMinMs)*(param->Value() / 100);
		const float amt = GetParam(kFlangerTime)->Value() * (GetParam(kFlangerDepth)->Value() / 100);
		mFlangerDepth.activate(0.2f, mFlanger.depth.getLastValue(), amt);
	}
	break;

	case kFlangerRate:
		mFlanger.rate.setLastValue(param->Value());
		break;
	
	case kFlangerFeedback:
		mFlanger.feedback.setLastValue(param->Value() / 100);
		break;

	case kFlangerMix:
	{
		const double mix = param->Value() / 100.0;
		// mix [0.5,1] => dry [1,0]
		const double dry = BOUNDED(2 - mix * 2, 0, 1);
		// mix [0, 0.5] => wet [0,1]
		const double wet = BOUNDED(mix * 2, 0, 1);
		mFlanger.dry.setLastValue(dry);
		mFlanger.wet.setLastValue(wet);
	}
	break;


	case kWaveform:
		mWaveFormIdx = GetParam(kWaveform)->Int();
		break;

	case kKey:
		mKeyIdx = GetParam(kKey)->Int();
		break;

	case kOctave:
		mLowOctave = GetParam(kOctave)->Int() + 1;
		// fallthru because hi octave is relative to low octave
	case kRange:
		mHiOctave = mLowOctave + GetParam(kRange)->Int();
		break;

	case kScale:
		mPreviousNoteIndex = 0;
		mScaleIdx = GetParam(kScale)->Int();
		break;

	case kSeed:
	{
		// when Seed is zero, we use a random seed, other wise we seed with the param
		const int seedParam = GetParam(kSeed)->Int();
		if (seedParam == 0)
		{
			std::random_device rd;
			mRandomGen.seed(rd());
		}
		else
		{
			mRandomGen.seed(seedParam);
		}

		// we want to have deterministic output,
		// which means we should start from the beginning of the sequence.
		mTick = -1;
		mPreviousNoteIndex = 0;
	}
	break;
	
	case kPlayState:
		{
			const PlayState state = (PlayState)param->Int();
			if (state == PS_Stop)
			{
				StopSequencer();
			}
			else if ( state == PS_Pause )
			{
				mTones[mActiveTone]->noteOff();
			}
			mPlayState = state;
		}
		break;

	case kProbabilityRandomize:
	case kPanRandomize:
	case kVelocityRandomize:
	case kAttackRandomize:
	case kDecayRandomize:
	case kSustainRandomize:
	case kReleaseRandomize:
		if (param->Int() == 1)
		{
			std::uniform_real_distribution<> dist(0, 1);
			for (int i = 0; i < kSequencerSteps; ++i)
			{
				int pidx = paramIdx - kSequencerSteps + i;
				BeginInformHostOfParamChange(pidx);
				double normValue = dist(mRandomGen);
				GetParam(pidx)->SetNormalized(normValue);
				GetGUI()->SetParameterFromPlug(pidx, normValue, true);
				InformHostOfParamChange(pidx, normValue);
				EndInformHostOfParamChange(pidx);
			}
		}
		break;

	default:
		break;
	}
}

void Melodizer::HandleMidiControlChange(IMidiMsg* pMsg)
{
	const IMidiMsg::EControlChangeMsg cc = pMsg->ControlChangeIdx();
	for (int i = 0; i < kNumParams; ++i)
	{
		if (mControlChangeForParam[i] == cc)
		{
			const double value = pMsg->ControlChange(cc);
			GetParam(i)->SetNormalized(value);
			OnParamChange(i);
			GetGUI()->SetParameterFromPlug(i, GetParam(i)->GetNormalized(), true);
		}
	}
}

void Melodizer::GenerateNote( int tick,
                          unsigned int waveformIdx, 
                          const Scale* notes, 
                          unsigned int key, 
                          int lowOctave, 
                          int hiOctave,
                          unsigned int& previousNote
                         )
{
	int nextNote = previousNote;
	// figure out next note
	{
		int listLen = 0;
		const int* nextNoteList = notes->notes[previousNote];
		// look for the terminating 0 in this note list
		while ( listLen < 12 && nextNoteList[listLen] > 0)
		{
			++listLen;
		}
		if (listLen > 0)
		{
			// we subtract one from the number in the list
			// because notes stored with C = 1
			nextNote = nextNoteList[RandomRange(0, listLen - 1)] - 1;
		}
		else // return to the root if for some reason we jumped to a scale degree with no next notes
		{
			nextNote = 0;
		}
	}
	int baseNote = nextNote + key;
	int octave = RandomRange(lowOctave, hiOctave-1);
    int note = baseNote + octave * 12;
	const float pulseWidth = GetParam(kPulseWidth)->Value();
	const float fromFreq = mTones[mActiveTone]->getFrequency();
    const float toFreq 	= Frequency::ofMidiNote( note ).asHz();
	const float glide   = GetParam(kGlide)->Value();
	const float amp 	= GetParam(kVelocityFirst + tick)->Value() / 100;
	const float fromPan = mTones[mActiveTone]->getPan();
	const float toPan 	= GetParam(kPanFirst + tick)->Value() * GetParam(kWidth)->Value() / 100;
	const float panDur = GetParam(kMovement)->Value();
	const float attack  = GetParam(kEnvAttack)->Value()  * GetParam(kAttackFirst + tick)->Value() / 100;
	const float decay   = GetParam(kEnvDecay)->Value()   * GetParam(kDecayFirst + tick)->Value() / 100;
	const float sustain = GetParam(kEnvSustain)->Value() * GetParam(kSustainFirst + tick)->Value() / 100;
	const float release = GetParam(kEnvRelease)->Value() * GetParam(kReleaseFirst + tick)->Value() / 100;
	
	// move to the next active tone
	const int voices = GetParam(kVoices)->Int();
	if ( voices > 1 )
	{
		mActiveTone = (mActiveTone+1)%voices;
	}
	mTones[mActiveTone]->setPulseWidth(pulseWidth);
	mTones[mActiveTone]->noteOn(mWaveforms[waveformIdx], tick, fromFreq, toFreq, glide, amp, attack, decay, sustain, release, fromPan, toPan, panDur);

    previousNote = nextNote;    
}

int Melodizer::RandomRange(int low, int hi)
{
	std::uniform_int_distribution<> dist(low, hi);
	return dist(mRandomGen);
}

float Melodizer::RandomRange(float low, float hi)
{
	std::uniform_real_distribution<> dist(low, hi);
	return dist(mRandomGen);
}

void Melodizer::SetPlayStateFromMidi(PlayState state)
{
	IParam* param = GetParam(kPlayState);
	if ((PlayState)param->Int() != state)
	{
		const double normValue = param->GetNormalized(state);
		BeginInformHostOfParamChange(kPlayState);		
		SetParameterFromGUI(kPlayState, normValue);
		EndInformHostOfParamChange(kPlayState);
		GetGUI()->SetParameterFromPlug(kPlayState, normValue, true);
	}
}
