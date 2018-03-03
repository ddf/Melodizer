#pragma  once

#include "IPlug_include_in_plug_hdr.h"

#include "Params.h"
#include "Interface.h"

#include <vector>
#include <random>

#include "Summer.h"

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
  void OnParamChange(int paramIdx) override;
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames) override;

  void BeginMIDILearn(int param1, int param2, int x, int y);

private:
	void GenerateNote(int tick, unsigned int waveformIdx, const Scale* notes, unsigned int key, int lowOctave, int hiOctave, float panRange, unsigned int& previousNoteIndex);
	int RandomRange(int low, int hi);
	float RandomRange(float low, float hi);

	Interface mInterface;

	// count how many samples so we know when to increment mTick
	unsigned long int mSampleCount;
	// is the tick even or odd, for the purposes of adjust sample count with shuffle
	bool mOddTick;
	unsigned int mTick;
	unsigned int mPreviousNoteIndex;

	// random number generation for testing against probabilities and generating random numbers for tones, used by RandomRange methods
	std::default_random_engine mRandomGen;

	Minim::Summer mMelodyBus;

	// we'll have one Tone for each tick that will be reconfigured before being triggered
	std::vector<Tone*> mTones;

	// we'll have a number of Waveforms to choose from
	std::vector<Minim::Waveform*> mWaveforms;
};
