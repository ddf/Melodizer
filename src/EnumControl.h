#pragma once

#include "IControl.h"

class EnumControl : public IControl
{
public:
	EnumControl(IPlugBase* pPlug, IRECT rect, int paramIdx, IText* textStyle);

	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDown(int x, int y, IMouseMod* pMod) override;
	void OnMouseWheel(int x, int y, IMouseMod* pMod, int d) override;
private:

	void StepValue(int amount);

	IRECT mTextRect;
	IRECT mDecrementRect;
	IRECT mIncrementRect;
};