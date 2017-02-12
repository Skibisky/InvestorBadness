#include "BandwagonBot.h"

void BandwagonBot::Turn(std::vector<Contribution*> avail, int pts)
{
	// find the investment with the largest contribution and go
	int max = 1;
	Contribution* cc = nullptr;
	for each(auto c in avail)
	{
		if (c->current() >= max)
		{
			// if definitively a bigger bandwagon, jump on
			// otherwise, non-commital
			if (c->current() > max || rand() % 2 == 0)
			{
				max = c->current();
				cc = c;
			}
		}
	}
	// if we didn't find anything good, wait till next round.
	if (cc != nullptr)
		cc->myContrib = pts;
}
