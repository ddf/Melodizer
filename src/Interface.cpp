#include "Interface.h"

#include "..\Melodizer.h"
#include "Params.h"

#include "KnobLineCoronaControl.h"
#include "LED.h"

enum ELayout
{
	kWidth = GUI_WIDTH,
	kHeight = GUI_HEIGHT,

	kFirstKnobColumnX = 95,
	kColumnSpacing = 45,

	kKnobLED_W = 8,
	kKnobLED_H = kKnobLED_W,
	kKnobLED_X = kFirstKnobColumnX - kKnobLED_W / 2,
	kKnobLED_Y = 40 - kKnobLED_H / 2,

	kProbabilityKnob_W = 30,
	kProbabilityKnob_H = kProbabilityKnob_W,
	kProbabilityKnob_X = kFirstKnobColumnX - kProbabilityKnob_W / 2,
	kProbabilityKnob_Y = kKnobLED_Y + kKnobLED_H + 10,

	kProbabilityLabel_X = 10,
	kProbabilityLabel_Y = kProbabilityKnob_Y + kProbabilityKnob_H / 2,
	kProbabilityLabel_W = kFirstKnobColumnX - kProbabilityLabel_X,
	kProbabilityLabel_H = 10,
};

namespace Color
{
	const IColor LedOn(255, 89, 196, 255);
	const IColor LedOff(255, 39, 96, 155);

	const IColor Background(255, 7, 14, 38);
	const IColor KnobLine(255, 89, 196, 255);
	const IColor KnobCorona(255, 89, 196, 255);
	
	const IColor Label(255, 208, 208, 216);
}

namespace TextStyle
{
	// can't be const because of stupid ITextControl constructor
	IText  Label(DEFAULT_TEXT_SIZE, &Color::Label, 0, IText::kStyleNormal, IText::kAlignNear);
}

namespace Strings
{
	// again, these can't be const because of non-const method args :(
	char * ProbabilityLabel = "Probability";
}

Interface::Interface(Melodizer* inPlug)
	: mPlug(inPlug)
	, mLEDs()
{
}

Interface::~Interface()
{
}

void Interface::CreateControls(IGraphics* pGraphics)
{
	pGraphics->AttachPanelBackground(&Color::Background);

	mLEDs.reserve(kProbabilityLast - kProbabilityFirst + 1);

	// all them knobs
	const int knobCount = 16;
	for(int i = 0; i < knobCount; ++i)
	{
		const int offset = kColumnSpacing*i;
		IRECT ledRect = MakeIRectHOffset(kKnobLED, offset);
		LED* led = new LED(mPlug, ledRect, Color::Background, Color::LedOn, Color::LedOff);
		mLEDs.push_back(led);
		pGraphics->AttachControl(led);
		
		IRECT knobRect = MakeIRectHOffset(kProbabilityKnob, offset);
		pGraphics->AttachControl(new KnobLineCoronaControl(mPlug, knobRect, i, &Color::KnobLine, &Color::KnobCorona));
	}

	// row labels
	IRECT labelRect = MakeIRect(kProbabilityLabel);
	pGraphics->MeasureIText(&TextStyle::Label, Strings::ProbabilityLabel, &labelRect);
	const int offset = labelRect.H() / 2;
	labelRect.T -= offset;
	labelRect.B -= offset;
	pGraphics->AttachControl(new ITextControl(mPlug, labelRect, &TextStyle::Label, Strings::ProbabilityLabel));
}

void Interface::OnTick(const unsigned int tick)
{
	if ( tick < mLEDs.size() )
	{
		mLEDs[tick]->Blink();
	}
}
