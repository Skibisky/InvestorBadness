#pragma once
#include "..\..\basebot.h"

/**
	BadEcoBot:
		Attempts to emulate someone who has never looked into economics.
*/
class BadEcoBot : public BaseBot
{
public:
	BadEcoBot(int n);
	// Inherited via BaseBot
	virtual void Turn(std::vector<Contribution*> avail, int pts) override;

	virtual void Success(int investID, int place, int pts, int bps) override;

private:
	int favouriteNumbers[3];
	int stockpile;
	int favouriteProject;
};