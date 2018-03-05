#pragma once

#include "IControl.h"

// provides a standardized background and label for a group of controls
class ControlGroup : public IPanelControl
{
public:
	ControlGroup(IPlugBase* pPlug, IRECT iRect, const IColor* backColor, IText* textStyle, const char * label);

	// transform iRect into our local space
	IRECT GetControlRect(IRECT iRect) const;	

	virtual bool Draw(IGraphics* pGraphics) override;

private:
	IRECT mTextRect;
	const char * mLabel;
};
