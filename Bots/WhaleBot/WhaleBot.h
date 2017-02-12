#pragma once
#include "..\..\basebot.h"

class WhaleBot : public BaseBot
{
public:
	WhaleBot(int n);
	// Inherited via BaseBot
	virtual void Turn(std::vector<Contribution*> avail, int pts) override;

	virtual void Success(int investID, int place, int pts, int bps) override;

	int whaling;
};