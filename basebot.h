#pragma once
#include <vector>
#include <typeinfo>

#include "Game\investment.h"

#define NUMBER_PADDER 15
#define NUMBER_PLAYER 3

class BaseBot
{
public:
	BaseBot() {}
	// You are the Nth bot!
	BaseBot(int n) {};

	// avail: list of available choices
	// pts: how many points you can allocate (at least 1)
	virtual void Turn(std::vector<Contribution*> avail, int pts) = 0;

	// investID which finished
	// place you came
	// pts you earned
	// bps you earned
	virtual void Success(int investID, int place, int pts, int bps) = 0;

	//http://stackoverflow.com/a/10896310
	std::string name() const
	{
		auto n = std::string(typeid(*this).name());
		auto it = n.find("class ");
		if (it == n.npos) return n;
		return n.replace(it, 6, "");
	}
};