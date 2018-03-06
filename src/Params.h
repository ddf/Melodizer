#pragma once

#define STEP_PARAM(paramName) paramName##First, paramName##Last = paramName##First + kSequencerSteps - 1, paramName##Randomize

enum EParamSettings
{
	kTempoMin = 1,
	kTempoMax = 960,
	
	kVoicesMin = 1,
	kVoicesMax = 32,

	kOctaveMin = -1,
	kOctaveMax = 9,

	kRangeMin  = 1,
	kRangeMax  = 10,

	kSeedMin   = 0,
	kSeedMax   = 1<<15,
	
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
	
	// sequenc time params
	kTempo,
	kStepLength,
	kShuffle,
	kPlayState,
	
	// sequence pitch params
	kKey,
	kScale,
	kOctave,
	kRange,
	
	// step params
	STEP_PARAM(kStepMode), // see: enum StepMode
	STEP_PARAM(kProbability),
	STEP_PARAM(kPan),
	STEP_PARAM(kVelocity),
	STEP_PARAM(kAttack),
	STEP_PARAM(kDecay),
	STEP_PARAM(kSustain),
	STEP_PARAM(kRelease),

	kNumParams
};

enum StepMode
{
	SM_Norm = 0,
	SM_Skip,
	SM_Loop,

	SM_Count
};

enum StepLength
{
	SL_4,
	SL_4T,
	SL_4D,
	SL_8,
	SL_8T,
	SL_8D,
	SL_16,
	SL_16T,
	SL_16D,
	SL_32,
	SL_32T,
	SL_32D,
	SL_64,
	SL_64T,
	SL_64D,
	
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
