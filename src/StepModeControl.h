#pragma once

#include "IControl.h"

class StepModeControl : public IControl
{
public:
	StepModeControl(IPlugBase* pPlug, IRECT iRect, const int paramIdx, IText* textStyle);
	
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDown(int x, int y, IMouseMod* mod) override;
	
private:
	IRECT mNormRect;
	IRECT mSkipRect;
	IRECT mLoopRect;
};
