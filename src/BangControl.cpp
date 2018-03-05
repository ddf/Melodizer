#include "BangControl.h"

BangControl::BangControl(IPlugBase* pPlug, IRECT iRect, int paramIdx, IColor onColor, IColor offColor,
						 IText* textStyle, const char * label)
	: IControl(pPlug, iRect, paramIdx)
	, mOnColor(onColor)
	, mOffColor(offColor)
	, mLabel(label)
{
	if ( textStyle != nullptr )
	{
		SetText(textStyle);
	}
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
	
	pGraphics->DrawRect(&mOnColor, &mRECT);
	
	if ( mLabel != nullptr )
	{
		pGraphics->DrawIText(&mText, const_cast<char*>(mLabel), &mRECT);
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
