#pragma once
#include "..\..\basebot.h"

/**
	RandomBot:
		The usual cheese.
		Puts all its points into a random project.
*/
class RandomBot : public BaseBot
{
public:
	RandomBot(int n) {}
	// Inherited via BaseBot
	virtual void Turn(std::vector<Contribution*> avail, int pts) override;

	virtual void Success(int investID, int place, int pts, int bps) override {}
};