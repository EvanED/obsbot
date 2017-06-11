#include "obsbot.hpp"
#include <iostream>
#include <sstream>
#include <cassert>

#include "util/enumerate.hpp"
#include "bitmaps.hpp"

//#include "P:\programs\bwapi-master\bwapi\BWAPI\Source\BWAPI\UnitImpl.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

using namespace BWAPI;
using namespace Filter;

UnitType const army_types[] = {
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

void ExampleAIModule::onStart()
{
	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);
}

struct UnitStats
{
	int n_workers = 0;
	int n_army = 0;

	int used_food = 0;
	int available_food = 0;

	void record_unit(UnitInterface const * u) {
		if (u->getType().isWorker())
			++n_workers;

		if (std::find(std::begin(army_types), std::end(army_types), u->getType()) != std::end(army_types)) {
			++n_army;
		}

		used_food += u->getType().supplyRequired();
		available_food += u->getType().supplyProvided();
	}
};

template <typename T, size_t size_minus_one, size_t tsize = size_minus_one + 1>
class FixedCircularBuffer
{
public:
	typedef unsigned int size_type;
	typedef T const & const_reference;

private:
	std::array<T, tsize + 1> m_storage;
	size_type m_insertion_point = 0;

	static_assert(tsize <= std::numeric_limits<size_type>::max(), "foo");

public:
	FixedCircularBuffer(T const & t) {
		m_storage.fill(t);
	}

	FixedCircularBuffer(T && t) {
		m_storage.fill(t);
	}

	void push_back(T const & v) {
		m_storage[m_insertion_point] = v;
		m_insertion_point = (m_insertion_point + 1) % tsize;
	}

	const_reference at(size_type logical_idx) const {
		if (logical_idx > tsize)
			throw std::out_of_range("Out of range");

		size_type phisical_idx = (logical_idx + m_insertion_point) % tsize;
		return m_storage[physical_idx];
	}

	class const_iterator : std::iterator<std::forward_iterator_tag, T> {
		T const * m_item;
		T const * m_end;

		const_iterator(T const * s, T const * e)
			: m_item(s), m_end(e)
		{}

		friend class FixedCircularBuffer<T, size_minus_one, tsize>;

	public:
		const_iterator& operator++() {
			++m_item;
			if (m_item == m_end)
				m_item = m_end - tsize;
			return *this;
		}

		const_iterator operator++(int) {
			const_iterator copy = *this;
			++*this;
			return copy;
		}

		const_reference operator* () const {
			return *m_item;
		}

		bool operator== (const_iterator that) const {
			assert(this->m_end == that.m_end);
			return this->m_item == that.m_item;
		}

		bool operator!= (const_iterator that) const {
			return !(*this == that);
		}
	};

	const_iterator begin() const {
		return const_iterator(
			m_storage.data() + ((m_insertion_point + 1) % tsize),
			m_storage.data() + tsize);
	}

	const_iterator end() const {
		return const_iterator(
			m_storage.data() + m_insertion_point,
			m_storage.data() + tsize);
	}

	size_type size() const {
		return size_minus_one;
	}
};

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
	const int width_per_char = 5;
	const int height_per_line = 20;

	const int box_top = 40;
	const int box_left = 30;
	const int box_right = box_left + 26 * width_per_char;
	const int box_bottom = box_top + 3 * height_per_line;

	Broodwar->drawBoxScreen(box_left, box_top, box_right, box_bottom, Colors::Grey, true);
	Broodwar->drawTextScreen(box_left + 5, box_top, "Economy:");

	int next_y = height_per_line;
	for (auto const & player : Broodwar->getPlayers()) {
		if (player->isObserver() || player->isNeutral())
			continue;

		Broodwar->drawTextScreen(box_left + width_per_char, box_top + next_y,
			"Workers: %d  Lost: %d",
			player->allUnitCount(UnitTypes::Terran_SCV)
			+ player->allUnitCount(UnitTypes::Protoss_Probe)
			+ player->allUnitCount(UnitTypes::Zerg_Drone),
			player->deadUnitCount(UnitTypes::Terran_SCV)
			+ player->deadUnitCount(UnitTypes::Protoss_Probe)
			+ player->deadUnitCount(UnitTypes::Zerg_Drone));

		next_y += height_per_line;
	}
}

void ExampleAIModule::onFrame()
{
	// Called once every game frame

	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	std::vector<UnitStats> unit_stats(Broodwar->getPlayers().size());

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

		unit_stats.at(u->getPlayer()->getID()).record_unit(u);

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
