#pragma once

#define STEP_PARAM(paramName) paramName##First, paramName##Last = paramName##First + kSequencerSteps - 1, paramName##Randomize

enum EParamSettings
{
	kTempoMin = 1,
	kTempoMax = 960,
	
	kVoicesMin = 1,
	kVoicesMax = 32,
	
	kSequencerSteps = 16,
};

enum EParams
{
	// synth params
	kWaveform = 0,
	kPulseWidth,
	
	kEnvAttack,
	kEnvDecay,
	kEnvSustain,
	kEnvRelease,
	
	kVoices,
	kVolume,
	kWidth,
	kGlide,
	kMovement,
	
	// global sequence params
	kScale,
	kKey,
	kTempo,
	kStepLength,
	kOctave,
	kRange,
	kShuffle,
	kSeed,
	
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
	SL_8,
	SL_16,
	SL_32,
	SL_64,
	
	SL_Count,
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
