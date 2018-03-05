#pragma once

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
	void AttachEnum(IGraphics* pGraphics, IRECT rect, const int paramIdx, const char * label = nullptr);
	void AttachTextBox(IGraphics* pGraphics, IRECT rect, const int paramIdx, const char * maxValue, const char * label = nullptr);
	void AttachKnob(IGraphics* pGraphics, IRECT rect, const int paramIdx, const char * label = nullptr);
	void AttachStepRowLabel(IGraphics* pGraphics, int rowNum, const char * name);
	void AttachStepRowRandomizer(IGraphics* pGraphics, int rowNum, const int param);

	Melodizer* const mPlug;

	// bank of LED graphics above all the knobs
	std::vector<LED*> mLEDs;
	std::vector<LED*> mNoteOns;	
	
};

