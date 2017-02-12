#include "BadEcoBot.h"

BadEcoBot::BadEcoBot(int n)
{
	favouriteNumbers[0] = n;
	favouriteNumbers[1] = rand() % 10;
	favouriteNumbers[2] = rand() % 10;
	stockpile = 0;
	favouriteProject = 0;
}

void BadEcoBot::Turn(std::vector<Contribution*> avail, int pts)
{
	for each(auto p in avail)
	{
		// put some of our saved points into our favourite project
		if (p->id() == favouriteProject)
		{
			p->myContrib = 1 + sqrt(pts);
			pts -= p->myContrib;
			if (pts > 1)
				stockpile -= rand() % p->myContrib;
		}

		if (stockpile < 0)
			stockpile = 0;
		if (pts < 0)
			pts = 0;

		// save some for later
		if (pts <= stockpile)
			break;

		// if the project ends in our favourite numbers, put points in it
		if (p->id() % 10 == favouriteNumbers[0] ||
			p->id() % 10 == favouriteNumbers[1] || 
			p->id() % 10 == favouriteNumbers[2])
		{
			p->myContrib++;
			pts--;

			// for every spare point
			int sz = pts;
			for (int i = stockpile; i < sz; i++)
			{
				if (pts <= stockpile)
					break;

				// maybe put it in, maybe save it
				if (rand() % 5 == 0)
				{
					p->myContrib++;
					pts--;
				}
			}

			// with no favourite project, maybe this one is okay
			if (favouriteProject == 0 && rand() % 100 == 0)
				favouriteProject = p->id();
		}
	}
}

void BadEcoBot::Success(int investID, int place, int pts, int bps)
{
	// save some of these for later
	if (pts > 0)
		stockpile += rand() % pts;
	// check if our favourite project finished?
	if (investID == favouriteProject)
		favouriteProject = 0;
}
