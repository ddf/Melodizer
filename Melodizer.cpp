#include "Melodizer.h"
#include "IPlug_include_in_plug_src.h"
#include "resource.h"
#include "Params.h"

const int kNumPrograms = 1;

Melodizer::Melodizer(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
	, mInterface(this)
	, mTick(0)
{
  TRACE;

  char paramName[32];

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  for (int i = kProbabilityFirst; i <= kProbabilityLast; ++i)
  {
	  sprintf(paramName, "Probability %d", i - kProbabilityFirst);
	  GetParam(i)->InitDouble(paramName, 50, 0, 100, 1, "%");
  }

  IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
  mInterface.CreateControls(pGraphics);
  AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
}

Melodizer::~Melodizer() {}

void Melodizer::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	// Mutex is already locked for us.
	const unsigned int samplesPerTick = (unsigned int)GetSamplesPerBeat() / 2;

	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
	{
		*out1 = *in1;
		*out2 = *in2;

		++mSampleCount;

		if (mSampleCount == samplesPerTick)
		{
			mTick = (mTick + 1) % 16;
			mInterface.OnTick(mTick);
			mSampleCount = 0;
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

  mTick = 0;
  mSampleCount = 0;
  mInterface.OnTick(0);
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
