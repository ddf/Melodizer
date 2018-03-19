#include "KnobLineCoronaControl.h"

#include "Melodizer.h"

#pragma  region KnobLineCoronaControl
KnobLineCoronaControl::KnobLineCoronaControl(IPlugBase* pPlug, IRECT pR, int paramIdx,
											 const IColor* pColor, const IColor* pCoronaColor,
											 float coronaThickness,
											 double innerRadius, double outerRadius,
											 double minAngle, double maxAngle,
											 EDirection direction, double gearing)
: IKnobLineControl(pPlug, pR, paramIdx, pColor, innerRadius, outerRadius, minAngle, maxAngle, direction, gearing)
, mCoronaColor(*pCoronaColor)
, mCoronaBlend(IChannelBlend::kBlendAdd, coronaThickness)
, mLabelControl(nullptr)
, mLabelHidden(false)
{
}

bool KnobLineCoronaControl::Draw(IGraphics* pGraphics)
{
	float cx = mRECT.MW(), cy = mRECT.MH();
	float v = mMinAngle + (float)mValue * (mMaxAngle - mMinAngle);
	for (int i = 0; i <= mCoronaBlend.mWeight; ++i)
	{
		IColor color = mCoronaColor;
		pGraphics->DrawArc(&color, cx, cy, mOuterRadius - i, mMinAngle, v, nullptr, true);
		color.R /= 2;
		color.G /= 2;
		color.B /= 2;
		pGraphics->DrawArc(&color, cx, cy, mOuterRadius - i, v, mMaxAngle, nullptr, true);
	}
	return IKnobLineControl::Draw(pGraphics);
}

void KnobLineCoronaControl::OnMouseDown(int x, int y, IMouseMod* pMod)
{
	if (pMod->R)
	{
		Melodizer* plug = dynamic_cast<Melodizer*>(mPlug);
		if (plug != nullptr)
		{
			plug->BeginMIDILearn(mParamIdx, -1, x, y);
		}
	}
	else if ( mLabelControl != nullptr )
	{
		// if our label was hidden when we attached it, 
		// that means we should reposition it below the knob before displaying it.
		if (mLabelHidden)
		{
			IRECT targetRect = mRECT.GetPadded(-15, mRECT.H()-2, 15, 20);
			IRECT& labelRect = *mLabelControl->GetRECT();
			labelRect = targetRect;
			mLabelControl->SetTargetArea(targetRect);
			mLabelControl->Hide(false);
		}
		SetValDisplayControl(mLabelControl);
		SetDirty();
	}
}

void KnobLineCoronaControl::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod)
{
	double gearing = mGearing;
	
#ifdef PROTOOLS
#ifdef OS_WIN
	if (pMod->C) gearing *= 10.0;
#else
	if (pMod->R) gearing *= 10.0;
#endif
#else
	if (pMod->C || pMod->S) gearing *= 10.0;
#endif
	
	mValue += (double)dY / (double)(mRECT.T - mRECT.B) / gearing;
	mValue += (double)dX / (double)(mRECT.R - mRECT.L) / gearing;
	
	SetDirty();
}

void KnobLineCoronaControl::OnMouseUp(int x, int y, IMouseMod* pMod)
{
	SetValDisplayControl(nullptr);
	if (mLabelControl != nullptr)
	{
		mLabelControl->SetTextFromPlug(mLabelString.Get());
		if (mLabelHidden)
		{
			mLabelControl->Hide(true);
		}
	}
}

void KnobLineCoronaControl::SetLabelControl(ITextControl* control)
{
	mLabelControl = control;
	if (mLabelControl != nullptr)
	{
		mLabelString.Set(mLabelControl->GetTextForPlug());
		mLabelHidden = mLabelControl->IsHidden();
	}
	else
	{
		mLabelString.Set("");
		mLabelHidden = false;
	}
	
}

#pragma  endregion KnobLineCoronaControl
