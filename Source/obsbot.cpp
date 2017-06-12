#include "obsbot.hpp"
#include <iostream>
#include <sstream>

#include "util/fixed_circular_buffer.hpp"
#include "util/enumerate.hpp"
#include "ui/ui.hpp"
#include "ui/bitmaps.hpp"
#include "ui/components.hpp"
#include "sc_util/UnitInfo.hpp"
#include "PlayerStats.hpp"

//#include "P:\programs\bwapi-master\bwapi\BWAPI\Source\BWAPI\UnitImpl.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

using namespace BWAPI;
using namespace Filter;

void ExampleAIModule::onStart()
{
	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);
}

struct UnitClientInfo
{
	static const unsigned ring_size = 31;
	FixedCircularBuffer<Position, ring_size> position_history;

	UnitClientInfo(Unit u)
		: position_history(u->getPosition())
	{}

	void update(Unit u) {
		position_history.push_back(u->getPosition());
	}

	Position weighted_position() const {
		double x = 0, y = 0, total_weight = 0;
		const double factor = ring_size + 1;
		for (auto p : enumerate(position_history)) {
			x += p.first / factor * p.second.x;
			y += p.first / factor * p.second.y;
			total_weight += p.first / factor;
		}
		return Position(
			static_cast<int>(x / total_weight),
			static_cast<int>(y / total_weight)
		);
	}
};

// TODO: move to clientinfo field
std::unordered_map<Unit, UnitClientInfo> client_infos;

void draw_workers()
{
	const int box_top = 40;
	const int box_left = 30;

	std::vector<Player> players;
	for (auto const & player : Broodwar->getPlayers()) {
		if (player->isObserver() || player->isNeutral())
			continue;
		players.push_back(player);
	}

	build_economy_tab(players)->draw(Position(box_left, box_top));
}

void ExampleAIModule::onFrame()
{
	// Called once every game frame

	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	std::vector<PlayerStats> unit_stats{ get_player_stats() };

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

		draw_health_bar(u);

		if (client_infos.find(u) == client_infos.end()) {
			client_infos.emplace(u, UnitClientInfo(u));
		}

		UnitClientInfo & unit_info = client_infos.at(u);
		unit_info.update(u);
		Position avg = unit_info.weighted_position();

		Broodwar->drawLineMap(
			u->getPosition(),
			u->getPosition() + (u->getPosition() - avg),
			u->getPlayer()->getColor());
	}

	const int top_box_height = 20;
	const int topline = 3;
	const int sq_size = top_box_height - 2*topline;
	const int width_per_char = 5;

	Broodwar->drawBoxScreen(0, 0, SCREEN_WIDTH, top_box_height, Colors::Black, true);

	int next_x = 10;
	int next_y = 30;
	for (auto const & player : Broodwar->getPlayers()) {
		if (player->isObserver() || player->isNeutral())
			continue;

		const int first_x = next_x;

		Broodwar->drawTextScreen(next_x, topline, "%s", player->getName().c_str());
		next_x += width_per_char * player->getName().size() + sq_size;

		draw_bitmap(next_x, topline, minerals_image);
		next_x += sq_size + sq_size / 2;
		Broodwar->drawTextScreen(next_x, topline, "%d", player->gatheredMinerals() - player->spentMinerals());
		next_x += width_per_char * 4 + sq_size / 2;

		draw_bitmap(next_x, topline, gas_image);
		next_x += sq_size + sq_size / 2;
		Broodwar->drawTextScreen(next_x, topline, "%d", player->gatheredGas() - player->spentGas());
		next_x += width_per_char * 4 + sq_size / 2;

		draw_bitmap(next_x, topline, food_image);
		next_x += sq_size + sq_size / 2;
		Broodwar->drawTextScreen(next_x, topline, "%d/%d", player->supplyUsed() / 2, player->supplyTotal() / 2);
		next_x += width_per_char * 6 + sq_size / 2;

		Broodwar->drawBoxScreen(first_x-5, top_box_height-2, next_x - sq_size / 2, top_box_height, player->getColor(), true);

		next_x += sq_size * 3;
	}

	draw_workers();
}

void ExampleAIModule::onEnd(bool did_i_win)
{
}

void ExampleAIModule::onSendText(std::string text)
{
	Broodwar->sendText("%s", text.c_str());
}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
	Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{
}
