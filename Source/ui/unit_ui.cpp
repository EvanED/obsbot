#include "ui.hpp"

using namespace BWAPI;

void draw_health_bar(Unit u)
{
	int uwidth = u->getRight() - u->getLeft();
	int bar_height = 2;

	double how_alive = static_cast<double>(u->getHitPoints()) / u->getType().maxHitPoints();

	if (u->getHitPoints() != u->getType().maxHitPoints()) {
		Broodwar->drawBoxMap(
			u->getLeft(),
			u->getBottom() - bar_height,
			static_cast<int>(u->getLeft() + uwidth * how_alive),
			u->getBottom(),
			u->getPlayer()->getColor(),
			true
		);

		Broodwar->drawLineMap(
			u->getLeft(),
			u->getBottom(),
			u->getLeft() + uwidth,
			u->getBottom(),
			u->getPlayer()->getColor()
		);
	}

}