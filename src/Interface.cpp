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

	kShuffleControl_X = kRangeControl_X + kRangeControl_W + 10,
	kShuffleControl_Y = kEnumY,
	kShuffleControl_W = 20,
	kShuffleControl_H = 20,

	kFirstKnobColumnX = 95,

	kKnobLED_W = 8,
	kKnobLED_H = kKnobLED_W,
	kKnobLED_X = kFirstKnobColumnX - kKnobLED_W / 2,
	kKnobLED_Y = 50 - kKnobLED_H / 2,

	kStepKnob_W = 30,
	kStepKnob_H = kStepKnob_W,
	kStepKnob_X = kFirstKnobColumnX - kStepKnob_W / 2,
	kStepKnob_Y = kKnobLED_Y + kKnobLED_H + 10,

	kStepKnobLabel_X = 10,
	kStepKnobLabel_Y = kStepKnob_Y + kStepKnob_H / 2,
	kStepKnobLabel_W = kFirstKnobColumnX - kStepKnobLabel_X,
	kStepKnobLabel_H = 10,

	kStepKnobColumnSpacing = 45,
	kStepKnobRowSpacing = 45,
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
	const char * ProbabilityLabel = "P(N)";
	const char * AttackLabel = "A";
	const char * DecayLabel = "D";
	const char * SustainLabel = "S";
	const char * ReleaseLabel = "R";
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
	// shuffle
	pGraphics->AttachControl(new KnobLineCoronaControl(mPlug, MakeIRect(kShuffleControl), kShuffle, &Color::KnobLine, &Color::KnobCorona));

	mLEDs.reserve(kProbabilityLast - kProbabilityFirst + 1);

	// all them knobs
	for(int i = 0; i < kSequencerSteps; ++i)
	{
		const int hoffset = kStepKnobColumnSpacing*i;
		IRECT ledRect = MakeIRectHOffset(kKnobLED, hoffset);
		LED* led = new LED(mPlug, ledRect, Color::Background, Color::LedOn, Color::LedOff);
		mLEDs.push_back(led);
		pGraphics->AttachControl(led);
		
		// probability
		pGraphics->AttachControl(new KnobLineCoronaControl(mPlug, MakeIRectHOffset(kStepKnob, hoffset), kProbabilityFirst + i, &Color::KnobLine, &Color::KnobCorona));
		// attack
		pGraphics->AttachControl(new KnobLineCoronaControl(mPlug, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing*1), kAttackFirst + i, &Color::KnobLine, &Color::KnobCorona));
		// decay
		pGraphics->AttachControl(new KnobLineCoronaControl(mPlug, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing*2), kDecayFirst + i, &Color::KnobLine, &Color::KnobCorona));
		// sustain
		pGraphics->AttachControl(new KnobLineCoronaControl(mPlug, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing*3), kSustainFirst + i, &Color::KnobLine, &Color::KnobCorona));
		// release
		pGraphics->AttachControl(new KnobLineCoronaControl(mPlug, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing*4), kReleaseFirst + i, &Color::KnobLine, &Color::KnobCorona));
	}

	AttachStepRowLabel(pGraphics, 0, Strings::ProbabilityLabel);
	AttachStepRowLabel(pGraphics, 1, Strings::AttackLabel);
	AttachStepRowLabel(pGraphics, 2, Strings::DecayLabel);
	AttachStepRowLabel(pGraphics, 3, Strings::SustainLabel);
	AttachStepRowLabel(pGraphics, 4, Strings::ReleaseLabel);
}

void Interface::AttachStepRowLabel(IGraphics* pGraphics, int rowNum, const char * name)
{
	IRECT labelRect = MakeIRectVOffset(kStepKnobLabel, kStepKnobRowSpacing*rowNum);
	pGraphics->MeasureIText(&TextStyles::Label, const_cast<char*>(name), &labelRect);
	const int offset = labelRect.H() / 2;
	labelRect.T -= offset;
	labelRect.B -= offset;
	pGraphics->AttachControl(new ITextControl(mPlug, labelRect, &TextStyles::Label, const_cast<char*>(name)));
}

void Interface::OnTick(const unsigned int tick)
{
	if ( tick < mLEDs.size() )
	{
		mLEDs[tick]->Blink();
	}
}
