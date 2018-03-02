#pragma once

#include "IControl.h"

class EnumControl : public IControl
{
public:
	EnumControl(IPlugBase* pPlug, IRECT rect, int paramIdx, IText* textStyle);

	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDown(int x, int y, IMouseMod* pMod) override;
private:
	IRECT mTextRect;
};