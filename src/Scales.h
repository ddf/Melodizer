#pragma  once 

struct Scale
{    
    const char *  name;
	// each Scale consists of 12 int arrays of 12 elements each.
	// each array lists the notes that can be jumped to from that scale degree.
	// they are "ragged" in the sense that a zero in the array indicates an end of valid entries.
	// for example:
	// if notes[0] is { 1, 3, 5 } it means that from the first scale degree (ie C),
	// we are allowed to play C, D, and E as the next note. 
	// if we choose E, then to pick the following note we'd look at the contents of notes[4],
	// because notes[4] holds the list of scale degrees we can jump to from scale degree 5 (ie E).
	const int     notes[12][12];
};

extern const Scale* const Scales[]; 
extern const int		  ScalesLength; 

