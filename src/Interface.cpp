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

	kKeyControl_X = 80,
	kKeyControl_Y = 10,
	kKeyControl_W = 25,
	kKeyControl_H = kEnumHeight,

	kScaleControl_X = kKeyControl_X + kKeyControl_W + 10,
	kScaleControl_Y = kKeyControl_Y,
	kScaleControl_W = 100,
	kScaleControl_H = kEnumHeight,

	kWaveformControl_X = kScaleControl_X + kScaleControl_W + 10,
	kWaveformControl_Y = 10,
	kWaveformControl_W = 50,
	kWaveformControl_H = kEnumHeight,

	kTempoControl_X = kWaveformControl_X + kWaveformControl_W + 10,
	kTempoControl_Y = 10,
	kTempoControl_W = 50,
	kTempoControl_H = kEnumHeight,

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

	const IColor EnumBackground(255, 9, 66, 125);
	const IColor EnumBorder = KnobLine;
}

namespace TextStyle
{
	// can't be const because of stupid ITextControl constructor
	IText  Label(DEFAULT_TEXT_SIZE, &Color::Label, 0, IText::kStyleNormal, IText::kAlignNear);
	IText  Enum(DEFAULT_TEXT_SIZE, &Color::Label, 0, IText::kStyleNormal, IText::kAlignCenter, 0, IText::kQualityDefault, &Color::EnumBackground, &Color::EnumBorder);
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

	pGraphics->AttachControl(new EnumControl(mPlug, MakeIRect(kWaveformControl), kWaveform, &TextStyle::Enum));
	pGraphics->AttachControl(new EnumControl(mPlug, MakeIRect(kScaleControl), kScale, &TextStyle::Enum));
	pGraphics->AttachControl(new EnumControl(mPlug, MakeIRect(kKeyControl), kKey, &TextStyle::Enum));

	// tempo
	{
		IRECT tempoRect = MakeIRect(kTempoControl);
		IRECT tempoTextRect = MakeIRect(kTempoControl);
		tempoTextRect.GetPadded(-1);
		pGraphics->MeasureIText(&TextStyle::Enum, "000.00", &tempoTextRect);
		const int offset = (tempoRect.H() - tempoTextRect.H()) / 2;
		tempoTextRect.T += offset;
		tempoTextRect.B += offset;
		pGraphics->AttachControl(new TextBox(mPlug, tempoRect, kTempo, &TextStyle::Enum, tempoTextRect));
	}

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
