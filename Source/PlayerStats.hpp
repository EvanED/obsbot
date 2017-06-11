#pragma once

#include <BWAPI.h>
#include "sc_util/UnitInfo.hpp"

struct PlayerStats
{
	int n_workers = 0;
	int n_army = 0;
};

std::vector<PlayerStats>
get_player_stats();
