#include "..\..\basebot.h"

/**
	DiversifyBot:
		Get a nice portfolio of projects, even if we don't place.
		If we invest in everything, focus on coming 5th.
*/
class DiversifyBot : public BaseBot
{
public:
	DiversifyBot(int n) {}
	// Inherited via BaseBot
	virtual void Turn(std::vector<Contribution*> avail, int pts) override;
	virtual void Success(int investID, int place, int pts, int bps) override {}
};