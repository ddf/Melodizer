#pragma once

enum EParams
{
	kWaveform,
	kScale,
	kKey,
	kTempo,
	kOctave,
	kRange,

	kProbabilityFirst,
	kProbabilityLast = kProbabilityFirst + 15,

	kNumParams
};

enum EParamSettings
{
	kTempoMin = 1,
	kTempoMax = 960,
};

enum WaveformType
{
	WT_Sine = 0,
	WT_Triangle,
	WT_Saw,
	WT_Square,
	WT_Quarterpulse,
	WT_Sine4,
	WT_Sine8,
	WT_Sine16,
	WT_Sine32,
	WT_Count,
};
