#include "EnumControl.h"

EnumControl::EnumControl(IPlugBase* pPlug, IRECT rect, int paramIdx, IText* textStyle)
	: IControl(pPlug, rect, paramIdx)
{
	SetText(textStyle);
	mDblAsSingleClick = true;
	mDisablePrompt = false;
}

bool EnumControl::Draw(IGraphics* pGraphics)
{
	pGraphics->FillIRect(&mText.mTextEntryBGColor, &mRECT);
	pGraphics->DrawRect(&mText.mTextEntryFGColor, &mRECT);
	static char display[16];
	GetParam()->GetDisplayForHost(display);
	IRECT textRect = mRECT;
	// vertically center the text
	pGraphics->MeasureIText(&mText, display, &textRect);
	int offset = (mRECT.H() - textRect.H()) / 2;
	textRect.T += offset;
	textRect.B += offset;
	pGraphics->DrawIText(&mText, display, &textRect);

	return true;
}

void EnumControl::OnMouseDown(int x, int y, IMouseMod* pMod)
{
	if (pMod->R)
	{
		PromptUserInput();
	}
	else
	{
		int count = GetParam()->GetNDisplayTexts();
		if (count > 1)
		{
			mValue += 1.0 / (double)(count - 1);
		}
		else
		{
			mValue += 1.0;
		}

		if (mValue > 1.001)
		{
			mValue = 0.0;
		}
		SetDirty();
	}
}