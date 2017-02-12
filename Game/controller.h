#pragma once
#include <map>
#include "..\basebot.h"

struct BotStats
{
	int ID;
	BaseBot* bot;
	long points;
	int bps;
	long spent;
	int totalScore;
};

class Controller
{
public:
	Controller();
	void Init();
	void Run(int len);
	void Round();
	void Score();
	void FinalScore();


private:
	std::map<int, BotStats> bots;
	std::map<int, Project*> projects;
	int bid;
	int pid;
};