#include "RandomBot.h"

void RandomBot::Turn(std::vector<Contribution*> avail, int pts)
{
	int i = rand() % avail.size();
	avail.at(i)->myContrib = pts;
	// there isn't much to it
}
