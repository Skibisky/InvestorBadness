#include "DiversifyBot.h"

void DiversifyBot::Turn(std::vector<Contribution*> avail, int pts)
{
	// point a point in every project
	for each(auto inv in avail)
	{
		if (pts <= 0)
			break;

		if (inv->oldContrib == 0)
		{
			inv->myContrib++;
			pts--;
		}
	}

	// dump remaining points in projects we aren't even 5th in
	for each(auto inv in avail)
	{
		if (inv->myRank > 5 && inv->contribs()[4] - inv->oldContrib < 3)
		{
			inv->myContrib++;
			pts--;
		}
	}
}
