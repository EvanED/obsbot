#pragma once

#include <BWAPI.h>

bool is_army(BWAPI::UnitType t);

inline bool is_army(BWAPI::UnitInterface const * u) {
	return is_army(u->getType());
}
