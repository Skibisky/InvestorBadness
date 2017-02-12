#include "investment.h"

#include <algorithm>
#include <vector>

Project::Project(int id)
{
	m_id = id;
	m_level = (rand() % 10) - 1;
	m_selfGrow = (rand() % 5 == 0);
	LevelUp();
}

Contribution* Project::GetInvest()
{
	Contribution* cont = new Contribution(m_id);
	cont->m_current = m_current;
	cont->m_max = m_cost;

	for (int i = 0; i < 5; i++)
	{
		cont->m_contribs[i] = m_contribs[i];
		cont->m_pts[i] = m_pts[i];
		cont->m_bps[i] = m_bps[i];
	}

	cont->myContrib = 0;
	cont->oldContrib = 0;
	cont->myRank = 0;

	return cont;
}

void Project::LevelUp()
{
	m_level++;
	m_current = 0;
	int multi = m_level * (rand() % 4 + 2);
	m_cost = floor(50 + multi * 3.5 + multi * multi * 0.4);

	for (int i = 0; i < 5; i++)
	{
		double tpts = m_cost*m_cost*0.00002 / (i + 1) + m_cost*0.09 / (i + 1) + 3 / (i + 1);
		m_pts[i] = (int)ceil(tpts / 5)*5;
		if (m_level > 5)
			m_pts[i] += 5;

		double tbps = m_level*m_level*(-0.02 / (5 + (i+1))) + m_level * (1.0 / (2 + (i + 1))) + (9.0 / (20 * (i + 1)));
		m_bps[i] = (int)round(tbps);

		m_contribs[i] = 0;
		m_bids[i] = 0;
	}
}

void Project::Turn()
{
	if (m_selfGrow)
		m_current += rand() % 3 + 1;
}

int Project::GetPoints(int id)
{
	auto it = m_allcontrib.find(id);
	if (it == m_allcontrib.end())
		return 0;
	return it->second;
}

int Project::GetRank(int id)
{
	std::vector<std::pair<int, int>> tcont((size_t)m_allcontrib.size());

	// sort all the contributions
	std::partial_sort_copy(m_allcontrib.begin(),
		m_allcontrib.end(),
		tcont.begin(),
		tcont.end(),
		[](std::pair<int, int> const& l,
			std::pair<int, int> const& r)
	{
		return l.second > r.second;
	});

	int sz = tcont.size();
	for (int i = 0; i < sz; i++)
	{
		if (tcont[i].first == id)
			return i + 1;
	}

	return 6;
}

int Project::Allocate(int bid, int pts)
{
	int ret = 0;
	int tot = pts;
	// clamp spent points
	if (pts > GetRemaining())
	{
		ret = pts - GetRemaining();
		tot = GetRemaining();
	}

	auto it = m_allcontrib.find(bid);
	if (it == m_allcontrib.end())
	{
		m_allcontrib[bid] = tot;
	}
	else
	{
		it->second += tot;
	}
	m_current += tot;

	std::vector<std::pair<int, int>> tcont((size_t)5);

	// sort contributions
	std::partial_sort_copy(m_allcontrib.begin(),
		m_allcontrib.end(),
		tcont.begin(),
		tcont.end(),
		[](std::pair<int, int> const& l,
			std::pair<int, int> const& r)
	{
		return l.second > r.second;
	});
	
	// update who is winning and with what
	for (int i = 0; i < 5; i++)
	{
		m_contribs[i] = tcont.at(i).second;
		m_bids[i] = tcont.at(i).first;
	}

	// return overspend
	return ret;
}

