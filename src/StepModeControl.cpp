#include "StepModeControl.h"
#include "Params.h"
#include "Melodizer.h"

StepModeControl::StepModeControl(IPlugBase* pPlug, IRECT iRect, const int paramIdx, IText* textStyle)
: IControl(pPlug, iRect, paramIdx)
{
	SetText(textStyle);
	mStateCount = GetParam()->GetNDisplayTexts();
	switch (mStateCount)
	{
	case 2:
		mStateRects[1] = iRect.SubRectVertical(2, 0).GetPadded(0, 0, 0, -1);
		mStateRects[0] = iRect.SubRectVertical(2, 1);
		break;

	case 3:
		mStateRects[2] = iRect.SubRectVertical(3, 0).GetPadded(0, 0, 0, -1);
		mStateRects[1] = iRect.SubRectVertical(3, 1);
		mStateRects[0] = iRect.SubRectVertical(3, 2).GetPadded(0, 1, 0, 0);
		break;
	}

	// expand rect to the left to account for the labels we will draw
	mRECT.L -= 20;
	mTargetRECT = mRECT;
}

bool StepModeControl::Draw(IGraphics *pGraphics)
{
	mText.mAlign = IText::kAlignFar;
	const int pad = -mStateRects[0].W() - 2;
	for (int i = 0; i < mStateCount; ++i)
	{
		IColor* fillColor = GetParam()->Int() == i ? &mText.mTextEntryFGColor : &mText.mTextEntryBGColor;
		pGraphics->FillIRect(fillColor, &mStateRects[i]);

		IRECT textRect = mStateRects[i].GetPadded(pad, 0, pad, 0);
		pGraphics->DrawIText(&mText, const_cast<char*>(GetParam()->GetDisplayText(i)), &textRect);
	}

	return true;
}

void StepModeControl::OnMouseDown(int x, int y, IMouseMod *mod)
{
	if (mod->L)
	{
		for (int i = 0; i < mStateCount; ++i)
		{
			if (mStateRects[i].Contains(x, y))
			{
				SetValueFromUserInput(GetParam()->GetNormalized(i));
				break;
			}
		}
	}
	else if (mod->R)
	{
		Melodizer* plug = static_cast<Melodizer*>(mPlug);
		if (plug != nullptr)
		{
			plug->BeginMIDILearn(mParamIdx, -1, x, y);
		}
	}
}
