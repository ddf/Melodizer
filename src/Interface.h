#pragma once

#include "IPlugStructs.h"
#include <vector>

class Melodizer;
class IGraphics;
class LED;
class IControl;
class ITextControl;
class KnobLineCoronaControl;

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

	// called by the plug whenever its play state changes
	void OnPlayStateChanged();

	// called by the plug when the Scale changes to indicate if the UI should change to Fingered mode
	void SetFingeredMode(bool enabled);

private:	
	IControl* AttachEnum(IGraphics* pGraphics, IRECT rect, const int paramIdx, const char * label = nullptr);
	IControl* AttachTextBox(IGraphics* pGraphics, IRECT rect, const int paramIdx, const float scrollSpeed, const char * maxValue, const char * label = nullptr);
	KnobLineCoronaControl* AttachKnob(IGraphics* pGraphics, IRECT rect, const int paramIdx, const char * label = nullptr);
	void AttachStepRowLabel(IGraphics* pGraphics, int rowNum, const char * name);
	void AttachStepRowRandomizer(IGraphics* pGraphics, int rowNum, const int param);

	Melodizer* mPlug;

	// control that is displayed under knobs in the sequence section while they are being manipulated by the mouse
	ITextControl* mSequenceKnobValueControl;

	IControl* mPresetControl;
	// we hold on to this so we can disable it when clock source is not Internal
	IControl* mTempoControl;

	IControl* mKeyControl;
	IControl* mOctaveControl;
	IControl* mRangeControl;

	IControl* mPlayStateControl;

	// bank of LED graphics above all the knobs
	std::vector<LED*> mLEDs;
	std::vector<LED*> mNoteOns;	
};

