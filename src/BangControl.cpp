#include "BangControl.h"

BangControl::BangControl(IPlugBase* pPlug, IRECT iRect, int paramIdx, IColor onColor, IColor offColor)
	: IControl(pPlug, iRect, paramIdx)
	, mOnColor(onColor)
	, mOffColor(offColor)
{
	mDblAsSingleClick = true;
}

bool BangControl::Draw(IGraphics* pGraphics)
{
	if (mValue > 0.5)
	{
		pGraphics->FillIRect(&mOnColor, &mRECT);
	}
	else
	{
		pGraphics->FillIRect(&mOffColor, &mRECT);
	}

	return true;
}

void BangControl::OnMouseDown(int x, int y, IMouseMod* pMod)
{
	mValue = 1;
	SetDirty();
}

void BangControl::OnMouseUp(int x, int y, IMouseMod* pMod)
{
	mValue = 0;
	SetDirty();
}
