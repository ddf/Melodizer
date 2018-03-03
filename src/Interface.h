#pragma once

#include "Params.h"
#include "IPlugStructs.h"
#include <vector>

class Melodizer;
class IGraphics;
class LED;

class Interface
{
public:
	Interface(Melodizer* inPlug);
	~Interface();

	void CreateControls(IGraphics* pGraphics);

	// called by the plug when the next triggered 16th note happens.
	// we use this to blink the appropriate LED
	void OnTick(const unsigned int tick, bool noteOn);

private:	
	void AttachKnob(IGraphics* pGraphics, IRECT rect, EParams paramIdx, const char * label = nullptr);
	void AttachStepRowLabel(IGraphics* pGraphics, int rowNum, const char * name);
	void AttachStepRowRandomizer(IGraphics* pGraphics, int rowNum, EParams param);

	Melodizer* const mPlug;

	// bank of LED graphics above all the knobs
	std::vector<LED*> mLEDs;
	std::vector<LED*> mNoteOns;	
};

