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

//#define ENABLE_DUMP

enum ELayout
{
	kEnumHeight = 20,
	kButtonHeight = 15,
	kLargeKnobSize = 30,
	kSmallKnobSize = 20,

	kPlugTitle_W = 200,
	kPlugTitle_H = 15,
	kPlugTitle_X = GUI_WIDTH - 10 - kPlugTitle_W,
	kPlugTitle_Y = 5,

	kPresetRestoreControl_X = 10,
	kPresetRestoreControl_Y = 10,
	kPresetRestoreControl_W = 205,
	kPresetRestoreControl_H = kButtonHeight,

	kLoadControl_X = kPresetRestoreControl_X + kPresetRestoreControl_W + 10,
	kLoadControl_Y = kPresetRestoreControl_Y,
	kLoadControl_W = 50,
	kLoadControl_H = kButtonHeight,

	kSaveControl_X = kLoadControl_X + kLoadControl_W + 10,
	kSaveControl_Y = kLoadControl_Y,
	kSaveControl_W = kLoadControl_W,
	kSaveControl_H = kButtonHeight,

	kDumpControl_X = kSaveControl_X + kSaveControl_W + 10,
	kDumpControl_Y = kSaveControl_Y,
	kDumpControl_W = kSaveControl_W,
	kDumpControl_H = kButtonHeight,

	kOscGroup_X = 10,
	kOscGroup_Y = kPresetRestoreControl_Y + kPresetRestoreControl_H + 10,
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

	kEffectsGroup_X = kEnvGroup_X + kEnvGroup_W + 10,
	kEffectsGroup_Y = kEnvGroup_Y,
	kEffectsGroup_W = 385,
	kEffectsGroup_H = kOscGroup_H,

	kDelayDurationControl_X = 0,
	kDelayDurationControl_Y = 20,
	kDelayDurationControl_W = 60,
	kDelayDurationControl_H = kEnumHeight,

	kDelayFeedbackControl_X = 0,
	kDelayFeedbackControl_Y = 15,
	kDelayFeedbackControl_W = kLargeKnobSize,
	kDelayFeedbackControl_H = kLargeKnobSize,

	kDelayMixControl_X = 0,
	kDelayMixControl_Y = 15,
	kDelayMixControl_W = kLargeKnobSize,
	kDelayMixControl_H = kLargeKnobSize,

	// flanger controls are just a bunch of knobs, so we only need one rect
	kFlangerControl_X = 25,
	kFlangerControl_Y = 15,
	kFlangerControl_W = kLargeKnobSize,
	kFlangerControl_H = kLargeKnobSize,

	kTimeGroup_X = 10,
	kTimeGroup_Y = kOscGroup_Y + kOscGroup_H + 10,
	kTimeGroup_W = 325,
	kTimeGroup_H = kOscGroup_H,

	kClockSourceControl_X = 20,
	kClockSourceControl_Y = 20,
	kClockSourceControl_W = 10,
	kClockSourceControl_H = 20,

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

	kMasterGroup_W = 180,
	kMasterGroup_X = kPitchGroup_X + kPitchGroup_W + 10,
	kMasterGroup_Y = kOscGroup_Y + kOscGroup_H + 10,
	kMasterGroup_H = kOscGroup_H,

	kVoiceControl_X = 0,
	kVoiceControl_Y = 20,
	kVoiceControl_H = 20,
	kVoiceControl_W = 25,

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
	kSeedControl_W = 45,
	kSeedControl_H = kEnumHeight,

	kSequenceGroup_X = 10,
	kSequenceGroup_Y = kTimeGroup_Y + kTimeGroup_H + 10,
	kSequenceGroup_W = 780,
	kSequenceGroup_H = GUI_HEIGHT - 10 - kSequenceGroup_Y,

	kFirstKnobColumnX = kSequenceGroup_X + 65,

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

	kStepKnobColumnSpacing = 46,
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

	const IColor BangBackground = EnumBackground;
	const IColor BangActive = LedOn;

	const IColor GroupOutline = KnobLine;
	const IColor GroupLabel = Background;

	const IColor Title = LedOff;
}

namespace TextStyles
{
#ifdef OS_WIN
	const int ControlTextSize = 12;
	const int LabelTextSize = 12;
	const int ButtonTextSize = 12;
	char * ControlFont = 0;
	char * LabelFont = 0;
#else
	const int ControlTextSize = 14;
	const int LabelTextSize = 12;
	const int ButtonTextSize = 12;
	char * ControlFont = 0;
	char * LabelFont = "Helvetica Neue";
#endif
	// can't be const because of stupid ITextControl constructor
	IText  Title(LabelTextSize+8, &Color::Title, LabelFont, IText::kStyleBold, IText::kAlignFar);
	IText  Label(LabelTextSize, &Color::Label, LabelFont, IText::kStyleBold, IText::kAlignCenter);
	IText  GroupLabel(LabelTextSize, &Color::GroupLabel, LabelFont, IText::kStyleBold, IText::kAlignNear);
	IText  Enum(ControlTextSize, &Color::Label, ControlFont, IText::kStyleNormal, IText::kAlignCenter, 0, IText::kQualityDefault, &Color::EnumBackground, &Color::EnumBorder);
	IText  TextBox(ControlTextSize, &Color::Label, ControlFont, IText::kStyleNormal, IText::kAlignCenter, 0, IText::kQualityDefault, &Color::EnumBackground, &Color::EnumBorder);
	IText  StepMode(ControlTextSize-2, &Color::Label, ControlFont, IText::kStyleNormal, IText::kAlignCenter, 0, IText::kQualityDefault, &Color::EnumBackground, &Color::EnumBorder);
	IText  RandLabel(ControlTextSize-2, &Color::Label, ControlFont, IText::kStyleNormal, IText::kAlignCenter);
	IText  ButtonLabel(ButtonTextSize, &Color::Label, ControlFont, IText::kStyleNormal, IText::kAlignCenter);
}

namespace Strings
{
	const char * Title = PLUG_NAME " " VST3_VER_STR;
	const char * OscLabel = "Oscil";
	const char * EnvLabel = "Envelope";
	const char * EffectsLabel = "Effects";
	const char * MasterLabel = "Master";
	const char * TimeLabel = "Clock";
	const char * PitchLabel = "Pitch";
	const char * PresetsLabel = "Presets";
	const char * SequenceLabel = "Sequence";
	const char * RandomizeLabel = "rand";

	const char * VoicesLabel = "Voices";
	const char * SeedLabel = "Seed";
	const char * ClockSourceLabel = "Source";
	const char * TempoLabel = "BPM";
	const char * StepLabel = "Step";
	const char * ShuffleLabel = "Swing";
	const char * KeyLabel = "Key";
	const char * ScaleLabel = "Scale";
	const char * OctaveLabel = "Oct.";
	const char * RangeLabel = "Range";
	const char * LoadLabel = "Load...";
	const char * SaveLabel = "Save...";
	const char * DumpLabel = "Dump";

	const char * ProbabilityLabel = "P(N)";
	const char * PanLabel = "L-R";
	const char * VelocityLabel = "V";
	const char * AttackLabel = "A";
	const char * DecayLabel = "D";
	const char * SustainLabel = "S";
	const char * ReleaseLabel = "R";

	const char * DelayTimeLabel = "Delay";
	const char * DelayFeedbackLabel = "Feed";
	const char * DelayMixLabel = "Mix";

	const char * FlangerTimeLabel = "Flange";
	const char * FlangerRateLabel = "Rate";
	const char * FlangerDepthLabel = "Depth";
	const char * FlangerFeedbackLabel = "Feed";
	const char * FlangerMixLabel = "Mix";
}

Interface::Interface(Melodizer* inPlug)
	: mPlug(inPlug)
	, mLEDs()
	, mSequenceKnobValueControl(nullptr)
	, mPresetControl(nullptr)
	, mTempoControl(nullptr)
	, mKeyControl(nullptr)
	, mOctaveControl(nullptr)
	, mRangeControl(nullptr)
	, mPlayStateControl(nullptr)
{
}

Interface::~Interface()
{
	mPlug = nullptr;
	mLEDs.clear();
	mSequenceKnobValueControl = nullptr;
	mPresetControl = nullptr;
	mTempoControl = nullptr;
	mKeyControl = nullptr;
	mKeyControl = nullptr;
	mOctaveControl = nullptr;
	mRangeControl = nullptr;
	mPlayStateControl = nullptr;
}

void Interface::CreateControls(IGraphics* pGraphics)
{
	pGraphics->HandleMouseOver(true);

	pGraphics->AttachPanelBackground(&Color::Background);

	pGraphics->AttachControl(new ITextControl(mPlug, MakeIRect(kPlugTitle), &TextStyles::Title, Strings::Title));

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

	// Effects section
	{
		ControlGroup* group = new ControlGroup(mPlug, MakeIRect(kEffectsGroup), &Color::GroupOutline, &TextStyles::GroupLabel, Strings::EffectsLabel);
		pGraphics->AttachControl(group);

		IRECT rect = group->GetControlRect(MakeIRect(kDelayDurationControl));
		AttachEnum(pGraphics, rect, kDelayDuration, Strings::DelayTimeLabel);

		int hoff = rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kDelayFeedbackControl, hoff));
		AttachKnob(pGraphics, rect, kDelayFeedback, Strings::DelayFeedbackLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kDelayMixControl, hoff));
		AttachKnob(pGraphics, rect, kDelayMix, Strings::DelayMixLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kFlangerControl, hoff));
		AttachKnob(pGraphics, rect, kFlangerTime, Strings::FlangerTimeLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kFlangerControl, hoff));
		AttachKnob(pGraphics, rect, kFlangerRate, Strings::FlangerRateLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kFlangerControl, hoff));
		AttachKnob(pGraphics, rect, kFlangerDepth, Strings::FlangerDepthLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kFlangerControl, hoff));
		AttachKnob(pGraphics, rect, kFlangerFeedback, Strings::FlangerFeedbackLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kFlangerControl, hoff));
		AttachKnob(pGraphics, rect, kFlangerMix, Strings::FlangerMixLabel);
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

		IRECT rect = group->GetControlRect(MakeIRect(kClockSourceControl));
		//AttachEnum(pGraphics, rect, kClockSource, Strings::ClockSourceLabel);
		pGraphics->AttachControl(new StepModeControl(mPlug, rect, kClockSource, &TextStyles::StepMode));

		// adjust the rect to match what step mode control does
		rect.L -= 18;
		rect.R += 2;
		// shift above for the label
		rect.B = rect.T;
		rect.T -= kClockSourceControl_Y;
		pGraphics->AttachControl(new ITextControl(mPlug, rect, &TextStyles::Label, const_cast<char*>(Strings::ClockSourceLabel)));
		
		int hoff = rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kTempoControl, hoff));
		mTempoControl = AttachTextBox(pGraphics, rect, kTempo, 0.01f, "000.000", Strings::TempoLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kStepLengthControl, hoff));
		AttachEnum(pGraphics, rect, kStepLength, Strings::StepLabel);
		//AttachKnob(pGraphics, rect, kStepLength, Strings::StepLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kShuffleControl, hoff));
		AttachKnob(pGraphics, rect, kShuffle, Strings::ShuffleLabel);
		
		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kPlayStateControl, hoff));
		mPlayStateControl = new PlayStateControl(mPlug, rect, Color::LedOff, Color::LedOn);
		pGraphics->AttachControl(mPlayStateControl);
	}

	// Pitch section
	{
		ControlGroup* group = new ControlGroup(mPlug, MakeIRect(kPitchGroup), &Color::GroupOutline, &TextStyles::GroupLabel, Strings::PitchLabel);
		pGraphics->AttachControl(group);

		IRECT rect = group->GetControlRect(MakeIRect(kKeyControl));
		mKeyControl = AttachEnum(pGraphics, rect, kKey, Strings::KeyLabel);

		int hoff = rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kScaleControl, hoff));
		AttachEnum(pGraphics, rect, kScale, Strings::ScaleLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kOctaveControl, hoff));
		mOctaveControl = AttachTextBox(pGraphics, rect, kOctave, 1.0f / (float)(kOctaveMax - kOctaveMin - 1), "00", Strings::OctaveLabel);

		hoff += rect.W() + 10;
		rect = group->GetControlRect(MakeIRectHOffset(kRangeControl, hoff));
		mRangeControl = AttachTextBox(pGraphics, rect, kRange, 1.0f / (float)(kRangeMax - kRangeMin - 1), "00", Strings::RangeLabel);
	}

	// Presets section
	{
		pGraphics->AttachControl(new BangControl(mPlug, MakeIRect(kLoadControl), kLoadPreset, Color::BangActive, Color::BangBackground, &TextStyles::ButtonLabel, Strings::LoadLabel));
		pGraphics->AttachControl(new BangControl(mPlug, MakeIRect(kSaveControl), kSavePreset, Color::BangActive, Color::BangBackground, &TextStyles::ButtonLabel, Strings::SaveLabel));
#ifdef ENABLE_DUMP
		pGraphics->AttachControl(new BangControl(mPlug, MakeIRect(kDumpControl), kDumpPreset, Color::BangActive, Color::BangBackground, &TextStyles::ButtonLabel, Strings::DumpLabel));
#endif
		mPresetControl = AttachEnum(pGraphics, MakeIRect(kPresetRestoreControl), kRestorePreset);
	}

	// sequence 
	{
		ControlGroup* group = new ControlGroup(mPlug, MakeIRect(kSequenceGroup), &Color::GroupOutline, &TextStyles::GroupLabel, Strings::SequenceLabel);
		pGraphics->AttachControl(group);

		mSequenceKnobValueControl = new ITextControl(mPlug, MakeIRect(kStepKnobLabel), &TextStyles::Label, "");

		mLEDs.reserve(kProbabilityLast - kProbabilityFirst + 1);

		// all them knobs
		KnobLineCoronaControl* knob;
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
			knob = AttachKnob(pGraphics, MakeIRectHOffset(kStepKnob, hoffset), kProbabilityFirst + i);
			knob->SetLabelControl(mSequenceKnobValueControl, true);
			// pan
			knob = AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 1), kPanFirst + i);
			knob->SetLabelControl(mSequenceKnobValueControl, true);
			// velocity
			knob = AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 2), kVelocityFirst + i);
			knob->SetLabelControl(mSequenceKnobValueControl, true);
			// attack
			knob = AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 3), kAttackFirst + i);
			knob->SetLabelControl(mSequenceKnobValueControl, true);
			// decay
			knob = AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 4), kDecayFirst + i);
			knob->SetLabelControl(mSequenceKnobValueControl, true);
			// sustain
			knob = AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 5), kSustainFirst + i);
			knob->SetLabelControl(mSequenceKnobValueControl, true);
			// release
			knob = AttachKnob(pGraphics, MakeIRectHVOffset(kStepKnob, hoffset, kStepKnobRowSpacing * 6), kReleaseFirst + i);
			knob->SetLabelControl(mSequenceKnobValueControl, true);
		}

		// attach last so it draws on top of the knobs
		pGraphics->AttachControl(mSequenceKnobValueControl);
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
	pGraphics->AttachControl(new BangControl(mPlug, rect, param, Color::BangActive, Color::BangBackground, &TextStyles::RandLabel, Strings::RandomizeLabel));
}

void Interface::OnPlayStateChanged()
{
	if (mPlayStateControl != nullptr)
	{
		// have to call both SetDirty and Redraw because OnPlayStateChanged
		// is called from the audio thread. see comment in LED::Blink.
		mPlayStateControl->SetDirty();
		mPlayStateControl->Redraw();
	}
}

IControl* Interface::AttachEnum(IGraphics* pGraphics, IRECT rect, const int paramIdx, const char * label /*= nullptr*/)
{
	IControl* control = new EnumControl(mPlug, rect, paramIdx, &TextStyles::Enum);
	pGraphics->AttachControl(control);

	if (label != nullptr)
	{
		rect.B = rect.T;
		rect.T -= 20;
		pGraphics->AttachControl(new ITextControl(mPlug, rect, &TextStyles::Label, const_cast<char*>(label)));
	}

	return control;
}

IControl* Interface::AttachTextBox(IGraphics* pGraphics, IRECT rect, const int paramIdx, const float scrollSpeed, const char * maxValue, const char * label /*= nullptr*/)
{
	IControl* control = new TextBox(mPlug, rect, paramIdx, &TextStyles::TextBox, pGraphics, maxValue, false, scrollSpeed);
	pGraphics->AttachControl(control);

	if (label != nullptr)
	{
		rect.B = rect.T;
		rect.T -= 20;
		pGraphics->AttachControl(new ITextControl(mPlug, rect, &TextStyles::Label, const_cast<char*>(label)));
	}

	return control;
}

KnobLineCoronaControl* Interface::AttachKnob(IGraphics* pGraphics, IRECT rect, const int paramIdx, const char * label /*= nullptr*/)
{
	KnobLineCoronaControl* knob = new KnobLineCoronaControl(mPlug, rect, paramIdx, &Color::KnobLine, &Color::KnobCorona);
	pGraphics->AttachControl(knob);
	
	if (label != nullptr)
	{
		rect.B = rect.T;
		rect.T -= 15;
		rect.L -= 15;
		rect.R += 15;
		ITextControl* labelControl = new ITextControl(mPlug, rect, &TextStyles::Label, const_cast<char*>(label));
		pGraphics->AttachControl(labelControl);
		knob->SetLabelControl(labelControl);
	}

	return knob;
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

void Interface::OnClockSourceChanged(const int source)
{
	if (mTempoControl != nullptr)
	{
		mTempoControl->GrayOut(source != CS_Internal);
	}
}

void Interface::OnPresetChanged()
{
	if (mPresetControl != nullptr)
	{
		mPresetControl->SetDirty(false);
		mPresetControl->Redraw();
	}
}

void Interface::SetFingeredMode(bool enabled)
{
	if (mKeyControl != nullptr)
	{
		mKeyControl->GrayOut(enabled);
	}

	if (mOctaveControl != nullptr)
	{
		mOctaveControl->GrayOut(enabled);
	}

	if (mRangeControl != nullptr)
	{
		mRangeControl->GrayOut(enabled);
	}
}
