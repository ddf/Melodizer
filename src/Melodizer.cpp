#include "Melodizer.h"
#include "IPlug_include_in_plug_src.h"
#include "resource.h"
#include "Params.h"

#include "Scales.h"

#include "Waves.h"
#include "Instruments.h" // contains Tone

using namespace Minim;

const int kNumPrograms = 1;

Melodizer::Melodizer(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
	, mInterface(this)
	, mTick(0)
	, mPreviousNoteIndex(0)
	, mSampleCount(0)
	, mOddTick(false)
	, mMelodyBus()
	, mMelodyVolume(0)
	, mMelodyVolumeLine(0,0,0)
	, mRandomGen(0)
	, mTones(kVoicesMax)
	, mActiveTone(0)
{
	TRACE;
	
	// setup dsp chain
	{
		mMelodyVolume.setAudioChannelCount(2);
		mMelodyVolume.setSampleRate(44100);
		mMelodyBus.patch( mMelodyVolume );
		mMelodyVolumeLine.patch( mMelodyVolume.amplitude );
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

		IParam* param = GetParam(kWaveform);
		param->InitEnum("Waveform", WT_Sine4, WT_Count);

		for (int i = 0; i < WT_Count; ++i)
		{
			WaveformType type = (WaveformType)i;
			switch (type)
			{
			case WT_Sine:
				param->SetDisplayText(i, "SIN");
				mWaveforms.push_back(Waves::SINE());
				break;
			case WT_Triangle:
				param->SetDisplayText(i, "TRI");
				mWaveforms.push_back(Waves::TRIANGLE());
				break;
			case WT_Saw:
				param->SetDisplayText(i, "SAW");
				mWaveforms.push_back(Waves::SAW());
				break;
			case WT_Square:
				param->SetDisplayText(i, "SQR");
				mWaveforms.push_back(Waves::SQUARE());
				break;
			case WT_Sine4:
				param->SetDisplayText(i, "SIN4");
				mWaveforms.push_back(Waves::gen10(kWaveformLength, harmonics, 4));
				break;
			case WT_Sine8:
				param->SetDisplayText(i, "SIN8");
				mWaveforms.push_back(Waves::gen10(kWaveformLength, harmonics, 8));
				break;
			case WT_Sine16:
				param->SetDisplayText(i, "SIN16");
				mWaveforms.push_back(Waves::gen10(kWaveformLength, harmonics, 16));
				break;
			case WT_Sine32:
				param->SetDisplayText(i, "SIN32");
				mWaveforms.push_back(Waves::gen10(kWaveformLength, harmonics, 32));
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
			param->SetDisplayText(i, Scales[i]->scaleName);
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
		GetParam(kTempo)->InitDouble("Tempo", DEFAULT_TEMPO, kTempoMin, kTempoMax, 0.01, "bpm");
		IParam* param = GetParam(kStepLength);
		param->InitEnum("Step Length", SL_8, SL_Count);
		for(int i = 0; i < SL_Count; ++i)
		{
			switch(i)
			{
				case SL_4: param->SetDisplayText(i, "1/4"); break;
				case SL_8: param->SetDisplayText(i, "1/8"); break;
				case SL_16: param->SetDisplayText(i, "1/16"); break;
				case SL_32: param->SetDisplayText(i, "1/32"); break;
				case SL_64: param->SetDisplayText(i, "1/64"); break;
			}
		}
	}
	
	// octave + range
	{
		GetParam(kOctave)->InitInt("Octave", 4, 0, 8);
		GetParam(kRange)->InitInt("Range", 0, 0, 8);
	}

	// shuffle
	{
		GetParam(kShuffle)->InitDouble("Shuffle", 0.5, 0.1, 0.9, 0.01f);
	}

	// seed
	{
		GetParam(kSeed)->InitInt("Seed", 0, 0, 32768);
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

	// knob bank
	{
		//arguments are: name, defaultVal, minVal, maxVal, step, label
		char paramName[32];
		const double percentStep = 0.5f;
		for (int i = 0; i < kSequencerSteps; ++i)
		{
			sprintf(paramName, "Step Mode %d", i);
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
	MakeDefaultPreset((char *) "-", kNumPrograms);
}

Melodizer::~Melodizer()
{
	for (int i = 0; i < mWaveforms.size(); ++i)
	{
		delete mWaveforms[i];
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
	// Mutex is already locked for us.
	// one beat is two steps, which allows us to use kShuffle directly to calculate mSampleCount.
	unsigned int samplesPerBeat = (unsigned int)(GetSampleRate() * 60.0 / GetParam(kTempo)->Value());
	switch(GetParam(kStepLength)->Int())
	{
		case SL_4:  samplesPerBeat *= 2; break;
		case SL_16: samplesPerBeat /= 2; break;
		case SL_32: samplesPerBeat /= 4; break;
		case SL_64: samplesPerBeat /= 8; break;
	}
	const unsigned int waveformIdx = GetParam(kWaveform)->Int();
	const unsigned int scaleIdx = GetParam(kScale)->Int();
	const unsigned int keyIdx = GetParam(kKey)->Int();
	const unsigned int lowOctave = GetParam(kOctave)->Int();
	const unsigned int hiOctave = lowOctave + GetParam(kRange)->Int();
	
	// change the volume smoothly over the course of this output frame
	const float volume = GetParam(kVolume)->DBToAmp();
	const float frameDuration = (float)nFrames / GetSampleRate();
	mMelodyVolumeLine.activate(frameDuration, mMelodyVolume.amplitude.getLastValue(), volume);

	double* out1 = outputs[0];
	double* out2 = outputs[1];

	float result[2];
	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		if (mSampleCount == 0)
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
				mSampleCount = (unsigned long)round(samplesPerBeat*GetParam(kShuffle)->Value());
			}
			else
			{
				mSampleCount = (unsigned long)round(samplesPerBeat*(1.0 - GetParam(kShuffle)->Value()));
			}

			mOddTick = !mOddTick;

			const bool noteOn = RandomRange(0.f, 100.f) <= GetParam(kProbabilityFirst + mTick)->Value();
			mInterface.OnTick(mTick, noteOn);
			if (noteOn)
			{
				GenerateNote(mTick, waveformIdx, Scales[scaleIdx], keyIdx, lowOctave, hiOctave, mPreviousNoteIndex);
			}
		}

		mMelodyVolume.tick(result, 2);
		*out1 = result[0];
		*out2 = result[1];

		--mSampleCount;
	}
}

void Melodizer::BeginMIDILearn(int param1, int param2, int x, int y)
{
}

void Melodizer::Reset()
{
	TRACE;
	IMutexLock lock(this);

	// reseed the random generator.
	// when Seed is zero, we use a random seed, other wise we seed with the param
	const int seedParam = GetParam(kSeed)->Int();
	if ( seedParam == 0)
	{
		std::random_device rd;
		mRandomGen.seed(rd());
	}
	else
	{
		mRandomGen.seed(seedParam);
	}

	mTick = 0;
	mPreviousNoteIndex = 0;
	mSampleCount = 0;
	mOddTick = false;
	mActiveTone = 0;
	for(int i = 0; i < mTones.size(); ++i)
	{
		mTones[i]->noteOff();
	}
	mMelodyVolume.setAudioChannelCount(2);
	mMelodyVolume.setSampleRate(GetSampleRate());
}

void Melodizer::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	const IParam* param = GetParam(paramIdx);
	switch (paramIdx)
	{
	case kScale:
		mPreviousNoteIndex = 0;
		break;

	case kSeed:
		// reset the plug when the seed changes because 
		// we want to have deterministic output,
		// which means we should start from the beginning of the sequence.
		Reset();
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

void Melodizer::GenerateNote( int tick,
                          unsigned int waveformIdx, 
                          const Scale* notes, 
                          unsigned int key, 
                          int lowOctave, 
                          int hiOctave,
                          unsigned int& previousNoteIndex
                         )
{
	int nextNoteIndex = previousNoteIndex;
	// figure out next note
	{
		int listLen = 1;
		const int* nextNoteList = notes->scale[previousNoteIndex];
		while (nextNoteList[listLen] != -1)
		{
			++listLen;
		}
		nextNoteIndex = nextNoteList[RandomRange(1, listLen-1)];
	}
	int baseNote = notes->scale[nextNoteIndex][0] + key;
	int octave = RandomRange(lowOctave, hiOctave);
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

    previousNoteIndex = nextNoteIndex;    
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
