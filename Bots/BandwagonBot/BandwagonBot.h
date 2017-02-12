#pragma once
#include "..\..\basebot.h"

/**
	BandwagonBot:
		Finds the project with the most points and dumps all its points into it.
		Attempts to greedily take 1st as well as help others cash out.
*/
class BandwagonBot : public BaseBot
{
public:
	BandwagonBot(int n) {}
	// Inherited via BaseBot
	virtual void Turn(std::vector<Contribution*> avail, int pts) override;

	// Don't care, I'll jump on *any* bandwagon
	virtual void Success(int investID, int place, int pts, int bps) override {}
};