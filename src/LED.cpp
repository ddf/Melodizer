#include "LED.h"

LED::LED(IPlugBase* pPlug, IRECT iRect, IColor backColor, IColor onColor, IColor offColor) 
	: IControl(pPlug, iRect)
	, mBackColor(backColor)
	, mOnColor(onColor)
	, mOffColor(offColor)
	, mColorLerp(0)
{

}

LED::~LED()
{
}

bool LED::Draw(IGraphics* pGraphics)
{

	IColor color = mOffColor;
	color.A += (mOnColor.A - mOffColor.A)*mColorLerp;
	color.R += (mOnColor.R - mOffColor.R)*mColorLerp;
	color.G += (mOnColor.G - mOffColor.G)*mColorLerp;
	color.B += (mOnColor.B - mOffColor.B)*mColorLerp;

	IChannelBlend backBlend(IChannelBlend::kBlendClobber);

	pGraphics->FillIRect(&mBackColor, &mRECT, &backBlend);
	pGraphics->FillCircle(&color, mRECT.MW(), mRECT.MH(), mRECT.W()/2 - 1);

	if (mColorLerp > 0)
	{
		mColorLerp = BOUNDED(mColorLerp - 0.1f, 0, 1);
		Redraw();
	}

	return true;
}

void LED::Blink()
{
	mColorLerp = 1;
	SetDirty();
}
