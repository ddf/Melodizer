#pragma once

#include "IPlugStructs.h"
#include <vector>

class Melodizer;
class IGraphics;
class LED;
class IControl;

class Interface
{
public:
	Interface(Melodizer* inPlug);
	~Interface();

	void CreateControls(IGraphics* pGraphics);

	// called by the plug when the next triggered 16th note happens.
	// we use this to blink the appropriate LED
	void OnTick(const unsigned int tick, bool noteOn);

	// call by the plug when ClockSource changes
	void OnClockSourceChanged(const int source);

	// called by the plug when a preset is loaded or saved
	void OnPresetChanged();

private:	
	IControl* AttachEnum(IGraphics* pGraphics, IRECT rect, const int paramIdx, const char * label = nullptr);
	IControl* AttachTextBox(IGraphics* pGraphics, IRECT rect, const int paramIdx, const float scrollSpeed, const char * maxValue, const char * label = nullptr);
	void AttachKnob(IGraphics* pGraphics, IRECT rect, const int paramIdx, const char * label = nullptr);
	void AttachStepRowLabel(IGraphics* pGraphics, int rowNum, const char * name);
	void AttachStepRowRandomizer(IGraphics* pGraphics, int rowNum, const int param);

	Melodizer* const mPlug;

	IControl* mPresetControl;
	// we hold on to this so we can disable it when clock source is not Internal
	IControl* mTempoControl;

	// bank of LED graphics above all the knobs
	std::vector<LED*> mLEDs;
	std::vector<LED*> mNoteOns;	
	
};

