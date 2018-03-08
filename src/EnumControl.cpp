#include "EnumControl.h"
#include "Melodizer.h"

EnumControl::EnumControl(IPlugBase* pPlug, IRECT rect, int paramIdx, IText* textStyle)
	: IControl(pPlug, rect, paramIdx)
{
	SetText(textStyle);
	mDblAsSingleClick = true;
	mDisablePrompt = false;
	// our contents need to fit into a 2px padded rect so we can draw the outline using mRECT
	// and have 1px background color between that and the contents
	IRECT contentsRect = mRECT.GetPadded(-5);
	mDecrementRect = IRECT(contentsRect.L, contentsRect.T, contentsRect.L + 5, contentsRect.B);
	mIncrementRect = IRECT(contentsRect.R - 5, contentsRect.T, contentsRect.R, contentsRect.B);

	mTextRect = contentsRect.GetHPadded(-7);

	if (paramIdx < kNumParams)
	{
		mMin = GetParam()->GetMin();
		mMax = GetParam()->GetMax();
	}
	else
	{
		mMin = 0;
		mMax = mPlug->NPresets() - 1;
	}
}

bool EnumControl::Draw(IGraphics* pGraphics)
{
	pGraphics->FillIRect(&mText.mTextEntryBGColor, &mRECT);
	pGraphics->DrawRect(&mText.mTextEntryFGColor, &mRECT);

	// buttons
	IColor buttonColor = mText.mTextEntryFGColor;
	const int value = mParamIdx < kNumParams ? GetParam()->Int() : mPlug->GetCurrentPresetIdx();
	if (value == mMin)
	{
		buttonColor.R *= 0.5f; buttonColor.G *= 0.5f; buttonColor.B *= 0.5f;
	}
	pGraphics->FillTriangle(&buttonColor, mDecrementRect.L, mDecrementRect.MH(), mDecrementRect.R, mDecrementRect.T, mDecrementRect.R, mDecrementRect.B, 0);

	buttonColor = mText.mTextEntryFGColor;
	if (value == mMax)
	{
		buttonColor.R *= 0.5f; buttonColor.G *= 0.5f; buttonColor.B *= 0.5f;
	}
	pGraphics->FillTriangle(&buttonColor, mIncrementRect.L, mIncrementRect.T, mIncrementRect.R, mIncrementRect.MH(), mIncrementRect.L, mIncrementRect.B, 0);

	char* label = 0;
	if (mParamIdx < kNumParams)
	{
		static char display[16];
		GetParam()->GetDisplayForHost(display);
		label = display;
	}
	else
	{
		label = const_cast<char*>(mPlug->GetPresetName(mPlug->GetCurrentPresetIdx()));
	}

	IRECT textRect = mTextRect;
	// vertically center the text
	pGraphics->MeasureIText(&mText, label, &textRect);
#ifdef OS_OSX
	textRect.B -= 4;
#endif
	int offset = (mTextRect.H() - textRect.H()) / 2;
	textRect.T += offset;
	textRect.B += offset;
	pGraphics->DrawIText(&mText, label, &textRect);

	return true;
}

void EnumControl::OnMouseDown(int x, int y, IMouseMod* pMod)
{
	if (pMod->L)
	{
		if (mTextRect.Contains(x, y))
		{
			if (mParamIdx < kNumParams)
			{
				PromptUserInput();
			}
			else // need to build the popup for presets by hand
			{
				IPopupMenu menu;
				const int currentPresetIdx = mPlug->GetCurrentPresetIdx();
				for (int i = 0; i < mPlug->NPresets(); ++i)
				{
					const char * presetName = mPlug->GetPresetName(i);
					if (i == currentPresetIdx)
					{
						menu.AddItem(new IPopupMenuItem(presetName, IPopupMenuItem::kChecked), -1);
					}
					else
					{
						menu.AddItem(new IPopupMenuItem(presetName), -1);
					}
				}

				if (GetGUI()->CreateIPopupMenu(&menu, &mTextRect))
				{
					mPlug->RestorePreset(menu.GetChosenItemIdx());
				}
			}
		}
		else if (mIncrementRect.Contains(x, y))
		{
			StepValue(1);
		}
		else if (mDecrementRect.Contains(x, y))
		{
			StepValue(-1);
		}
	}
	else if ( pMod->R && mParamIdx < kNumParams)
	{
		Melodizer* plug = static_cast<Melodizer*>(mPlug);
		if (plug != nullptr)
		{
			plug->BeginMIDILearn(mParamIdx, -1, x, y);
		}
	}
}

void EnumControl::OnMouseWheel(int x, int y, IMouseMod* pMod, int d)
{
	StepValue(d);
}

void EnumControl::StepValue(int amount)
{
	const bool isParam = mParamIdx < kNumParams;
	if (isParam)
	{
		int count = GetParam()->GetNDisplayTexts();
		if (count > 1)
		{
			mValue += 1.0 / (double)(count - 1) * amount;
		}
		else
		{
			mValue += 1.0;
		}
	}
	else
	{
		const int presetIdx = BOUNDED(mPlug->GetCurrentPresetIdx() + amount, mMin, mMax);
		if (presetIdx != mPlug->GetCurrentPresetIdx())
		{
			mPlug->RestorePreset(presetIdx);
		}
	}

	SetDirty(isParam);
	Redraw();
}
