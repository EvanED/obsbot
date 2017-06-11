#include "PlayerStats.hpp"

using namespace BWAPI;

static void record_unit(PlayerStats & st, BWAPI::UnitInterface const * u) {
	if (u->getType().isWorker())
		++st.n_workers;

	if (is_army(u))
		++st.n_army;
}

std::vector<PlayerStats>
get_player_stats()
{
	std::vector<PlayerStats> ret(Broodwar->getPlayers().size());

	for (auto &u : Broodwar->getAllUnits())
	{
		// Ignore the unit if it no longer exists
		// Make sure to include this block when handling any Unit pointer!
		if (!u->exists())
			continue;

		// Ignore the unit if it is incomplete or busy constructing
		if (!u->isCompleted() || u->isConstructing())
			continue;

		if (u->getPlayer()->isNeutral())
			continue;

		record_unit(ret.at(u->getPlayer()->getID()), u);
	}

	return ret;
}