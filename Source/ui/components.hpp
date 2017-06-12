#pragma once

#include <cassert>
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <BWAPI/Position.h>
#include <BWAPI/Color.h>

namespace BWAPI {
	class PlayerInterface;
}

typedef std::function<void(BWAPI::Position p)> Drawer;

Drawer
build_economy_tab(std::vector<BWAPI::PlayerInterface *> const & players);
