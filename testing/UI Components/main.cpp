#include "BWAPI.h"
#include "../../Source/ui/components.hpp"

using namespace BWAPI;

int get_worker_count(PlayerInterface * p)
{
	return 5;
}

int main()
{
	PlayerInterface p1("P1");
	PlayerInterface p2("P2");

	std::vector<PlayerInterface*> players{ &p1, &p2 };
	Position p(20, 30);
	build_economy_tab(players)->draw(p);
}