#include "PlayStateControl.h"
#include "Melodizer.h"

PlayStateControl::PlayStateControl(IPlugBase* pPlug, IRECT rect, const IColor& backgroundColor, const IColor& foregroundColor)
: IControl(pPlug, rect)
, mBack(backgroundColor)
, mFore(foregroundColor)
{
	mDblAsSingleClick = true;
	mPlayRect = rect.SubRectHorizontal(3, 0);
	mPauseRect = rect.SubRectHorizontal(3, 1);
	mStopRect = rect.SubRectHorizontal(3, 2);
}

PlayState PlayStateControl::GetPlayState() const
{
	const Melodizer* plug = static_cast<const Melodizer*>(mPlug);
	if (plug != nullptr)
	{
		return plug->GetPlayState();
	}

	return PS_Stop;
}

void PlayStateControl::SetPlayState(PlayState state)
{
	Melodizer* plug = static_cast<Melodizer*>(mPlug);
	if (plug != nullptr)
	{
		plug->ChangePlayState(state);
	}
}

bool PlayStateControl::Draw(IGraphics* pGraphics)
{
	const PlayState state = GetPlayState();
	// play button
	{
		const bool active = (state == PS_Play || state == PS_Pause);
		const int cx = (int)mPlayRect.MW();
		const int cy = (int)mPlayRect.MH();
		IRECT back = mPlayRect.GetPadded(-1);
		pGraphics->FillIRect(active ? &mFore : &mBack, &back);
		pGraphics->FillTriangle(active ? &mBack : &mFore,
								cx - 6, cy - 5,
								cx - 6, cy + 5,
								cx + 8, cy,
								&mBlend
								);
	}
	
	// pause button
	{
		bool active = state == PS_Pause;
		IRECT back = mPauseRect.GetPadded(-1);
		pGraphics->FillIRect(active ? &mFore : &mBack, &back);
		const IColor* color = active ? &mBack : &mFore;
		const int cx = (int)mPauseRect.MW();
		const int cy = (int)mPauseRect.MH();
		const int w = 4;
		const int h = 6;
		IRECT slab(cx - (int)(w*1.5f), cy - h, cx - (int)(w*0.5f), cy + h);
		pGraphics->FillIRect(color, &slab, &mBlend);
		slab.L += w*2;
		slab.R += w*2;
		pGraphics->FillIRect(color, &slab, &mBlend);
	}
	
	// stop button
	{
		const int cx = (int)mStopRect.MW();
		const int cy = (int)mStopRect.MH();
		const int r = 6;
		IRECT back = mStopRect.GetPadded(-1);
		IRECT button(cx - r, cy - r, cx + r, cy + r);
		pGraphics->FillIRect(&mBack, &back);
		pGraphics->FillIRect(&mFore, &button);
	}
	
	return true;
}

void PlayStateControl::OnMouseDown(int x, int y, IMouseMod* pMod)
{
	if (mPlayRect.Contains(x, y))
	{
		// if pause we stop and then start
		// so that tick will be reset to zero.
		// this matches transport behavior in reaper
		// where pressing the play button while paused
		// will cause it to start playing from the timeline marker
		// rather than resuming from the current playhead position
		if ( GetPlayState() == PS_Pause )
		{
			SetPlayState(PS_Stop);
		}
		SetPlayState(PS_Play);
	}
	else if (mPauseRect.Contains(x, y))
	{
		if ( GetPlayState() == PS_Pause )
		{
			SetPlayState(PS_Play);
		}
		else
		{
			SetPlayState(PS_Pause);
		}
	}
	else if (mStopRect.Contains(x, y))
	{
		SetPlayState(PS_Stop);
	}
}
