#include "Melodizer.h"
#include "IPlug_include_in_plug_src.h"
#include "resource.h"
#include "Params.h"

#include "Waves.h"
#include "Instruments.h" // contains Tone

using namespace Minim;

const int kNumPrograms = 1;

Melodizer::Melodizer(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
	, mInterface(this)
	, mTick(0)
	, mSampleCount(0)
	, mMelodyBus()
	, mRandomGen(0)
	, mRandomDist(0, 100)
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
		param->InitEnum("Waveform", WT_Saw, WT_Count);

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

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  char paramName[32];
  const int toneCount = kProbabilityLast - kProbabilityFirst + 1;
  for (int i = 0; i < toneCount; ++i)
  {
	  sprintf(paramName, "Probability %d", i);
	  const int paramIdx = kProbabilityFirst + i;
	  GetParam(paramIdx)->InitDouble(paramName, 50, 0, 100, 1, "%");

	  mTones.push_back(new Tone(mMelodyBus));
  }

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
	const unsigned int samplesPerTick = (unsigned int)GetSamplesPerBeat() / 2;
	const unsigned int waveformIdx = GetParam(kWaveform)->Int();

	double* out1 = outputs[0];
	double* out2 = outputs[1];

	float result[2];
	for (int s = 0; s < nFrames; ++s, ++out1, ++out2)
	{
		mMelodyBus.tick(result, 2);
		*out1 = result[0];
		*out2 = result[1];

		--mSampleCount;

		if (mSampleCount == 0)
		{
			mTones[mTick]->noteOff();

			mTick = (mTick + 1) % 16;
			mSampleCount = samplesPerTick;

			const double prob = GetParam(kProbabilityFirst + mTick)->Value();
			if (mRandomDist(mRandomGen) <= prob)
			{				
				mInterface.OnTick(mTick);
				mTones[mTick]->noteOn(0.5f, mWaveforms[waveformIdx], mTick, 440, 1, 0);
			}
		}
	}
}

void Melodizer::BeginMIDILearn(int param1, int param2, int x, int y)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Melodizer::Reset()
{
  TRACE;
  IMutexLock lock(this);

  // reseed the random generator
  std::random_device rd;
  mRandomGen.seed(rd());

  mTick = 0;
  mSampleCount = (unsigned int)GetSamplesPerBeat() / 2;
  mInterface.OnTick(0);
  mMelodyBus.setAudioChannelCount(2);
  mMelodyBus.setSampleRate(GetSampleRate());
}

void Melodizer::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    default:
      break;
  }
}
