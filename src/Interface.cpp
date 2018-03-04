#include "Interface.h"

#include "Melodizer.h"
#include "Params.h"

#include "KnobLineCoronaControl.h"
#include "LED.h"
#include "EnumControl.h"
#include "TextBox.h"
#include "BangControl.h"

enum ELayout
{
	kEnvControl_X = 20,
	kEnvControl_Y = 20,
	kEnvControl_W = 30,
	kEnvControl_H = 30,
	kEnvControl_S = 45,
	
	kVoiceControl_X = kEnvControl_X + 4*kEnvControl_S,
	kVoiceControl_Y = kEnvControl_Y,
	kVoiceControl_H = 20,
	kVoiceControl_W = 20,
	
	kVolumeControl_X = kVoiceControl_X + kVoiceControl_W + 10,
	kVolumeControl_Y = kVoiceControl_Y,
	kVolumeControl_W = 30,
	kVolumeControl_H = 30,
	
	kWidthControl_X = kVolumeControl_X + kVolumeControl_W + 10,
	kWidthControl_Y = kEnvControl_Y,
	kWidthControl_W = 30,
	kWidthControl_H = 30,
	
	kGlideControl_X = kWidthControl_X + kWidthControl_W + 10,
	kGlideControl_Y = kEnvControl_Y,
	kGlideControl_W = 30,
	kGlideControl_H = 30,

	kEnumHeight = 20,
	kEnumY = kEnvControl_Y + kEnvControl_H + 10,

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
	
	kStepLengthControl_X = kTempoControl_X + kTempoControl_W + 10,
	kStepLengthControl_Y = kEnumY,
	kStepLengthControl_W = 30,
	kStepLengthControl_H = kEnumHeight,

	kOctaveControl_X = kStepLengthControl_X + kStepLengthControl_W + 10,
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

	kFirstKnobColumnX = 65,

	kKnobLED_W = 8,
	kKnobLED_H = kKnobLED_W,
	kKnobLED_X = kFirstKnobColumnX - kKnobLED_W / 2,
	kKnobLED_Y = kEnumY + kEnumHeight + 25 - kKnobLED_H / 2,

	kStepModeControl_W = 36,
	kStepModeControl_H = 15,
	kStepModeControl_X = kFirstKnobColumnX - kStepModeControl_W / 2,
	kStepModeControl_Y = kKnobLED_Y + kKnobLED_H + 10,

	kStepKnob_W = 30,
	kStepKnob_H = kStepKnob_W,
	kStepKnob_X = kFirstKnobColumnX - kStepKnob_W / 2,
	kStepKnob_Y = kStepModeControl_Y + kStepModeControl_H + 10,

	kStepKnobLabel_X = 0,
	kStepKnobLabel_Y = kStepKnob_Y + kStepKnob_H / 2,
	kStepKnobLabel_W = kFirstKnobColumnX - kStepKnobLabel_X - kStepKnob_W / 2,
	kStepKnobLabel_H = 10,

	kStepKnobColumnSpacing = 45,
	kStepKnobRowSpacing = 45,

	kStepRandomize_W = 10,
	kStepRandomize_H = 10,
	kStepRandomize_X = kFirstKnobColumnX + kStepKnobColumnSpacing*(kSequencerSteps-1) + 35,
	kStepRandomize_Y = kStepKnob_Y + kStepKnob_H / 2 - kStepRandomize_H / 2,	
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
	const char * PanLabel = "L-R";
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

	AttachKnob(pGraphics, MakeIRect(kEnvControl), kEnvAttack, "A");
	AttachKnob(pGraphics, MakeIRectHOffset(kEnvControl, kEnvControl_S), kEnvDecay, "D");
	AttachKnob(pGraphics, MakeIRectHOffset(kEnvControl, kEnvControl_S * 2), kEnvSustain, "S");
	AttachKnob(pGraphics, MakeIRectHOffset(kEnvControl, kEnvControl_S * 3), kEnvRelease, "R");
	
	pGraphics->AttachControl(new TextBox(mPlug, MakeIRect(kVoiceControl), kVoices, &TextStyles::Enum, pGraphics, "000"));
	AttachKnob(pGraphics, MakeIRect(kVolumeControl), kVolume, "Gain");
	AttachKnob(pGraphics, MakeIRect(kWidthControl), kWidth, "Width");
	AttachKnob(pGraphics, MakeIRect(kGlideControl), kGlide, "Glide");

	pGraphics->AttachControl(new EnumControl(mPlug, MakeIRect(kWaveformControl), kWaveform, &TextStyles::Enum));
	pGraphics->AttachControl(new EnumControl(mPlug, MakeIRect(kScaleControl), kScale, &TextStyles::Enum));
	pGraphics->AttachControl(new EnumControl(mPlug, MakeIRect(kKeyControl), kKey, &TextStyles::Enum));

	// tempo
	pGraphics->AttachControl(new TextBox(mPlug, MakeIRect(kTempoControl), kTempo, &TextStyles::Enum, pGraphics, "000.000"));
	// step length
	pGraphics->AttachControl(new EnumControl(mPlug, MakeIRect(kStepLengthControl), kStepLength, &TextStyles::Enum));
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
		
		// mode
		pGraphics->AttachControl(new EnumControl(mPlug, MakeIRectHOffset(kStepModeControl, hoffset), kStepModeFirst + i, &TextStyles::Enum));
		// led behind the probability knob that blinks when a note is actually played
		ledRect = MakeIRectHOffset(kStepKnob, hoffset).GetPadded(-4);
		led = new LED(mPlug, ledRect, Color::Background, Color::LedOn, Color::Background);
		mNoteOns.push_back(led);
		pGraphics->AttachControl(led);
		// probability
		AttachKnob(pGraphics, MakeIRectHOffset(kStepKnob, hoffset), kProbabilityFirst + i);
		// pan
		AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing*1), kPanFirst + i);
		// attack
		AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing*2), kAttackFirst + i);
		// decay
		AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing*3), kDecayFirst + i);
		// sustain
		AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing*4), kSustainFirst + i);
		// release
		AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing*5), kReleaseFirst + i);
	}

	AttachStepRowLabel(pGraphics, 0, Strings::ProbabilityLabel);
	AttachStepRowLabel(pGraphics, 1, Strings::PanLabel);
	AttachStepRowLabel(pGraphics, 2, Strings::AttackLabel);
	AttachStepRowLabel(pGraphics, 3, Strings::DecayLabel);
	AttachStepRowLabel(pGraphics, 4, Strings::SustainLabel);
	AttachStepRowLabel(pGraphics, 5, Strings::ReleaseLabel);

	AttachStepRowRandomizer(pGraphics, 0, kProbabilityRandomize);
	AttachStepRowRandomizer(pGraphics, 1, kPanRandomize);
	AttachStepRowRandomizer(pGraphics, 2, kAttackRandomize);
	AttachStepRowRandomizer(pGraphics, 3, kDecayRandomize);
	AttachStepRowRandomizer(pGraphics, 4, kSustainRandomize);
	AttachStepRowRandomizer(pGraphics, 5, kReleaseRandomize);
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

void Interface::AttachStepRowRandomizer(IGraphics* pGraphics, int rowNum, const int param)
{
	IRECT rect = MakeIRectVOffset(kStepRandomize, kStepKnobRowSpacing*rowNum);
	pGraphics->AttachControl(new BangControl(mPlug, rect, param, Color::LedOn, Color::LedOff));
}

void Interface::AttachKnob(IGraphics* pGraphics, IRECT rect, const int paramIdx, const char * label)
{
	pGraphics->AttachControl(new KnobLineCoronaControl(mPlug, rect, paramIdx, &Color::KnobLine, &Color::KnobCorona));
	
	if (label != nullptr)
	{
		rect.B = rect.T;
		rect.T -= 15,
		pGraphics->AttachControl(new ITextControl(mPlug, rect, &TextStyles::Label, const_cast<char*>(label)));
	}
}

void Interface::OnTick(const unsigned int tick, bool noteOn)
{
	if ( tick < mLEDs.size() )
	{
		mLEDs[tick]->Blink();
		if (noteOn)
		{
			mNoteOns[tick]->Blink();
		}
	}
}
