#include "Interface.h"

#include "Melodizer.h"
#include "Params.h"

#include "KnobLineCoronaControl.h"
#include "LED.h"
#include "EnumControl.h"
#include "TextBox.h"

enum ELayout
{
	kWidth = GUI_WIDTH,
	kHeight = GUI_HEIGHT,

	kEnumHeight = 20,
	kEnumY = 10,

	kKeyControl_X = 80,
	kKeyControl_Y = kEnumY,
	kKeyControl_W = 25,
	kKeyControl_H = kEnumHeight,

	kScaleControl_X = kKeyControl_X + kKeyControl_W + 10,
	kScaleControl_Y = kEnumY,
	kScaleControl_W = 100,
	kScaleControl_H = kEnumHeight,

	kWaveformControl_X = kScaleControl_X + kScaleControl_W + 10,
	kWaveformControl_Y = kEnumY,
	kWaveformControl_W = 50,
	kWaveformControl_H = kEnumHeight,

	kTempoControl_X = kWaveformControl_X + kWaveformControl_W + 10,
	kTempoControl_Y = kEnumY,
	kTempoControl_W = 50,
	kTempoControl_H = kEnumHeight,
	
	kOctaveControl_X = kTempoControl_X + kTempoControl_W + 10,
	kOctaveControl_Y = kEnumY,
	kOctaveControl_W = 20,
	kOctaveControl_H = kEnumHeight,
	
	kRangeControl_X = kOctaveControl_X + kOctaveControl_W + 10,
	kRangeControl_Y = kEnumY,
	kRangeControl_W = 20,
	kRangeControl_H = kEnumHeight,

	kFirstKnobColumnX = 95,
	kColumnSpacing = 45,

	kKnobLED_W = 8,
	kKnobLED_H = kKnobLED_W,
	kKnobLED_X = kFirstKnobColumnX - kKnobLED_W / 2,
	kKnobLED_Y = 50 - kKnobLED_H / 2,

	kProbabilityKnob_W = 30,
	kProbabilityKnob_H = kProbabilityKnob_W,
	kProbabilityKnob_X = kFirstKnobColumnX - kProbabilityKnob_W / 2,
	kProbabilityKnob_Y = kKnobLED_Y + kKnobLED_H + 10,

	kProbabilityLabel_X = 0,
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
	
	const IColor Label = KnobLine; // (255, 208, 208, 216);

	const IColor EnumBackground(255, 9, 66, 125);
	const IColor EnumBorder = KnobLine;
}

namespace TextStyles
{
#ifdef OS_WIN
	const int TextSize = 12;
	char * Font = 0;
#else
	const int TextSize = 14;
	char * Font = 0;
#endif
	// can't be const because of stupid ITextControl constructor
	IText  Label(TextSize, &Color::Label, Font, IText::kStyleBold, IText::kAlignCenter);
	IText  Enum(TextSize, &Color::Label, Font, IText::kStyleNormal, IText::kAlignCenter, 0, IText::kQualityDefault, &Color::EnumBackground, &Color::EnumBorder);
}

namespace Strings
{
	// again, these can't be const because of non-const method args :(
	char * ProbabilityLabel = "P(N)";
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

	pGraphics->AttachControl(new EnumControl(mPlug, MakeIRect(kWaveformControl), kWaveform, &TextStyles::Enum));
	pGraphics->AttachControl(new EnumControl(mPlug, MakeIRect(kScaleControl), kScale, &TextStyles::Enum));
	pGraphics->AttachControl(new EnumControl(mPlug, MakeIRect(kKeyControl), kKey, &TextStyles::Enum));

	// tempo
	pGraphics->AttachControl(new TextBox(mPlug, MakeIRect(kTempoControl), kTempo, &TextStyles::Enum, pGraphics, "000.000"));
	// octave
	pGraphics->AttachControl(new TextBox(mPlug, MakeIRect(kOctaveControl), kOctave, &TextStyles::Enum, pGraphics, "00"));
	// range
	pGraphics->AttachControl(new TextBox(mPlug, MakeIRect(kRangeControl), kRange, &TextStyles::Enum, pGraphics, "00"));

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
		const int paramIdx = kProbabilityFirst + i;
		pGraphics->AttachControl(new KnobLineCoronaControl(mPlug, knobRect, paramIdx, &Color::KnobLine, &Color::KnobCorona));
	}

	// row labels
	IRECT labelRect = MakeIRect(kProbabilityLabel);
	pGraphics->MeasureIText(&TextStyles::Label, Strings::ProbabilityLabel, &labelRect);
	const int offset = labelRect.H() / 2;
	labelRect.T -= offset;
	labelRect.B -= offset;
	pGraphics->AttachControl(new ITextControl(mPlug, labelRect, &TextStyles::Label, Strings::ProbabilityLabel));
}

void Interface::OnTick(const unsigned int tick)
{
	if ( tick < mLEDs.size() )
	{
		mLEDs[tick]->Blink();
	}
}
