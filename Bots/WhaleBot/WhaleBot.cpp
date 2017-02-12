#include "WhaleBot.h"

WhaleBot::WhaleBot(int n)
{
	whaling = 0;
}

void WhaleBot::Turn(std::vector<Contribution*> avail, int pts)
{
	// if no whale target, make on up
	if (whaling == 0)
	{
		whaling = rand() % avail.size();
	}

	// find it
	for each (auto c in avail)
	{
		if (c->id() == whaling)
		{
			c->myContrib = pts;
			pts = 0;
		}
	}

	// if we didn't find it, stop
	if (pts != 0)
		whaling = 0;
}

void WhaleBot::Success(int investID, int place, int pts, int bps)
{
	// project complete, stop whaling on it
	if (investID == whaling)
		whaling = 0;
}
