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
	, mRandomGen(0)
{
	TRACE;

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
			case WT_Quarterpulse:
				param->SetDisplayText(i, "PULSE");
				mWaveforms.push_back(Waves::QUARTERPULSE());
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

	// tempo
	{
		GetParam(kTempo)->InitDouble("Tempo", DEFAULT_TEMPO, kTempoMin, kTempoMax, 0.01, "bpm");
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

			sprintf(paramName, "Step %d Attack", i);
			GetParam(kAttackFirst + i)->InitDouble(paramName, 100, 1, 100, percentStep, "%");

			sprintf(paramName, "Step %d Decay", i);
			GetParam(kDecayFirst + i)->InitDouble(paramName, 100, 1, 100, percentStep, "%");

			sprintf(paramName, "Step %d Sustain", i);
			GetParam(kSustainFirst + i)->InitDouble(paramName, 100, 1, 100, percentStep, "%");

			sprintf(paramName, "Step %d Release", i);
			GetParam(kReleaseFirst + i)->InitDouble(paramName, 100, 1, 100, percentStep, "%");

			mTones.push_back(new Tone(mMelodyBus));
		}
	}

	// randomizers
	{
		GetParam(kProbabilityRandomize)->InitEnum("Probability Randomize", 0, 2);
		GetParam(kProbabilityRandomize)->SetDisplayText(0, "Off");
		GetParam(kProbabilityRandomize)->SetDisplayText(1, "On");
	}

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

void Melodizer::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	// Mutex is already locked for us.
	// one beat is two ticks, which allows us to use kShuffle directly to calculate mSampleCount.
	const unsigned int samplesPerBeat = (unsigned int)(GetSampleRate() * 60.0 / GetParam(kTempo)->Value());
	const unsigned int waveformIdx = GetParam(kWaveform)->Int();
	const unsigned int scaleIdx = GetParam(kScale)->Int();
	const unsigned int keyIdx = GetParam(kKey)->Int();
	const unsigned int lowOctave = GetParam(kOctave)->Int();
	const unsigned int hiOctave = lowOctave + GetParam(kRange)->Int();

	double* out1 = outputs[0];
	double* out2 = outputs[1];

	float result[2];
	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		if (mSampleCount == 0)
		{
			mTones[mTick]->noteOff();

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
				GenerateNote(mTick, waveformIdx, Scales[scaleIdx], keyIdx, lowOctave, hiOctave, 0, mPreviousNoteIndex);
			}
		}

		mMelodyBus.tick(result, 2);
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

	// reseed the random generator
	std::random_device rd;
	mRandomGen.seed(rd());

	mTick = 0;
	mPreviousNoteIndex = 0;
	mSampleCount = 0;
	mOddTick = false;
	mMelodyBus.setAudioChannelCount(2);
	mMelodyBus.setSampleRate(GetSampleRate());
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

	case kProbabilityRandomize:
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
                          float panRange, 
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
    float freq = Frequency::ofMidiNote( note ).asHz();
	float amp = RandomRange(0.41f, 0.61f);
    float pan = 0.f;
    
    if ( panRange != 0 )
    {
        pan = RandomRange(0.f, panRange) + 0.2f;
        if ( RandomRange(0.f, 1.f) < 0.5f )
        {
            pan *= -1;
        }
		pan = RandomRange(-pan, pan);
    }
    
	const float attack  = GetParam(kEnvAttack)->Value()  * GetParam(kAttackFirst + tick)->Value() / 100;
	const float decay   = GetParam(kEnvDecay)->Value()   * GetParam(kDecayFirst + tick)->Value() / 100;
	const float sustain = GetParam(kEnvSustain)->Value() * GetParam(kSustainFirst + tick)->Value() / 100;
	const float release = GetParam(kEnvRelease)->Value() * GetParam(kReleaseFirst + tick)->Value() / 100;
    
	mTones[tick]->noteOn(mWaveforms[waveformIdx], tick, freq, amp, attack, decay, sustain, release, pan);

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
