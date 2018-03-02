#pragma  once

#include "IPlug_include_in_plug_hdr.h"
#include "Interface.h"

#include <vector>
#include <random>

#include "Summer.h"

class Tone;

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
  void OnParamChange(int paramIdx) override;
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames) override;

  void BeginMIDILearn(int param1, int param2, int x, int y);

private:
	Interface mInterface;

	// count how many samples so we know when to increment mTick
	unsigned long int mSampleCount;
	unsigned int mTick;

	// random number generation for testing against probabilities
	std::default_random_engine		 mRandomGen;
	std::uniform_real_distribution<> mRandomDist;



	Minim::Summer mMelodyBus;

	// we'll have one Tone for each tick that will be reconfigured before being triggered
	std::vector<Tone*> mTones;

	// we'll have a number of Waveforms to choose from
	std::vector<Minim::Waveform*> mWaveforms;
};
