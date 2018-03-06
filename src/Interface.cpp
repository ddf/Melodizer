#include "Interface.h"

#include "Melodizer.h"
#include "Params.h"

#include "KnobLineCoronaControl.h"
#include "LED.h"
#include "EnumControl.h"
#include "TextBox.h"
#include "BangControl.h"
#include "ControlGroup.h"
#include "StepModeControl.h"
#include "PlayStateControl.h"

enum ELayout
{
	kEnumHeight = 20,
	kLargeKnobSize = 30,
	kSmallKnobSize = 20,

	kOscGroup_X = 10,
	kOscGroup_Y = 10,
	kOscGroup_W = 205,
	kOscGroup_H = 74,

	kWaveformControl_X = 0,
	kWaveformControl_Y = 20,
	kWaveformControl_W = 65,
	kWaveformControl_H = kEnumHeight,
	
	kPulseWidthControl_X = 0,
	kPulseWidthControl_Y = 15,
	kPulseWidthControl_W = kLargeKnobSize,
	kPulseWidthControl_H = kLargeKnobSize,

	kGlideControl_X = 0,
	kGlideControl_Y = 15,
	kGlideControl_W = kLargeKnobSize,
	kGlideControl_H = kLargeKnobSize,

	kMovementControl_X = 0,
	kMovementControl_Y = 15,
	kMovementControl_W = kLargeKnobSize,
	kMovementControl_H = kLargeKnobSize,

	kEnvGroup_X = kOscGroup_X + kOscGroup_W + 10,
	kEnvGroup_Y = kOscGroup_Y,
	kEnvGroup_H = kOscGroup_H,
	kEnvGroup_W = 170,

	kEnvControl_X = 0,
	kEnvControl_Y = 15,
	kEnvControl_W = kLargeKnobSize,
	kEnvControl_H = kLargeKnobSize,
	kEnvControl_S = kLargeKnobSize + 10,

	kMasterGroup_W = 190,
	kMasterGroup_X = GUI_WIDTH - kMasterGroup_W - 10,
	kMasterGroup_Y = kOscGroup_Y,
	kMasterGroup_H = kOscGroup_H,

	kVoiceControl_X = 0,
	kVoiceControl_Y = 20,
	kVoiceControl_H = 20,
	kVoiceControl_W = 30,

	kVolumeControl_X = 0,
	kVolumeControl_Y = 15,
	kVolumeControl_W = kLargeKnobSize,
	kVolumeControl_H = kLargeKnobSize,

	kWidthControl_X = 0,
	kWidthControl_Y = 15,
	kWidthControl_W = kLargeKnobSize,
	kWidthControl_H = kLargeKnobSize,

	kSeedControl_X = 0,
	kSeedControl_Y = 20,
	kSeedControl_W = 50,
	kSeedControl_H = kEnumHeight,

	kTimeGroup_X = 10,
	kTimeGroup_Y = kOscGroup_Y + kOscGroup_H + 10,
	kTimeGroup_W = 290,
	kTimeGroup_H = kOscGroup_H,

	kTempoControl_X = 0,
	kTempoControl_Y = 20,
	kTempoControl_W = 50,
	kTempoControl_H = kEnumHeight,

	kStepLengthControl_X = 0,
	kStepLengthControl_Y = 20,
	kStepLengthControl_W = 60,
	kStepLengthControl_H = kEnumHeight,

	kShuffleControl_X = 0,
	kShuffleControl_Y = 15,
	kShuffleControl_W = kLargeKnobSize,
	kShuffleControl_H = kLargeKnobSize,
	
	kPlayStateControl_X = 0,
	kPlayStateControl_Y = 15,
	kPlayStateControl_W = 100,
	kPlayStateControl_H = kLargeKnobSize,

	kPitchGroup_X = kTimeGroup_X + kTimeGroup_W + 10,
	kPitchGroup_Y = kTimeGroup_Y,
	kPitchGroup_W = 255,
	kPitchGroup_H = kOscGroup_H,

	kKeyControl_X = 0,
	kKeyControl_Y = 20,
	kKeyControl_W = 45,
	kKeyControl_H = kEnumHeight,

	kScaleControl_X = 0,
	kScaleControl_Y = 20,
	kScaleControl_W = 120,
	kScaleControl_H = kEnumHeight,

	kOctaveControl_X = 0,
	kOctaveControl_Y = 20,
	kOctaveControl_W = 20,
	kOctaveControl_H = kEnumHeight,

	kRangeControl_X = 0,
	kRangeControl_Y = 20,
	kRangeControl_W = 20,
	kRangeControl_H = kEnumHeight,

	kSequenceGroup_X = 10,
	kSequenceGroup_Y = kTimeGroup_Y + kTimeGroup_H + 10,
	kSequenceGroup_W = 760,
	kSequenceGroup_H = GUI_HEIGHT - 10 - kSequenceGroup_Y,

	kFirstKnobColumnX = kSequenceGroup_X + 60,

	kKnobLED_W = 8,
	kKnobLED_H = kKnobLED_W,
	kKnobLED_X = kFirstKnobColumnX - kKnobLED_W / 2,
	kKnobLED_Y = kSequenceGroup_Y + 25,

	kStepModeControl_W = 10,
	kStepModeControl_H = 30,
	kStepModeControl_X = kFirstKnobColumnX - kStepModeControl_W / 2,
	kStepModeControl_Y = kKnobLED_Y + kKnobLED_H + 12,

	kStepKnob_W = 30,
	kStepKnob_H = kStepKnob_W,
	kStepKnob_X = kFirstKnobColumnX - kStepKnob_W / 2,
	kStepKnob_Y = kStepModeControl_Y + kStepModeControl_H + 10,

	kStepKnobLabel_X = kSequenceGroup_X,
	kStepKnobLabel_Y = kStepKnob_Y + kStepKnob_H / 4,
	kStepKnobLabel_W = kFirstKnobColumnX - kStepKnobLabel_X - kStepKnob_W / 2,
	kStepKnobLabel_H = 8,

	kStepKnobColumnSpacing = 45,
	kStepKnobRowSpacing = 45,

	kStepRandomize_W = 22,
	kStepRandomize_H = 12,
	kStepRandomize_X = kStepKnobLabel_X + kStepKnobLabel_W / 2 - kStepRandomize_W / 2,
	kStepRandomize_Y = kStepKnobLabel_Y + kStepKnobLabel_H,
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

	const IColor GroupOutline = KnobLine;
	const IColor GroupLabel = Background;
}

namespace TextStyles
{
#ifdef OS_WIN
	const int ControlTextSize = 12;
	const int LabelTextSize = 12;
	char * ControlFont = 0;
	char * LabelFont = 0;
#else
	const int ControlTextSize = 14;
	const int LabelTextSize = 12;
	char * ControlFont = 0;
	char * LabelFont = "Helvetica Neue";
#endif
	// can't be const because of stupid ITextControl constructor
	IText  Label(LabelTextSize, &Color::Label, LabelFont, IText::kStyleBold, IText::kAlignCenter);
	IText  GroupLabel(LabelTextSize, &Color::GroupLabel, LabelFont, IText::kStyleBold, IText::kAlignNear);
	IText  Enum(ControlTextSize, &Color::Label, ControlFont, IText::kStyleNormal, IText::kAlignCenter, 0, IText::kQualityDefault, &Color::EnumBackground, &Color::EnumBorder);
	IText  TextBox(ControlTextSize, &Color::Label, ControlFont, IText::kStyleNormal, IText::kAlignCenter, 0, IText::kQualityDefault, &Color::EnumBackground, &Color::EnumBorder);
	IText  StepMode(ControlTextSize-2, &Color::Label, ControlFont, IText::kStyleNormal, IText::kAlignCenter, 0, IText::kQualityDefault, &Color::EnumBackground, &Color::EnumBorder);
	IText  RandLabel(ControlTextSize-2, &Color::Label, ControlFont, IText::kStyleNormal, IText::kAlignCenter);
}

namespace Strings
{
	const char * OscLabel = "Oscil";
	const char * EnvLabel = "Envelope";
	const char * MasterLabel = "Master";
	const char * TimeLabel = "Clock";
	const char * PitchLabel = "Pitch";
	const char * SequenceLabel = "Sequence";
	const char * RandomizeLabel = "rand";

	const char * VoicesLabel = "Voices";
	const char * SeedLabel = "Seed";
	const char * TempoLabel = "BPM";
	const char * StepLabel = "Step";
	const char * ShuffleLabel = "Swing";
	const char * KeyLabel = "Key";
	const char * ScaleLabel = "Scale";
	const char * OctaveLabel = "Oct.";
	const char * RangeLabel = "Range";

	const char * ProbabilityLabel = "P(N)";
	const char * PanLabel = "L-R";
	const char * VelocityLabel = "V";
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

	// Osc section
	{
		ControlGroup* group = new ControlGroup(mPlug, MakeIRect(kOscGroup), &Color::GroupOutline, &TextStyles::GroupLabel, Strings::OscLabel);
		pGraphics->AttachControl(group);

		IRECT rect = group->GetControlRect(MakeIRect(kWaveformControl));
		AttachEnum(pGraphics, rect, kWaveform, "Wave");
		
		int hoff = rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kPulseWidthControl, hoff));
		AttachKnob(pGraphics, rect, kPulseWidth, "P/W");

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kGlideControl, hoff));
		AttachKnob(pGraphics, rect, kGlide, "Glide");

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kMovementControl, hoff));
		AttachKnob(pGraphics, rect, kMovement, "Move");
	}

	// Env section
	{
		ControlGroup* group = new ControlGroup(mPlug, MakeIRect(kEnvGroup), &Color::GroupOutline, &TextStyles::GroupLabel, Strings::EnvLabel);
		pGraphics->AttachControl(group);

		IRECT rect = group->GetControlRect(MakeIRect(kEnvControl));
		AttachKnob(pGraphics, rect, kEnvAttack, "A");

		rect = group->GetControlRect(MakeIRectHOffset(kEnvControl, kEnvControl_S));
		AttachKnob(pGraphics, rect, kEnvDecay, "D");

		rect = group->GetControlRect(MakeIRectHOffset(kEnvControl, kEnvControl_S * 2));
		AttachKnob(pGraphics, rect, kEnvSustain, "S");

		rect = group->GetControlRect(MakeIRectHOffset(kEnvControl, kEnvControl_S * 3));
		AttachKnob(pGraphics, rect, kEnvRelease, "R");
	}

	// Master section
	{
		ControlGroup* group = new ControlGroup(mPlug, MakeIRect(kMasterGroup), &Color::GroupOutline, &TextStyles::GroupLabel, Strings::MasterLabel);
		pGraphics->AttachControl(group);

		IRECT rect = group->GetControlRect(MakeIRect(kVolumeControl));
		AttachKnob(pGraphics, rect, kVolume, "Vol");

		int hoff = rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kWidthControl, hoff));
		AttachKnob(pGraphics, rect, kWidth, "Width");

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kVoiceControl, hoff));
		AttachTextBox(pGraphics, rect, kVoices, 1.0f / (float)(kVoicesMax - kVoicesMin - 1), "000", Strings::VoicesLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kSeedControl, hoff));
		AttachTextBox(pGraphics, rect, kSeed, 0.001f, "000000", Strings::SeedLabel);
	}

	// Time section
	{
		ControlGroup* group = new ControlGroup(mPlug, MakeIRect(kTimeGroup), &Color::GroupOutline, &TextStyles::GroupLabel, Strings::TimeLabel);
		pGraphics->AttachControl(group);

		IRECT rect = group->GetControlRect(MakeIRect(kTempoControl));
		AttachTextBox(pGraphics, rect, kTempo, 0.01f, "000.000", Strings::TempoLabel);

		int hoff = rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kStepLengthControl, hoff));
		AttachEnum(pGraphics, rect, kStepLength, Strings::StepLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kShuffleControl, hoff));
		AttachKnob(pGraphics, rect, kShuffle, Strings::ShuffleLabel);
		
		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kPlayStateControl, hoff));
		pGraphics->AttachControl(new PlayStateControl(mPlug, rect, Color::LedOff, Color::LedOn));
	}

	// Pitch section
	{
		ControlGroup* group = new ControlGroup(mPlug, MakeIRect(kPitchGroup), &Color::GroupOutline, &TextStyles::GroupLabel, Strings::PitchLabel);
		pGraphics->AttachControl(group);

		IRECT rect = group->GetControlRect(MakeIRect(kKeyControl));
		AttachEnum(pGraphics, rect, kKey, Strings::KeyLabel);

		int hoff = rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kScaleControl, hoff));
		AttachEnum(pGraphics, rect, kScale, Strings::ScaleLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kOctaveControl, hoff));
		AttachTextBox(pGraphics, rect, kOctave, 1.0f / (float)(kOctaveMax - kOctaveMin - 1), "00", Strings::OctaveLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kRangeControl, hoff));
		AttachTextBox(pGraphics, rect, kRange, 1.0f / (float)(kRangeMax - kRangeMin - 1), "00", Strings::RangeLabel);
	}

	// sequence 
	{
		ControlGroup* group = new ControlGroup(mPlug, MakeIRect(kSequenceGroup), &Color::GroupOutline, &TextStyles::GroupLabel, Strings::SequenceLabel);
		pGraphics->AttachControl(group);

		mLEDs.reserve(kProbabilityLast - kProbabilityFirst + 1);

		// all them knobs
		for (int i = 0; i < kSequencerSteps; ++i)
		{
			const int hoffset = kStepKnobColumnSpacing*i;
			IRECT ledRect = MakeIRectHOffset(kKnobLED, hoffset);
			LED* led = new LED(mPlug, ledRect, Color::Background, Color::LedOn, Color::LedOff);
			mLEDs.push_back(led);
			pGraphics->AttachControl(led);

			// mode
			//pGraphics->AttachControl(new EnumControl(mPlug, MakeIRectHOffset(kStepModeControl, hoffset), kStepModeFirst + i, &TextStyles::Enum));
			pGraphics->AttachControl(new StepModeControl(mPlug, MakeIRectHOffset(kStepModeControl, hoffset), kStepModeFirst + i, &TextStyles::StepMode));
			
			// led behind the probability knob that blinks when a note is actually played
			ledRect = MakeIRectHOffset(kStepKnob, hoffset).GetPadded(-4);
			led = new LED(mPlug, ledRect, Color::Background, Color::LedOn, Color::Background);
			mNoteOns.push_back(led);
			pGraphics->AttachControl(led);
			// probability
			AttachKnob(pGraphics, MakeIRectHOffset(kStepKnob, hoffset), kProbabilityFirst + i);
			// pan
			AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 1), kPanFirst + i);
			// velocity
			AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 2), kVelocityFirst + i);
			// attack
			AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 3), kAttackFirst + i);
			// decay
			AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 4), kDecayFirst + i);
			// sustain
			AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 5), kSustainFirst + i);
			// release
			AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 6), kReleaseFirst + i);
		}
	}

	AttachStepRowLabel(pGraphics, 0, Strings::ProbabilityLabel);
	AttachStepRowLabel(pGraphics, 1, Strings::PanLabel);
	AttachStepRowLabel(pGraphics, 2, Strings::VelocityLabel);
	AttachStepRowLabel(pGraphics, 3, Strings::AttackLabel);
	AttachStepRowLabel(pGraphics, 4, Strings::DecayLabel);
	AttachStepRowLabel(pGraphics, 5, Strings::SustainLabel);
	AttachStepRowLabel(pGraphics, 6, Strings::ReleaseLabel);

	// randomizers
	AttachStepRowRandomizer(pGraphics, 0, kProbabilityRandomize);
	AttachStepRowRandomizer(pGraphics, 1, kPanRandomize);
	AttachStepRowRandomizer(pGraphics, 2, kVelocityRandomize);
	AttachStepRowRandomizer(pGraphics, 3, kAttackRandomize);
	AttachStepRowRandomizer(pGraphics, 4, kDecayRandomize);
	AttachStepRowRandomizer(pGraphics, 5, kSustainRandomize);
	AttachStepRowRandomizer(pGraphics, 6, kReleaseRandomize);
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
	pGraphics->AttachControl(new BangControl(mPlug, rect, param, Color::LedOn, Color::LedOff, &TextStyles::RandLabel, Strings::RandomizeLabel));
}

void Interface::AttachEnum(IGraphics* pGraphics, IRECT rect, int paramIdx, const char * label)
{
	pGraphics->AttachControl(new EnumControl(mPlug, rect, paramIdx, &TextStyles::Enum));

	if (label != nullptr)
	{
		rect.B = rect.T;
		rect.T -= 20;
		pGraphics->AttachControl(new ITextControl(mPlug, rect, &TextStyles::Label, const_cast<char*>(label)));
	}
}

void Interface::AttachTextBox(IGraphics* pGraphics, IRECT rect, const int paramIdx, const float scrollSpeed, const char * maxValue, const char * label /*= nullptr*/)
{
	pGraphics->AttachControl(new TextBox(mPlug, rect, paramIdx, &TextStyles::TextBox, pGraphics, maxValue, false, scrollSpeed));

	if (label != nullptr)
	{
		rect.B = rect.T;
		rect.T -= 20;
		pGraphics->AttachControl(new ITextControl(mPlug, rect, &TextStyles::Label, const_cast<char*>(label)));
	}

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
