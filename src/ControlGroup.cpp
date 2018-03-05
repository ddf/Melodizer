#include "ControlGroup.h"

ControlGroup::ControlGroup(IPlugBase* pPlug, IRECT iRect, const IColor* backColor, IText* textStyle, const char * label)
	: IPanelControl(pPlug, iRect, backColor)
	, mLabel(label)
	, mTextRect(iRect)
{
	SetText(textStyle);
	mTextRect.L += 5;
	mTextRect.T += 2;
	mTextRect.B = mRECT.T + 16;
}

IRECT ControlGroup::GetControlRect(IRECT iRect) const
{
	const int voff = mTextRect.B + 5;
	const int hoff = mRECT.L + 10;
	return IRECT(iRect.L + hoff, iRect.T + voff, iRect.R + hoff, iRect.B + voff);
}

bool ControlGroup::Draw(IGraphics* pGraphics)
{
	IRECT fillRect = mRECT;
	fillRect.R += 1;
	fillRect.B = mTextRect.B;
	pGraphics->FillIRect(&mColor, &fillRect);

	fillRect = mRECT;
	fillRect.T += 5;
	pGraphics->RoundRect(&mColor, &mRECT, 0, 5, true);

	pGraphics->DrawIText(&mText, const_cast<char*>(mLabel), &mTextRect);
	
	return true;
}
