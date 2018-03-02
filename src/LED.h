#pragma once

#include "IControl.h"

class LED : public IControl
{
public:
	LED(IPlugBase* pPlug, IRECT iRect, IColor backColor, IColor onColor, IColor offColor);
	~LED();

	virtual bool Draw(IGraphics* pGraphics) override;

	void Blink();
	
private:
	IColor mBackColor;
	IColor mOnColor;
	IColor mOffColor;
	float  mColorLerp;
};

