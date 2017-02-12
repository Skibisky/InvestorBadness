#pragma once

#include <map>

/**
	Save ticks by reusing the same Contribution object to each bot.
	Don't let them fool others into thinking things aren't as they are.
*/
class Contribution
{
	// Project can hack it
	friend class Project;
public:
	const int id() { return m_id; }
	const int current() { return m_current; }
	const int totalCost() { return m_max; }
	const int* contribs() { return m_contribs; }
	const int* pts() { return m_pts; }
	const int* bps() { return m_bps; }
	// these are reset every loop anyway
	int myContrib;
	int oldContrib;
	int myRank;
private:
	Contribution(int id) { m_id = id; }
	int m_id;
	int m_current;
	int m_max;
	int m_contribs[5];
	int m_pts[5];
	int m_bps[5];
};

class Project
{
public:
	Project(int id);

	// Serialize this Investment into a trashy thing
	Contribution* GetInvest();

	// Figure out new costs and rewards
	void LevelUp();
	// Let the projects act
	void Turn();
	// A bot is giving you points
	int Allocate(int bid, int pts);

	/// Boring Getters ///
	int GetID() { return m_id; }
	int GetCost() { return m_cost; }
	int GetRemaining() { return __max(0, m_cost - m_current); }
	int GetLevel() { return m_level; }

	int GetRank(int id);
	int GetPoints(int id);

	int* GetTopContrib() { return m_bids; }
	int* GetPayoutPts() { return m_pts; }
	int* GetPayoutBps() { return m_bps; }
	
private:
	int m_id;
	int m_level;
	int m_cost;
	int m_current;

	// if this project will gain completion without contributors
	bool m_selfGrow;

	int m_bids[5];
	int m_contribs[5];
	int m_pts[5];
	int m_bps[5];

	// how much each bot as given
	std::map<int, int> m_allcontrib;
};

// pull out all the choices made by bots
struct investment
{
	int investID;
	int points;
};
