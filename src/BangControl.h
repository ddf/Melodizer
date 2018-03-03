#pragma once

#include "IControl.h"

// based on IContactControl, but without the need for a bitmap
class BangControl : public IControl
{
public:
	BangControl(IPlugBase* pPlug, IRECT iRect, int paramIdx, IColor onColor, IColor offColor);

	bool Draw(IGraphics* pGraphics) override;

	void OnMouseDown(int x, int y, IMouseMod* pMod) override;
	void OnMouseUp(int x, int y, IMouseMod* pMod);

private:
	IColor mOnColor;
	IColor mOffColor;
};
