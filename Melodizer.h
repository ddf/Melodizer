#pragma  once

#include "IPlug_include_in_plug_hdr.h"
#include "Interface.h"

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
};
