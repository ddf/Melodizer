#pragma once

#include <vector>

class Melodizer;
class IGraphics;
class LED;

class Interface
{
public:
	Interface(Melodizer* inPlug);
	~Interface();

	void CreateControls(IGraphics* pGraphics);

	// called by the plug when the next triggered 16th note happens.
	// we use this to blink the appropriate LED
	void OnTick(const unsigned int tick);

private:	
	Melodizer* const mPlug;

	// bank of LED graphics above all the knobs
	std::vector<LED*> mLEDs;
};

