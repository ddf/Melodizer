#include "BangControl.h"
#include "Params.h"

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
		char * label = const_cast<char*>(mLabel);
		IRECT textRect = mRECT;
		// vertically center the text
		pGraphics->MeasureIText(&mText, label, &textRect);
#ifdef OS_OSX
		textRect.B -= 4;
#endif
		int offset = (mRECT.H() - textRect.H()) / 2;
		textRect.T += offset;
		textRect.B += offset;
		pGraphics->MeasureIText(&mText, label, &textRect);
		pGraphics->DrawIText(&mText, label, &textRect);
	}

	return true;
}

void BangControl::OnMouseDown(int x, int y, IMouseMod* pMod)
{
	if (mParamIdx < kNumParams)
	{
		mValue = 1;
		SetDirty();
	}
	else // not an actual parameter, direct-cast this bang on click
	{
		switch (mParamIdx)
		{
		case kSavePreset:
		{
			WDL_String fileName("");
			WDL_String directory("");
			GetGUI()->PromptForFile(&fileName, kFileSave, &directory, "fxp");
			if (fileName.GetLength() > 0)
			{
				mPlug->SaveProgramAsFXP(&fileName);
			}
		}
		break;

		case kLoadPreset:
		{
			WDL_String fileName("");
			WDL_String directory("");
			GetGUI()->PromptForFile(&fileName, kFileOpen, &directory, "fxp");
			if (fileName.GetLength() > 0)
			{
				mPlug->LoadProgramFromFXP(&fileName);
			}
		}
		break;
		}
	}
}

void BangControl::OnMouseUp(int x, int y, IMouseMod* pMod)
{
	if (mParamIdx < kNumParams)
	{
		mValue = 0;
		SetDirty();
	}
}
