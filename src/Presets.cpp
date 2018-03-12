#include "Melodizer.h"

extern const int kNumPrograms = 11;

void Melodizer::MakePresets()
{
	MakeDefaultPreset((char *) "-", 1);

	MakePresetFromNamedParams("gentle tones", 62
		, 0, 1 // Waveform
		, 5, 0.553625 // Decay
		, 6, 50.833333 // Sustain
		, 7, 0.541375 // Release
		, 9, -5.999999 // Volume
		, 10, 50.833333 // Width
		, 13, 99.999998 // Tempo
		, 18, 2 // Scale
		, 19, 3 // Octave
		, 22, 21.666665 // Delay Feedback
		, 23, 10.833333 // Delay Mix
		, 24, 0.010000 // Flanger Delay
		, 25, 0.010000 // Flanger Rate
		, 26, 10.000000 // Flanger Depth
		, 46, 81.505883 // Step 0 Probability
		, 47, 96.133959 // Step 1 Probability
		, 48, 87.866420 // Step 2 Probability
		, 49, 29.115185 // Step 3 Probability
		, 50, 24.232674 // Step 4 Probability
		, 51, 66.030997 // Step 5 Probability
		, 52, 17.607604 // Step 6 Probability
		, 53, 30.443329 // Step 7 Probability
		, 54, 44.379771 // Step 8 Probability
		, 55, 40.164167 // Step 9 Probability
		, 56, 83.837509 // Step 10 Probability
		, 57, 69.951588 // Step 11 Probability
		, 58, 6.153756 // Step 12 Probability
		, 59, 69.015664 // Step 13 Probability
		, 60, 42.355290 // Step 14 Probability
		, 61, 12.327853 // Step 15 Probability
		, 63, 0.809779 // Step 0 Pan
		, 64, -0.696953 // Step 1 Pan
		, 65, -0.895831 // Step 2 Pan
		, 66, -0.143678 // Step 3 Pan
		, 67, 0.056237 // Step 4 Pan
		, 68, -0.431758 // Step 5 Pan
		, 69, -0.826873 // Step 6 Pan
		, 70, 0.806023 // Step 7 Pan
		, 71, 0.381208 // Step 8 Pan
		, 72, -0.275756 // Step 9 Pan
		, 73, 0.534400 // Step 10 Pan
		, 74, -0.646070 // Step 11 Pan
		, 75, 0.621145 // Step 12 Pan
		, 76, -0.558043 // Step 13 Pan
		, 77, -0.724800 // Step 14 Pan
		, 78, 0.313453 // Step 15 Pan
		, 80, 76.104969 // Step 0 Velocity
		, 81, 84.062886 // Step 1 Velocity
		, 82, 36.778101 // Step 2 Velocity
		, 83, 57.354820 // Step 3 Velocity
		, 84, 34.680039 // Step 4 Velocity
		, 85, 30.316326 // Step 5 Velocity
		, 86, 49.938861 // Step 6 Velocity
		, 87, 74.422139 // Step 7 Velocity
		, 88, 13.015202 // Step 8 Velocity
		, 89, 77.286619 // Step 9 Velocity
		, 90, 96.849668 // Step 10 Velocity
		, 91, 97.612524 // Step 11 Velocity
		, 92, 67.503840 // Step 12 Velocity
		, 93, 85.845733 // Step 13 Velocity
		, 94, 96.307379 // Step 14 Velocity
		, 95, 65.247780 // Step 15 Velocity
	);


	MakePresetFromNamedParams("creepy bassoon solo", 34
		, 0, 7 // Waveform
		, 1, 14.883333 // Pulse Width
		, 4, 0.054875 // Attack
		, 5, 0.370750 // Decay
		, 6, 24.748744 // Sustain
		, 7, 0.250000 // Release
		, 9, -5.999999 // Volume
		, 13, 119.999997 // Tempo
		, 18, 9 // Scale
		, 19, 2 // Octave
		, 20, 2 // Range
		, 21, 12 // Delay Duration
		, 22, 27.500001 // Delay Feedback
		, 23, 15.833333 // Delay Mix
		, 24, 2.092917 // Flanger Delay
		, 25, 0.010000 // Flanger Rate
		, 26, 10.000000 // Flanger Depth
		, 28, 16.666667 // Flanger Mix
		, 46, 14.420417 // Step 0 Probability
		, 47, 33.662027 // Step 1 Probability
		, 48, 40.716177 // Step 2 Probability
		, 49, 88.924181 // Step 3 Probability
		, 50, 9.738855 // Step 4 Probability
		, 51, 22.402623 // Step 5 Probability
		, 52, 51.103890 // Step 6 Probability
		, 53, 21.023326 // Step 7 Probability
		, 54, 22.776690 // Step 8 Probability
		, 55, 93.041098 // Step 9 Probability
		, 56, 63.811809 // Step 10 Probability
		, 57, 59.337205 // Step 11 Probability
		, 58, 82.438087 // Step 12 Probability
		, 59, 32.286206 // Step 13 Probability
		, 60, 73.490095 // Step 14 Probability
		, 61, 12.911901 // Step 15 Probability
	);

	MakePresetFromNamedParams("digital icicles", 81
		, 0, 3 // Waveform
		, 1, 9.166667 // Pulse Width
		, 5, 0.154625 // Decay
		, 6, 7.248744 // Sustain
		, 7, 0.296375 // Release
		, 9, -5.999999 // Volume
		, 13, 119.999997 // Tempo
		, 14, 2 // Step Length
		, 18, 7 // Scale
		, 19, 6 // Octave
		, 20, 4 // Range
		, 21, 13 // Delay Duration
		, 22, 43.333334 // Delay Feedback
		, 23, 5.000000 // Delay Mix
		, 24, 0.010000 // Flanger Delay
		, 25, 0.010000 // Flanger Rate
		, 26, 10.000000 // Flanger Depth
		, 46, 61.595070 // Step 0 Probability
		, 47, 71.017784 // Step 1 Probability
		, 48, 35.132623 // Step 2 Probability
		, 49, 24.433337 // Step 3 Probability
		, 50, 23.153098 // Step 4 Probability
		, 51, 29.064947 // Step 5 Probability
		, 52, 42.290011 // Step 6 Probability
		, 53, 70.966804 // Step 7 Probability
		, 54, 78.094184 // Step 8 Probability
		, 55, 3.415673 // Step 9 Probability
		, 56, 33.747050 // Step 10 Probability
		, 57, 30.541351 // Step 11 Probability
		, 58, 22.614306 // Step 12 Probability
		, 59, 37.509450 // Step 13 Probability
		, 60, 19.166666 // Step 14 Probability
		, 61, 13.811614 // Step 15 Probability
		, 63, 0.160250 // Step 0 Pan
		, 64, 0.066562 // Step 1 Pan
		, 65, 0.215581 // Step 2 Pan
		, 66, 0.591035 // Step 3 Pan
		, 67, -0.194723 // Step 4 Pan
		, 68, -0.051258 // Step 5 Pan
		, 69, 0.552480 // Step 6 Pan
		, 70, -0.259784 // Step 7 Pan
		, 71, 0.064065 // Step 8 Pan
		, 72, 0.898827 // Step 9 Pan
		, 73, 0.226092 // Step 10 Pan
		, 74, -0.132042 // Step 11 Pan
		, 75, -0.490745 // Step 12 Pan
		, 76, 0.768250 // Step 13 Pan
		, 77, 0.034482 // Step 14 Pan
		, 78, -0.305099 // Step 15 Pan
		, 80, 10.273206 // Step 0 Velocity
		, 81, 2.601163 // Step 1 Velocity
		, 82, 70.819486 // Step 2 Velocity
		, 83, 92.345379 // Step 3 Velocity
		, 84, 91.396450 // Step 4 Velocity
		, 85, 88.666418 // Step 5 Velocity
		, 86, 36.361028 // Step 6 Velocity
		, 87, 86.847877 // Step 7 Velocity
		, 88, 32.923440 // Step 8 Velocity
		, 89, 98.242123 // Step 9 Velocity
		, 90, 40.754564 // Step 10 Velocity
		, 91, 3.678966 // Step 11 Velocity
		, 92, 87.119489 // Step 12 Velocity
		, 93, 15.827751 // Step 13 Velocity
		, 94, 8.157894 // Step 14 Velocity
		, 95, 28.151148 // Step 15 Velocity
		, 114, 84.467361 // Step 0 Decay
		, 115, 74.610494 // Step 1 Decay
		, 116, 83.085703 // Step 2 Decay
		, 117, 79.695463 // Step 3 Decay
		, 118, 83.214770 // Step 4 Decay
		, 119, 71.590403 // Step 5 Decay
		, 120, 84.142940 // Step 6 Decay
		, 121, 81.587800 // Step 7 Decay
		, 122, 53.717845 // Step 8 Decay
		, 123, 55.527287 // Step 9 Decay
		, 124, 79.205596 // Step 10 Decay
		, 125, 78.730118 // Step 11 Decay
		, 126, 81.009738 // Step 12 Decay
		, 127, 67.170799 // Step 13 Decay
		, 128, 81.287926 // Step 14 Decay
		, 129, 84.415056 // Step 15 Decay
	);

	MakePresetFromNamedParams("meandering steel drum", 62
		, 0, 0 // Waveform
		, 1, 44.283333 // Pulse Width
		, 5, 0.221125 // Decay
		, 6, 49.748743 // Sustain
		, 7, 0.832750 // Release
		, 10, 30.833333 // Width
		, 13, 100.000000 // Tempo
		, 14, 7 // Step Length
		, 18, 9 // Scale
		, 22, 20.000000 // Delay Feedback
		, 24, 5.425583 // Flanger Delay
		, 25, 4.674333 // Flanger Rate
		, 26, 10.000000 // Flanger Depth
		, 28, 32.500000 // Flanger Mix
		, 46, 87.225450 // Step 0 Probability
		, 47, 97.575121 // Step 1 Probability
		, 48, 50.731056 // Step 2 Probability
		, 49, 82.441677 // Step 3 Probability
		, 50, 55.436407 // Step 4 Probability
		, 51, 58.667130 // Step 5 Probability
		, 52, 20.942310 // Step 6 Probability
		, 53, 68.141601 // Step 7 Probability
		, 54, 74.200160 // Step 8 Probability
		, 55, 41.005688 // Step 9 Probability
		, 56, 42.421721 // Step 10 Probability
		, 57, 4.569941 // Step 11 Probability
		, 58, 7.415620 // Step 12 Probability
		, 59, 93.109092 // Step 13 Probability
		, 60, 95.096820 // Step 14 Probability
		, 61, 88.956506 // Step 15 Probability
		, 63, 0.634874 // Step 0 Pan
		, 64, -0.457298 // Step 1 Pan
		, 65, 0.889959 // Step 2 Pan
		, 66, 0.936542 // Step 3 Pan
		, 67, 0.250438 // Step 4 Pan
		, 68, 0.779551 // Step 5 Pan
		, 69, 0.265491 // Step 6 Pan
		, 70, -0.420244 // Step 7 Pan
		, 71, -0.524523 // Step 8 Pan
		, 72, 0.134662 // Step 9 Pan
		, 73, 0.301070 // Step 10 Pan
		, 74, 0.151425 // Step 11 Pan
		, 75, -0.815755 // Step 12 Pan
		, 76, 0.929088 // Step 13 Pan
		, 77, -0.620210 // Step 14 Pan
		, 78, -0.533243 // Step 15 Pan
		, 80, 79.488843 // Step 0 Velocity
		, 81, 72.486264 // Step 1 Velocity
		, 82, 82.256980 // Step 2 Velocity
		, 83, 48.906553 // Step 3 Velocity
		, 84, 86.949070 // Step 4 Velocity
		, 85, 89.411449 // Step 5 Velocity
		, 86, 9.085049 // Step 6 Velocity
		, 87, 85.075150 // Step 7 Velocity
		, 88, 49.311059 // Step 8 Velocity
		, 89, 57.669988 // Step 9 Velocity
		, 90, 91.892600 // Step 10 Velocity
		, 91, 96.132473 // Step 11 Velocity
		, 92, 68.392927 // Step 12 Velocity
		, 93, 55.793332 // Step 13 Velocity
		, 94, 70.561934 // Step 14 Velocity
		, 95, 74.323321 // Step 15 Velocity
	);

	MakePresetFromNamedParams("stereo sparkle", 99
		, 1, 9.166667 // Pulse Width
		, 3, 1.016667 // Movement
		, 4, 0.088125 // Attack
		, 5, 0.221125 // Decay
		, 6, 48.082079 // Sustain
		, 7, 4.246000 // Release
		, 9, -5.999999 // Volume
		, 13, 130.236216 // Tempo
		, 18, 9 // Scale
		, 19, 3 // Octave
		, 20, 3 // Range
		, 21, 7 // Delay Duration
		, 22, 79.166669 // Delay Feedback
		, 23, 23.333333 // Delay Mix
		, 24, 3.946220 // Flanger Delay
		, 25, 6.778267 // Flanger Rate
		, 26, 3.149606 // Flanger Depth
		, 27, 53.543305 // Flanger Feedback
		, 28, 64.790028 // Flanger Mix
		, 46, 85.642165 // Step 0 Probability
		, 47, 80.374694 // Step 1 Probability
		, 48, 80.457878 // Step 2 Probability
		, 49, 85.899150 // Step 3 Probability
		, 50, 71.043092 // Step 4 Probability
		, 51, 6.876785 // Step 5 Probability
		, 52, 68.128133 // Step 6 Probability
		, 53, 63.424706 // Step 7 Probability
		, 54, 71.022564 // Step 8 Probability
		, 55, 75.415826 // Step 9 Probability
		, 56, 17.034370 // Step 10 Probability
		, 57, 66.667056 // Step 11 Probability
		, 58, 17.507836 // Step 12 Probability
		, 59, 42.840424 // Step 13 Probability
		, 60, 88.377339 // Step 14 Probability
		, 61, 11.608981 // Step 15 Probability
		, 63, -1.000000 // Step 0 Pan
		, 64, -0.835078 // Step 1 Pan
		, 65, -0.728304 // Step 2 Pan
		, 66, -0.537595 // Step 3 Pan
		, 67, -0.526622 // Step 4 Pan
		, 68, -0.462304 // Step 5 Pan
		, 69, -0.118283 // Step 6 Pan
		, 70, 0.007176 // Step 7 Pan
		, 71, 0.205919 // Step 8 Pan
		, 72, 0.408876 // Step 9 Pan
		, 73, 0.572116 // Step 10 Pan
		, 74, 0.662983 // Step 11 Pan
		, 75, 0.765621 // Step 12 Pan
		, 76, 0.766667 // Step 13 Pan
		, 77, 0.840863 // Step 14 Pan
		, 78, 1.000000 // Step 15 Pan
		, 80, 66.339886 // Step 0 Velocity
		, 81, 99.059647 // Step 1 Velocity
		, 82, 49.869245 // Step 2 Velocity
		, 83, 99.593526 // Step 3 Velocity
		, 84, 52.540761 // Step 4 Velocity
		, 85, 60.754550 // Step 5 Velocity
		, 86, 31.512186 // Step 6 Velocity
		, 87, 77.506554 // Step 7 Velocity
		, 88, 94.791538 // Step 8 Velocity
		, 89, 74.751246 // Step 9 Velocity
		, 90, 93.330473 // Step 10 Velocity
		, 91, 58.861083 // Step 11 Velocity
		, 92, 68.316114 // Step 12 Velocity
		, 93, 28.946224 // Step 13 Velocity
		, 94, 71.996266 // Step 14 Velocity
		, 95, 61.401415 // Step 15 Velocity
		, 97, 39.241717 // Step 0 Attack
		, 98, 14.754944 // Step 1 Attack
		, 99, 44.454784 // Step 2 Attack
		, 100, 77.701335 // Step 3 Attack
		, 101, 7.706121 // Step 4 Attack
		, 102, 36.088404 // Step 5 Attack
		, 103, 88.564269 // Step 6 Attack
		, 104, 1.490551 // Step 7 Attack
		, 105, 56.718149 // Step 8 Attack
		, 106, 10.266634 // Step 9 Attack
		, 107, 86.124435 // Step 10 Attack
		, 108, 11.036531 // Step 11 Attack
		, 109, 76.594930 // Step 12 Attack
		, 110, 23.713056 // Step 13 Attack
		, 111, 47.747945 // Step 14 Attack
		, 112, 3.784183 // Step 15 Attack
		, 148, 70.775256 // Step 0 Release
		, 149, 34.673944 // Step 1 Release
		, 150, 60.787387 // Step 2 Release
		, 151, 85.788640 // Step 3 Release
		, 152, 75.149148 // Step 4 Release
		, 153, 46.300837 // Step 5 Release
		, 154, 17.757446 // Step 6 Release
		, 155, 24.682773 // Step 7 Release
		, 156, 73.531778 // Step 8 Release
		, 157, 60.214438 // Step 9 Release
		, 158, 93.895484 // Step 10 Release
		, 159, 78.691456 // Step 11 Release
		, 160, 18.522187 // Step 12 Release
		, 161, 53.028349 // Step 13 Release
		, 162, 27.121891 // Step 14 Release
		, 163, 71.054707 // Step 15 Release
	);

	MakePresetFromNamedParams("melancholy violin", 104
		, 0, 5 // Waveform
		, 1, 36.525001 // Pulse Width
		, 3, 0.716667 // Movement
		, 4, 0.470500 // Attack
		, 5, 0.487125 // Decay
		, 6, 38.082077 // Sustain
		, 7, 1.498750 // Release
		, 9, -5.999999 // Volume
		, 13, 80.000000 // Tempo
		, 15, 90.000000 // Shuffle
		, 17, 9 // Key
		, 18, 5 // Scale
		, 19, 3 // Octave
		, 20, 2 // Range
		, 21, 7 // Delay Duration
		, 22, 0.000000 // Delay Feedback
		, 24, 0.010000 // Flanger Delay
		, 25, 0.010000 // Flanger Rate
		, 26, 0.000000 // Flanger Depth
		, 27, 3.333333 // Flanger Feedback
		, 37, 1 // Step 8 Mode
		, 46, 91.146186 // Step 0 Probability
		, 47, 14.954621 // Step 1 Probability
		, 48, 83.743741 // Step 2 Probability
		, 49, 58.910723 // Step 3 Probability
		, 50, 58.273583 // Step 4 Probability
		, 51, 73.006193 // Step 5 Probability
		, 52, 7.217921 // Step 6 Probability
		, 53, 34.361648 // Step 7 Probability
		, 54, 60.434461 // Step 8 Probability
		, 55, 84.408041 // Step 9 Probability
		, 56, 96.396844 // Step 10 Probability
		, 57, 52.596442 // Step 11 Probability
		, 58, 93.396107 // Step 12 Probability
		, 59, 68.403406 // Step 13 Probability
		, 60, 29.297390 // Step 14 Probability
		, 61, 83.994766 // Step 15 Probability
		, 72, -0.130914 // Step 9 Pan
		, 80, 53.887337 // Step 0 Velocity
		, 81, 8.000510 // Step 1 Velocity
		, 82, 73.590993 // Step 2 Velocity
		, 83, 24.141426 // Step 3 Velocity
		, 84, 58.745764 // Step 4 Velocity
		, 85, 98.675871 // Step 5 Velocity
		, 86, 50.941393 // Step 6 Velocity
		, 87, 1.490327 // Step 7 Velocity
		, 88, 44.951815 // Step 8 Velocity
		, 89, 51.566663 // Step 9 Velocity
		, 90, 94.612569 // Step 10 Velocity
		, 91, 30.829668 // Step 11 Velocity
		, 92, 82.686898 // Step 12 Velocity
		, 93, 39.760171 // Step 13 Velocity
		, 94, 64.541392 // Step 14 Velocity
		, 95, 4.863929 // Step 15 Velocity
		, 97, 88.239274 // Step 0 Attack
		, 98, 90.226559 // Step 1 Attack
		, 99, 75.593259 // Step 2 Attack
		, 100, 83.234760 // Step 3 Attack
		, 101, 84.629464 // Step 4 Attack
		, 102, 86.894019 // Step 5 Attack
		, 103, 92.632806 // Step 6 Attack
		, 104, 90.925000 // Step 7 Attack
		, 105, 96.852299 // Step 8 Attack
		, 106, 87.820933 // Step 9 Attack
		, 107, 83.043201 // Step 10 Attack
		, 108, 94.311674 // Step 11 Attack
		, 109, 92.257481 // Step 12 Attack
		, 110, 90.229977 // Step 13 Attack
		, 111, 76.673063 // Step 14 Attack
		, 112, 85.239771 // Step 15 Attack
		, 114, 18.708070 // Step 0 Decay
		, 115, 18.211800 // Step 1 Decay
		, 116, 91.352844 // Step 2 Decay
		, 117, 93.781828 // Step 3 Decay
		, 118, 10.253165 // Step 4 Decay
		, 119, 26.752236 // Step 5 Decay
		, 120, 77.266419 // Step 6 Decay
		, 121, 25.829671 // Step 7 Decay
		, 122, 13.380269 // Step 8 Decay
		, 123, 65.085970 // Step 9 Decay
		, 124, 20.648772 // Step 10 Decay
		, 125, 6.623456 // Step 11 Decay
		, 126, 81.474730 // Step 12 Decay
		, 127, 60.987051 // Step 13 Decay
		, 128, 75.739000 // Step 14 Decay
		, 129, 42.522120 // Step 15 Decay
		, 131, 99.538216 // Step 0 Sustain
		, 132, 67.184404 // Step 1 Sustain
		, 133, 48.139087 // Step 2 Sustain
		, 134, 71.497354 // Step 3 Sustain
		, 135, 61.337833 // Step 4 Sustain
		, 136, 91.288207 // Step 5 Sustain
		, 137, 53.658122 // Step 6 Sustain
		, 138, 57.925000 // Step 7 Sustain
		, 139, 35.317335 // Step 8 Sustain
		, 140, 65.482789 // Step 9 Sustain
		, 141, 58.445555 // Step 10 Sustain
		, 142, 64.718322 // Step 11 Sustain
		, 143, 60.687044 // Step 12 Sustain
		, 144, 59.355279 // Step 13 Sustain
		, 145, 67.468050 // Step 14 Sustain
		, 146, 44.810092 // Step 15 Sustain
		, 150, 96.700000 // Step 2 Release
		, 152, 96.700000 // Step 4 Release
	);

	MakePresetFromNamedParams("chiptune", 55
		, 0, 3 // Waveform
		, 1, 25.908333 // Pulse Width
		, 2, 0.033333 // Glide
		, 5, 0.186213 // Decay
		, 6, 10.000000 // Sustain
		, 7, 0.213125 // Release
		, 9, -5.999999 // Volume
		, 13, 132.000003 // Tempo
		, 14, 7 // Step Length
		, 16, 2 // Play State
		, 17, 2 // Key
		, 18, 3 // Scale
		, 20, 2 // Range
		, 21, 7 // Delay Duration
		, 22, 47.499999 // Delay Feedback
		, 23, 23.333333 // Delay Mix
		, 24, 0.010000 // Flanger Delay
		, 25, 0.010000 // Flanger Rate
		, 26, 10.000000 // Flanger Depth
		, 46, 100.000000 // Step 0 Probability
		, 47, 67.500001 // Step 1 Probability
		, 48, 6.666667 // Step 2 Probability
		, 49, 100.000000 // Step 3 Probability
		, 50, 0.000000 // Step 4 Probability
		, 51, 58.333331 // Step 5 Probability
		, 52, 100.000000 // Step 6 Probability
		, 53, 0.000000 // Step 7 Probability
		, 54, 100.000000 // Step 8 Probability
		, 55, 7.500000 // Step 9 Probability
		, 56, 100.000000 // Step 10 Probability
		, 57, 50.833333 // Step 11 Probability
		, 58, 0.000000 // Step 12 Probability
		, 59, 11.666667 // Step 13 Probability
		, 60, 100.000000 // Step 14 Probability
		, 61, 12.500000 // Step 15 Probability
		, 81, 75.000000 // Step 1 Velocity
		, 82, 34.166667 // Step 2 Velocity
		, 85, 50.833333 // Step 5 Velocity
		, 89, 37.500000 // Step 9 Velocity
		, 91, 33.333334 // Step 11 Velocity
		, 93, 63.333333 // Step 13 Velocity
		, 94, 73.333335 // Step 14 Velocity
		, 95, 83.333331 // Step 15 Velocity
		, 106, 71.949997 // Step 9 Attack
		, 108, 72.775002 // Step 11 Attack
		, 112, 99.175001 // Step 15 Attack
		, 115, 58.749998 // Step 1 Decay
		, 116, 53.800003 // Step 2 Decay
		, 117, 82.674999 // Step 3 Decay
		, 119, 65.349998 // Step 5 Decay
		, 123, 48.024999 // Step 9 Decay
		, 125, 71.949997 // Step 11 Decay
		, 127, 40.600001 // Step 13 Decay
		, 128, 44.725000 // Step 14 Decay
		, 129, 48.849999 // Step 15 Decay
	);

	MakePresetFromNamedParams("bass grind", 65
		, 0, 7 // Waveform
		, 1, 16.516667 // Pulse Width
		, 5, 0.136338 // Decay
		, 6, 2.500000 // Sustain
		, 7, 0.462875 // Release
		, 9, -5.999999 // Volume
		, 13, 132.000003 // Tempo
		, 14, 7 // Step Length
		, 17, 4 // Key
		, 18, 0 // Scale
		, 19, 1 // Octave
		, 20, 2 // Range
		, 21, 12 // Delay Duration
		, 22, 47.499999 // Delay Feedback
		, 24, 0.759850 // Flanger Delay
		, 25, 2.841917 // Flanger Rate
		, 26, 17.333333 // Flanger Depth
		, 28, 50.833333 // Flanger Mix
		, 46, 100.000000 // Step 0 Probability
		, 47, 100.000000 // Step 1 Probability
		, 48, 100.000000 // Step 2 Probability
		, 49, 100.000000 // Step 3 Probability
		, 50, 0.000000 // Step 4 Probability
		, 51, 100.000000 // Step 5 Probability
		, 52, 100.000000 // Step 6 Probability
		, 53, 100.000000 // Step 7 Probability
		, 54, 100.000000 // Step 8 Probability
		, 55, 0.000000 // Step 9 Probability
		, 56, 100.000000 // Step 10 Probability
		, 57, 100.000000 // Step 11 Probability
		, 58, 0.000000 // Step 12 Probability
		, 59, 100.000000 // Step 13 Probability
		, 60, 100.000000 // Step 14 Probability
		, 61, 100.000000 // Step 15 Probability
		, 64, -0.216667 // Step 1 Pan
		, 65, 0.283333 // Step 2 Pan
		, 66, -0.266667 // Step 3 Pan
		, 68, 0.283333 // Step 5 Pan
		, 69, -0.166667 // Step 6 Pan
		, 70, 0.266667 // Step 7 Pan
		, 76, -0.200000 // Step 13 Pan
		, 77, 0.300000 // Step 14 Pan
		, 78, 0.016667 // Step 15 Pan
		, 81, 85.000000 // Step 1 Velocity
		, 82, 70.833334 // Step 2 Velocity
		, 85, 53.333333 // Step 5 Velocity
		, 86, 65.833333 // Step 6 Velocity
		, 87, 84.166667 // Step 7 Velocity
		, 89, 37.500000 // Step 9 Velocity
		, 91, 80.833334 // Step 11 Velocity
		, 93, 72.499999 // Step 13 Velocity
		, 94, 80.000002 // Step 14 Velocity
		, 95, 88.333331 // Step 15 Velocity
		, 106, 71.949997 // Step 9 Attack
		, 108, 72.775002 // Step 11 Attack
		, 112, 99.175001 // Step 15 Attack
		, 115, 93.399998 // Step 1 Decay
		, 116, 93.400003 // Step 2 Decay
		, 117, 95.049999 // Step 3 Decay
		, 119, 92.574998 // Step 5 Decay
		, 124, 98.350000 // Step 10 Decay
		, 125, 90.099997 // Step 11 Decay
		, 127, 67.000000 // Step 13 Decay
		, 128, 71.950000 // Step 14 Decay
		, 129, 50.500000 // Step 15 Decay
	);

	MakePresetFromNamedParams("fjords of canada", 49
		, 0, 0 // Waveform
		, 1, 49.591667 // Pulse Width
		, 3, 2.000000 // Movement
		, 4, 2.000000 // Attack
		, 5, 2.000000 // Decay
		, 6, 70.833333 // Sustain
		, 7, 4.204375 // Release
		, 13, 60.000000 // Tempo
		, 19, 3 // Octave
		, 20, 2 // Range
		, 21, 7 // Delay Duration
		, 22, 32.500000 // Delay Feedback
		, 24, 5.009000 // Flanger Delay
		, 25, 0.109950 // Flanger Rate
		, 26, 50.000000 // Flanger Depth
		, 27, 4.166667 // Flanger Feedback
		, 28, 50.000000 // Flanger Mix
		, 63, -0.207530 // Step 0 Pan
		, 64, -0.773258 // Step 1 Pan
		, 65, 0.053286 // Step 2 Pan
		, 66, -0.163306 // Step 3 Pan
		, 67, -0.531381 // Step 4 Pan
		, 68, -0.716698 // Step 5 Pan
		, 69, -0.667401 // Step 6 Pan
		, 70, 0.575224 // Step 7 Pan
		, 71, 0.980621 // Step 8 Pan
		, 72, -0.308636 // Step 9 Pan
		, 73, 0.723675 // Step 10 Pan
		, 74, -0.768594 // Step 11 Pan
		, 75, 0.595725 // Step 12 Pan
		, 76, 0.992799 // Step 13 Pan
		, 77, -0.235492 // Step 14 Pan
		, 78, -0.863727 // Step 15 Pan
		, 131, 77.734004 // Step 0 Sustain
		, 132, 70.306107 // Step 1 Sustain
		, 133, 72.622119 // Step 2 Sustain
		, 134, 74.943272 // Step 3 Sustain
		, 135, 67.015740 // Step 4 Sustain
		, 136, 66.966601 // Step 5 Sustain
		, 137, 59.381991 // Step 6 Sustain
		, 138, 64.687387 // Step 7 Sustain
		, 139, 58.471877 // Step 8 Sustain
		, 140, 77.524951 // Step 9 Sustain
		, 141, 77.801194 // Step 10 Sustain
		, 142, 69.413296 // Step 11 Sustain
		, 143, 79.748468 // Step 12 Sustain
		, 144, 91.135590 // Step 13 Sustain
		, 145, 81.454138 // Step 14 Sustain
		, 146, 87.160677 // Step 15 Sustain
	);

	MakePresetFromNamedParams("deep sea signals", 64
		, 0, 7 // Waveform
		, 2, 2.000000 // Glide
		, 4, 0.586875 // Attack
		, 5, 0.171250 // Decay
		, 6, 7.500000 // Sustain
		, 7, 0.499750 // Release
		, 13, 50.000000 // Tempo
		, 18, 7 // Scale
		, 19, -1 // Octave
		, 22, 47.500000 // Delay Feedback
		, 23, 29.166667 // Delay Mix
		, 24, 12.090917 // Flanger Delay
		, 25, 13.936367 // Flanger Rate
		, 26, 22.500000 // Flanger Depth
		, 27, 50.833333 // Flanger Feedback
		, 28, 50.000000 // Flanger Mix
		, 46, 55.826035 // Step 0 Probability
		, 47, 48.264328 // Step 1 Probability
		, 48, 25.811748 // Step 2 Probability
		, 49, 0.637421 // Step 3 Probability
		, 50, 73.838548 // Step 4 Probability
		, 51, 31.733267 // Step 5 Probability
		, 52, 25.356488 // Step 6 Probability
		, 53, 48.179507 // Step 7 Probability
		, 54, 8.321319 // Step 8 Probability
		, 55, 92.167672 // Step 9 Probability
		, 56, 97.163773 // Step 10 Probability
		, 57, 35.263363 // Step 11 Probability
		, 58, 2.174314 // Step 12 Probability
		, 59, 72.785499 // Step 13 Probability
		, 60, 30.166610 // Step 14 Probability
		, 61, 99.707283 // Step 15 Probability
		, 63, -0.585504 // Step 0 Pan
		, 64, 0.090044 // Step 1 Pan
		, 65, 0.982375 // Step 2 Pan
		, 66, 0.820282 // Step 3 Pan
		, 67, 0.367713 // Step 4 Pan
		, 68, 0.143717 // Step 5 Pan
		, 69, 0.520350 // Step 6 Pan
		, 70, 0.698369 // Step 7 Pan
		, 71, -0.302552 // Step 8 Pan
		, 72, -0.310398 // Step 9 Pan
		, 73, 0.925661 // Step 10 Pan
		, 74, -0.835100 // Step 11 Pan
		, 75, 0.440131 // Step 12 Pan
		, 76, -0.347943 // Step 13 Pan
		, 77, 0.522635 // Step 14 Pan
		, 78, 0.259723 // Step 15 Pan
		, 131, 55.646684 // Step 0 Sustain
		, 132, 40.355043 // Step 1 Sustain
		, 133, 61.246426 // Step 2 Sustain
		, 134, 43.508055 // Step 3 Sustain
		, 135, 41.557361 // Step 4 Sustain
		, 136, 50.654197 // Step 5 Sustain
		, 137, 71.362596 // Step 6 Sustain
		, 138, 88.112224 // Step 7 Sustain
		, 139, 71.277194 // Step 8 Sustain
		, 140, 46.182380 // Step 9 Sustain
		, 141, 55.868169 // Step 10 Sustain
		, 142, 60.928714 // Step 11 Sustain
		, 143, 83.081719 // Step 12 Sustain
		, 144, 43.274174 // Step 13 Sustain
		, 145, 36.061268 // Step 14 Sustain
		, 146, 9.007126 // Step 15 Sustain
	);

}
