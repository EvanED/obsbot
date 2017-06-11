#pragma once

#include "UnitInfo.hpp"
#include "../util/container_algorithms.hpp"

using namespace BWAPI;

static const UnitType army_types[] = {
	UnitTypes::Terran_Marine,
	UnitTypes::Terran_Ghost,
	UnitTypes::Terran_Vulture,
	UnitTypes::Terran_Goliath,
	UnitTypes::Terran_Siege_Tank_Tank_Mode,
	UnitTypes::Terran_Wraith,
	UnitTypes::Terran_Science_Vessel,
	UnitTypes::Terran_Dropship,
	UnitTypes::Terran_Battlecruiser,
	UnitTypes::Terran_Siege_Tank_Siege_Mode,
	UnitTypes::Terran_Firebat,
	UnitTypes::Terran_Medic,
	UnitTypes::Terran_Valkyrie,

	UnitTypes::Zerg_Egg,
	UnitTypes::Zerg_Zergling,
	UnitTypes::Zerg_Hydralisk,
	UnitTypes::Zerg_Ultralisk,
	UnitTypes::Zerg_Mutalisk,
	UnitTypes::Zerg_Guardian,
	UnitTypes::Zerg_Queen,
	UnitTypes::Zerg_Defiler,
	UnitTypes::Zerg_Scourge,
	UnitTypes::Zerg_Infested_Terran,
	UnitTypes::Zerg_Devourer,

	UnitTypes::Protoss_Corsair,
	UnitTypes::Protoss_Dark_Templar,
	UnitTypes::Protoss_Dark_Archon,
	UnitTypes::Protoss_Zealot,
	UnitTypes::Protoss_Dragoon,
	UnitTypes::Protoss_High_Templar,
	UnitTypes::Protoss_Archon,
	UnitTypes::Protoss_Shuttle,
	UnitTypes::Protoss_Scout,
	UnitTypes::Protoss_Arbiter,
	UnitTypes::Protoss_Carrier,
	UnitTypes::Protoss_Reaver,
	UnitTypes::Protoss_Observer,
};

bool is_army(UnitType t)
{
	return algo::contains(army_types, t);
}