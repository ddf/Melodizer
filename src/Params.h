#pragma once

#define STEP_PARAM(paramName) paramName##First, paramName##Last = paramName##First + kSequencerSteps - 1, paramName##Randomize

enum EParamSettings
{
	// these are in dB
	kVolumeMin = -48,
	kVolumeMax = 6,

	kTempoMin = 2,
	kTempoMax = 960,
	
	kVoicesMin = 1,
	kVoicesMax = 32,

	kOctaveMin = -1,
	kOctaveMax = 9,

	kRangeMin  = 1,
	kRangeMax  = 10,

	kSeedMin   = 0,
	kSeedMax   = 1<<15,

	// this is used to set the maximum delay time of the Delay UGen
	// and also used to change the delay duration param if the tempo
	// gets slow enough that the current value would generate a longer delay than this.
	kDelayDurationMaxSeconds = 2,
	
	kSequencerSteps = 16,
};

enum EParams
{
	// synth params
	kWaveform = 0,
	kPulseWidth,
	kGlide,
	kMovement,
	
	// envelope
	kEnvAttack,
	kEnvDecay,
	kEnvSustain,
	kEnvRelease,
	
	// master params
	kVoices,
	kVolume,
	kWidth,
	kSeed,
	
	// sequence time params
	kClockSource,
	kTempo,
	kStepLength,
	kShuffle,
	kPlayState,
	
	// sequence pitch params
	kKey,
	kScale,
	kOctave,
	kRange,

	// effects - it's important that DelayDuration comes after Tempo 
	// because it might cap its value in OnParamChange based on Tempo.
	kDelayDuration,
	kDelayFeedback,
	kDelayMix,

	kFlangerTime,
	kFlangerRate,
	kFlangerDepth,
	kFlangerFeedback,
	kFlangerMix,
	
	// step params
	STEP_PARAM(kStepMode), // see: enum StepMode
	STEP_PARAM(kProbability),
	STEP_PARAM(kPan),
	STEP_PARAM(kVelocity),
	STEP_PARAM(kAttack),
	STEP_PARAM(kDecay),
	STEP_PARAM(kSustain),
	STEP_PARAM(kRelease),

	kNumParams,

	kSavePreset,
	kLoadPreset,
	kRestorePreset,
	kDumpPreset,
};

enum ClockSource
{
	CS_Internal = 0,
	CS_External,

	CS_Count,
};

enum StepMode
{
	SM_Norm = 0,
	SM_Skip,
	SM_Loop,

	SM_Count
};

// it's important that these are in order of shortest to longest durations,
// which is why 32T is before 64D, for example.
// putting them in order makes it easier to adjust the delay duration param
// if the tempo gets too slow (we can't let the duration get longer than kDelayDurationMaxSeconds)
enum StepLength
{
	SL_64T,
	SL_64,
	SL_32T,
	SL_64D,
	SL_32,
	SL_16T,
	SL_32D,
	SL_16,
	SL_8T,
	SL_16D,
	SL_8,
	SL_4T,
	SL_8D,
	SL_4,
	SL_4D,
	
	SL_Count,
};

enum PlayState
{
	PS_Stop,
	PS_Pause,
	PS_Play,
	
	PS_Count,
};

enum WaveformType
{
	WT_Sine = 0,
	WT_Triangle,
	WT_Saw,
	WT_Square,
	WT_Sine4,
	WT_Sine8,
	WT_Sine16,
	WT_Sine32,
	WT_Count,
};
