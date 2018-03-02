#pragma once

#include "IControl.h"

class TextBox : public ICaptionControl
{
public:
	TextBox(IPlugBase* pPlug, IRECT pR, int paramIdx, IText* pText, IRECT textRect, bool showParamUnits = false);

	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDown(int x, int y, IMouseMod* pMod) override;
	void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;
	void OnMouseWheel(int x, int y, IMouseMod* pMod, int d) override;

private:
	bool  mShowParamUnits;
	IRECT mTextRect;
};
