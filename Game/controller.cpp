#include "controller.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <random>
#include <chrono>

#include "..\Bots\BadEcoBot\BadEcoBot.h"
#include "..\Bots\BandwagonBot\BandwagonBot.h"
#include "..\Bots\DiversifyBot\DiversifyBot.h"
#include "..\Bots\RandomBot\RandomBot.h"
#include "..\Bots\WhaleBot\WhaleBot.h"

Controller::Controller()
{
	bid = 1;
	pid = 1;
}

#define MAKEBOT(x) bots[bid] = MakeBot(bid, new x(i)); bid++

BotStats MakeBot(int id, BaseBot* b)
{
	BotStats bs;
	bs.bot = b;
	bs.bps = 0;
	bs.points = 0;
	bs.ID = id;
	bs.spent = 0;
	bs.totalScore = 0;
	return bs;
}

void Controller::Init()
{
	// pad bots
	for (int i = 0; i < NUMBER_PADDER; i++)
	{
		MAKEBOT(BadEcoBot);
	}
	// fighter bots
	for (int i = 0; i < NUMBER_PLAYER; i++)
	{
		MAKEBOT(BandwagonBot);
		MAKEBOT(DiversifyBot);
		MAKEBOT(WhaleBot);
		MAKEBOT(RandomBot);
	}

	int sz = bots.size();
	for (int i = 0; i < sz; i++)
	{
		projects[pid] = new Project(pid); pid++;
	}
}

void Controller::Run(int len)
{
	for each (auto b in bots)
	{
		b.second.bps = 0;
		b.second.points = 0;
		b.second.spent = 0;
	}
	std::cout << "Round  ";
	for (int i = 0; i < len; i++)
	{
		if (i % 3 == 0)
		{
			for (int j = 0; j < log10(i); j++)
			{
				std::cout << '\b';
			}
			std::cout << i;
		}
		Round();
	}
	Score();
}

void Controller::Score()
{
	std::cout << std::endl << "Project Status" << std::endl;

	for each (auto p in projects)
	{
		//std::cout << p.second->GetID() << " " << p.second->GetRemaining() << "/" << p.second->GetCost() << std::endl;
	}
	
	std::cout << std::endl << "Bot Scores" << std::endl;

	int totalBps = 0;
	int totalPts = 0;
	int totalSpent = 0;
	for each (auto b in bots)
	{
		totalBps += b.second.bps;
		totalPts += b.second.points;
		totalSpent += b.second.spent;
	}

	std::vector<std::pair<int, BotStats>> sortedStats(bots.size());

	std::partial_sort_copy(bots.begin(),
		bots.end(),
		sortedStats.begin(),
		sortedStats.end(),
		[](std::pair<int, BotStats> const& l,
			std::pair<int, BotStats> const& r)
	{
		double lscore = l.second.bps * 250 + l.second.points * 10 - l.second.spent * 0.5;
		double rscore = r.second.bps * 250 + r.second.points * 10 - r.second.spent * 0.5;
		return lscore > rscore;
	});

	std::map<std::string, int> stratPoints;

	int place = 0;
	int sz = sortedStats.size();
	for each (auto b in sortedStats)
	{
		place++;
		std::cout << std::setw(3) << b.first << ":" << std::setw(20) << b.second.bot->name() << std::setw(8) << b.second.bps << std::setw(8) << b.second.points << std::setw(8) << b.second.spent << std::endl;
		//int stScore = ((sz - place) / 2.0) * ((sz - place) / 2.0) / (b.second.bot->name().compare("BadEcoBot") == 0 ? NUMBER_PADDER : NUMBER_PLAYER);
		//int stScore = (b.second.bps * 250 + b.second.points * 10 - b.second.spent * 0.5) / (b.second.bot->name().compare("BadEcoBot") == 0 ? NUMBER_PADDER : NUMBER_PLAYER);
		int stScore = (b.second.bps * 25000 / totalBps) / (b.second.bot->name().compare("BadEcoBot") == 0 ? NUMBER_PADDER : NUMBER_PLAYER);
		stScore += (b.second.points * 1000 / totalPts) / (b.second.bot->name().compare("BadEcoBot") == 0 ? NUMBER_PADDER : NUMBER_PLAYER);
		stScore -= (b.second.spent * 500 / totalSpent) / (b.second.bot->name().compare("BadEcoBot") == 0 ? NUMBER_PADDER : NUMBER_PLAYER);

		bots[b.second.ID].totalScore += stScore;

		auto it = stratPoints.find(b.second.bot->name());
		if (it == stratPoints.end())
			stratPoints[b.second.bot->name()] = stScore;
		else
			stratPoints[b.second.bot->name()] = stratPoints[b.second.bot->name()] + stScore;

	}

	sz = stratPoints.size();
	std::vector<std::pair<std::string, int>> stratSorted(sz);
	std::partial_sort_copy(stratPoints.begin(),
		stratPoints.end(),
		stratSorted.begin(),
		stratSorted.end(),
		[](std::pair<std::string, int> const& l,
			std::pair<std::string, int> const& r)
	{
		return l.second > r.second;
	});

	std::cout << std::endl << std::endl << "Strat scores:" << std::endl;

	for (int i = 0; i < sz; i++)
	{
		std::cout << std::setw(3) << stratSorted[i].second << "\t" << stratSorted[i].first << std::endl;
	}
}

void Controller::FinalScore()
{
	std::map<std::string, int> stratPoints;
	for each (auto b in bots)
	{
		stratPoints[b.second.bot->name()] += b.second.totalScore;
	}
	int sz = stratPoints.size();
	std::vector<std::pair<std::string, int>> stratSorted(sz);
	std::partial_sort_copy(stratPoints.begin(),
		stratPoints.end(),
		stratSorted.begin(),
		stratSorted.end(),
		[](std::pair<std::string, int> const& l,
			std::pair<std::string, int> const& r)
	{
		return l.second > r.second;
	});

	std::cout << std::endl << std::endl << "===Strat FINAL scores===" << std::endl;

	for (int i = 0; i < sz; i++)
	{
		std::cout << std::setw(3) << stratSorted[i].second << "\t" << stratSorted[i].first << std::endl;
	}
}

void Controller::Round()
{
	clock_t start, stop;
	start = clock();
	std::vector<Contribution*> contribs;
	for each (auto p in projects)
	{
		contribs.push_back(p.second->GetInvest());
	}
	unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(contribs.begin(), contribs.end(), std::default_random_engine(seed));

	std::map<int, std::list<investment>> invests;

	std::vector<BotStats*> blist;

	for each (auto b in bots)
	{
		blist.push_back(&bots[b.first]);
	}
	seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(blist.begin(), blist.end(), std::default_random_engine(seed));
	stop = clock();
	if (stop - start > 5)
		std::cout << "Setup took " << (stop - start) << std::endl;

	for each (auto b in blist)
	{
		// gain a point
		b->points++;
		// clear the last guys contribs
		for each (auto c in contribs)
		{
			c->myContrib = 0;
			c->myRank = projects.find(c->id())->second->GetRank(b->ID);
			c->oldContrib = projects.find(c->id())->second->GetPoints(b->ID);

		}
		// lets them turn
		start = clock();
		b->bot->Turn(contribs, b->points);
		stop = clock();
		if (stop - start > 5)
			std::cout << b->bot->name() << " took " << (stop - start) << std::endl;

		// check all the projects
		for each (auto c in contribs)
		{
			// if they contributed
			if (c->myContrib > 0)
			{
				// push an investment
				investment i;
				i.points = c->myContrib;
				// upto the current points
				if (b->points < i.points)
					i.points = b->points;

				// remove points
				b->points -= i.points;
				i.investID = c->id();
				invests[b->ID].push_back(i);
			}
		}
	}
	start = clock();
	for each (auto kv in invests)
	{
		for each (auto ll in kv.second)
		{
			if (ll.points == 0)
				continue;

			auto it = projects.find(ll.investID);
			if (it != projects.end())
			{
				Project* proj = it->second;
				int ret = proj->Allocate(kv.first, ll.points);
				auto bit = bots.find(kv.first);
				if (bit != bots.end())
				{
					bit->second.spent += ll.points - ret;
					//if (ret > 4)
					//	std::cout << "ret: " << ret << " to " << bit->second.bot->name() << std::endl;
					bit->second.points += ret;
				}
			}
		}
	}
	stop = clock();
	if (stop - start > 10)
		std::cout << "Invests took " << (stop - start) << std::endl;

	start = clock();
	std::list<int> rem;
	for each (auto kv in projects)
	{
		Project* proj = kv.second;
		proj->Turn();
		if (proj->GetRemaining() == 0)
		{
			auto c = proj->GetInvest();
			auto ppts = proj->GetPayoutPts();
			auto pbps = proj->GetPayoutBps();
			auto bids = proj->GetTopContrib();
			//std::cout << "p" << proj->GetID() << " payout " << ppts[0];
			for (int i = 0; i < 5; i++)
			{
				if (bids[i] != 0)
				{
					auto it = bots.find(bids[i]);
					if (it != bots.end())
					{
						//std::cout << " " << it->second.bot->name();
						it->second.points += ppts[i];
						it->second.bps += pbps[i];
						it->second.bot->Success(proj->GetID(), i, ppts[i], pbps[i]);
					}
				}
			}
			//std::cout << std::endl;
			rem.push_back(proj->GetID());
		}
	}
	for each (auto i in rem)
	{
		auto it = projects.find(i);
		if (it != projects.end())
		{
			if (rand() % (it->second->GetLevel() + 1) >= 10)
			{
				delete it->second;
				projects.erase(it);
			}
			else
			{
				it->second->LevelUp();
			}
		}
	}
	int sz = bots.size() * 3 - projects.size();
	for (int i = 0; i < sz; i++)
	{
		if (rand() % 10 == 0)
			projects[pid] = new Project(pid); pid++;
	}
	
	for each(auto c in contribs)
	{
		delete c;
	}
	stop = clock();
	if (stop - start > 5)
		std::cout << "Projs took " << (stop - start) << std::endl;


}