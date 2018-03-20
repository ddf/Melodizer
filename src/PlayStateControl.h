#pragma once

#include "IControl.h"
#include "Params.h"

// play, pause, and stop buttons used by the standalone to control how t increments
// essentially makes TTAlways behave like TTProjectTime
class PlayStateControl : public IControl
{
public:
	// give the rect that the three buttons should occupy and we split it into three sections
	PlayStateControl(IPlugBase* pPlug, IRECT rect, const IColor& backgroundColor, const IColor& foregroundColor);
	
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDown(int x, int y, IMouseMod* pMod) override;
	
private:
	
	PlayState GetPlayState() const;
	void SetPlayState(PlayState state);
	
	IRECT mPlayRect;
	IRECT mPauseRect;
	IRECT mStopRect;
	IColor mBack;
	IColor mFore;
};
