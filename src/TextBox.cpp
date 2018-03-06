#include "TextBox.h"
#include "Melodizer.h"

TextBox::TextBox(IPlugBase* pPlug, IRECT pR, int paramIdx, IText* pText, IGraphics* pGraphics, const char * maxText, bool showParamUnits, float scrollSpeed)
	: ICaptionControl(pPlug, pR, paramIdx, pText, showParamUnits)
	, mTextRect(pR)
	, mScrollSpeed(scrollSpeed)
{
	mTextRect.GetPadded(-1);
	pGraphics->MeasureIText(pText, const_cast<char*>(maxText), &mTextRect);
#ifdef OS_OSX
	mTextRect.B -= 4;
#endif
	const int offset = (mRECT.H() - mTextRect.H()) / 2;
	mTextRect.T += offset;
	mTextRect.B += offset;

	SetTextEntryLength(strlen(maxText) - 1);
}

bool TextBox::Draw(IGraphics* pGraphics)
{
	pGraphics->FillIRect(&mText.mTextEntryBGColor, &mRECT);
	pGraphics->DrawRect(&mText.mTextEntryFGColor, &mRECT);

	IRECT ourRect = mRECT;
	mRECT = mTextRect;
	bool result = ICaptionControl::Draw(pGraphics);
	mRECT = ourRect;

	return result;
}

void TextBox::OnMouseDown(int x, int y, IMouseMod* pMod)
{
	if (pMod->L)
	{
		IText ourText = mText;
		IRECT promptRect = mTextRect;
#if defined(OS_OSX)
		mText.mSize -= 2;
		promptRect.T -= 1;
#endif
		mPlug->GetGUI()->PromptUserInput(this, mPlug->GetParam(mParamIdx), &promptRect);
		mText = ourText;
		Redraw();
	}
	else if (pMod->R)
	{
		Melodizer* plug = static_cast<Melodizer*>(mPlug);
		if (plug != nullptr)
		{
			plug->BeginMIDILearn(mParamIdx, -1, x, y);
		}
	}
}

void TextBox::OnMouseWheel(int x, int y, IMouseMod* pMod, int d)
{
#ifdef PROTOOLS
	if (pMod->C)
	{
		mValue += GetParam()->GetStep() * mScrollSpeed/10 * d;
	}
#else
	if (pMod->C || pMod->S)
	{
		mValue += GetParam()->GetStep() * mScrollSpeed/10 * d;
	}
#endif
	else
	{
		mValue += GetParam()->GetStep() * mScrollSpeed * d;
	}

	SetDirty();
}
