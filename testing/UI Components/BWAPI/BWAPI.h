#pragma once

#include <memory>
#include "BWAPI/Position.h"
#include "BWAPI/Color.h"

namespace BWAPI
{

	struct Game {
		void drawTextScreen(int x, int y, const char * s, ...) const;
		void drawTextScreen(Position p, const char * s, ...) const;
		void drawBoxScreen(int x, int y, int x2, int y2, Color c, bool fill) const;
	};

	class PlayerInterface {
		std::string name;
	public:
		PlayerInterface(std::string s)
			: name(std::move(s))
		{}

		std::string const & getName() const {
			return name;
		}
	};

	extern std::unique_ptr<Game> Broodwar;

}