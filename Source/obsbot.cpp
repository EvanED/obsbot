#include "obsbot.hpp"
#include <iostream>
#include <sstream>
#include <cassert>

#include "bitmaps.hpp"

//#include "P:\programs\bwapi-master\bwapi\BWAPI\Source\BWAPI\UnitImpl.h"

#ifndef NOMINMAX
#  define NOMINMAX
#endif
#include <Windows.h>
#include <WinUser.h>

using namespace BWAPI;
using namespace Filter;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

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

void MouseSetup(INPUT *buffer)
{
	buffer->type = INPUT_MOUSE;
	buffer->mi.dx = (0 * (0xFFFF / SCREEN_WIDTH));
	buffer->mi.dy = (0 * (0xFFFF / SCREEN_HEIGHT));
	buffer->mi.mouseData = 0;
	buffer->mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
	buffer->mi.time = 0;
	buffer->mi.dwExtraInfo = 0;
}

void MouseMoveAbsolute(INPUT *buffer, int x, int y)
{
	buffer->mi.dx = long(x * (double(0xFFFF) / SCREEN_WIDTH));
	buffer->mi.dy = long(y * (double(0xFFFF) / SCREEN_HEIGHT));
	buffer->mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);

SendInput(1, buffer, sizeof(INPUT));
}

struct EnumWindowsCallbackArgs {
	EnumWindowsCallbackArgs(DWORD p) : pid(p) { }
	const DWORD pid;
	std::vector<HWND> handles;
};

static BOOL __stdcall EnumWindowsCallback(HWND hnd, LPARAM lParam)
{
	EnumWindowsCallbackArgs *args = (EnumWindowsCallbackArgs *)lParam;

	DWORD windowPID;
	(void)::GetWindowThreadProcessId(hnd, &windowPID);
	if (windowPID == args->pid) {
		args->handles.push_back(hnd);
	}

	return TRUE;
}

std::vector<HWND> getToplevelWindows()
{
	EnumWindowsCallbackArgs args(::GetCurrentProcessId());
	if (::EnumWindows(&EnumWindowsCallback, (LPARAM)&args) == FALSE) {
		// XXX Log error here
		return std::vector<HWND>();
	}
	return args.handles;
}

template <typename T>
void report_players(char const * name, T const & players)
{
	bool first = true;
	std::stringstream ss;
	ss << name << ": ";
	for (auto const & player : players) {
		if (!first) {
			ss << ", ";
			first = false;
		}
		ss << player->getName();
	}
	Broodwar << ss.str() << "\n";
}

void ExampleAIModule::onStart()
{
	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);

	report_players("Players", Broodwar->getPlayers());
	report_players("Enemies", Broodwar->enemies());
	report_players("Observers", Broodwar->observers());
}

void ExampleAIModule::onEnd(bool isWinner)
{
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

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	//if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
	//	return;
#if 0
	Broodwar->setScreenPosition(
		640 - Broodwar->getFrameCount() % (Broodwar->mapWidth() * BWAPI::TILEPOSITION_SCALE - 640),
		Broodwar->getFrameCount() % (Broodwar->mapHeight() * BWAPI::TILEPOSITION_SCALE - 480)
	);

	std::vector<HWND> wnds = getToplevelWindows();
	RECT rect;
	bool found_window = false;
	std::stringstream ss;
	for (auto wnd : wnds) {
		RECT rect2;
		BOOL ret = GetWindowRect(wnds[0], &rect2);
		if (!ret) {
			Broodwar->drawTextScreen(100, 100, "Could not get rect");
			return;
		}
		ss << "[" << rect2.left << "-" << rect2.right << ", " << rect2.top << "-" << rect2.bottom << "], ";
		if (found_window && (rect.left != rect2.left || rect.top != rect2.top)) {
			ss << " vs [" << rect.left << ", " << rect.top << "]";
			Broodwar->drawTextScreen(100, 100, "Multiple windows with different top/lefts: %s",
				ss.str().c_str());
			return;
		}
		rect = rect2;
		found_window = true;
	}
	if (!found_window) {
		Broodwar->drawTextScreen(100, 100, "Could not find window dims");
		return;
	}

	INPUT buffer;
	MouseSetup(&buffer);
	MouseMoveAbsolute(&buffer,
		rect.left + Broodwar->getFrameCount() % 640,
		rect.top + Broodwar->getFrameCount() % 480);
#endif

	//Broodwar->drawTextScreen(100, 100, "Inceceptor destroy score: %d",
	//	UnitTypes::Terran_Marine.destroyScore());

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

	//Broodwar->setTextSize(Text::Size::Large);
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
		//Broodwar->drawBoxScreen(next_x, topline, next_x + sq_size, topline + sq_size, Colors::Blue, true);
		next_x += sq_size + sq_size / 2;
		Broodwar->drawTextScreen(next_x, topline, "%d", player->gatheredMinerals() - player->spentMinerals());
		next_x += width_per_char * 4 + sq_size / 2;

		draw_bitmap(next_x, topline, gas_image);
		//Broodwar->drawBoxScreen(next_x, topline, next_x + sq_size, topline + sq_size, Colors::Green, true);
		next_x += sq_size + sq_size / 2;
		Broodwar->drawTextScreen(next_x, topline, "%d", player->gatheredGas() - player->spentGas());
		next_x += width_per_char * 4 + sq_size / 2;

		draw_bitmap(next_x, topline, food_image);
		//Broodwar->drawBoxScreen(next_x, topline, next_x + sq_size, topline + sq_size, Colors::White, true);
		next_x += sq_size + sq_size / 2;
		Broodwar->drawTextScreen(next_x, topline, "%d/%d", player->supplyUsed() / 2, player->supplyTotal() / 2);
		next_x += width_per_char * 6 + sq_size / 2;

		Broodwar->drawBoxScreen(first_x-5, top_box_height-2, next_x - sq_size / 2, top_box_height, player->getColor(), true);

		next_x += sq_size * 3;
#if 0
		Broodwar->drawTextScreen(
			next_x, topline, "%s  %d workers, %f/%f food",
			player->getName().c_str(),
			unit_stats[player->getID()].n_workers,
			unit_stats[player->getID()].used_food / 2.0,
			unit_stats[player->getID()].available_food / 2.0);

		Broodwar->drawTextScreen(
			10, next_y,
			"allUnitCount = %d, completedUnitCount = %d, deadUnitCount = %d\n"
			"gas=%d, gatheredGas=%d, gatheredMinerals=%d, minerals=%d\n"
			"refunded gas=%d, refunded minerals=%d, repaired gas=%d, repaired minerals=%d\n"
			"spent gas=%d, spent minerals=%d\n"
			"building score=%d, kill score=%d, unit score=%d\n"
			"incomplete unit count=%d, killed unit count=%d\n"
			"supply total=%f/%f/%f, supply used=%f/%f/%f",
			player->allUnitCount(),
			player->completedUnitCount(),
			player->deadUnitCount(),
			player->gas(),
			player->gatheredGas(),
			player->gatheredMinerals(),
			player->minerals(),
			player->refundedGas(),
			player->refundedMinerals(),
			player->repairedGas(),
			player->repairedMinerals(),
			player->spentGas(),
			player->spentMinerals(),
			player->getBuildingScore(),
			player->getKillScore(),
			player->getUnitScore(),
			player->incompleteUnitCount(),
			player->killedUnitCount(),
			player->supplyTotal(Races::Terran) /2.0,
			player->supplyTotal(Races::Zerg) /2.0,
			player->supplyTotal(Races::Protoss) /2.0,
			player->supplyUsed(Races::Terran) /2.0,
			player->supplyUsed(Races::Zerg) /2.0,
			player->supplyUsed(Races::Protoss) /2.0
			);

		next_x += 200;
		next_y += 150;
#endif
	}

	draw_workers();

#if 0
		// Finally make the unit do some stuff!

		// If the unit is a worker unit
		if (u->getType().isWorker())
		{
			// if our worker is idle
			if (u->isIdle())
			{
				// Order workers carrying a resource to return them to the center,
				// otherwise find a mineral patch to harvest.
				if (u->isCarryingGas() || u->isCarryingMinerals())
				{
					u->returnCargo();
				}
				else if (!u->getPowerUp())  // The worker cannot harvest anything if it
				{                             // is carrying a powerup such as a flag
											  // Harvest from the nearest mineral patch or gas refinery
					if (!u->gather(u->getClosestUnit(IsMineralField || IsRefinery)))
					{
						// If the call fails, then print the last error message
						Broodwar << Broodwar->getLastError() << std::endl;
					}

				} // closure: has no powerup
			} // closure: if idle

		}
		else if (u->getType().isResourceDepot()) // A resource depot is a Command Center, Nexus, or Hatchery
		{

			// Order the depot to construct more workers! But only when it is idle.
			if (u->isIdle() && !u->train(u->getType().getRace().getWorker()))
			{
				// If that fails, draw the error at the location so that you can visibly see what went wrong!
				// However, drawing the error once will only appear for a single frame
				// so create an event that keeps it on the screen for some frames
				Position pos = u->getPosition();
				Error lastErr = Broodwar->getLastError();
				Broodwar->registerEvent([pos, lastErr](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
					nullptr,    // condition
					Broodwar->getLatencyFrames());  // frames to run

													// Retrieve the supply provider type in the case that we have run out of supplies
				UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
				static int lastChecked = 0;

				// If we are supply blocked and haven't tried constructing more recently
				if (lastErr == Errors::Insufficient_Supply &&
					lastChecked + 400 < Broodwar->getFrameCount() &&
					Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
				{
					lastChecked = Broodwar->getFrameCount();

					// Retrieve a unit that is capable of constructing the supply needed
					Unit supplyBuilder = u->getClosestUnit(GetType == supplyProviderType.whatBuilds().first &&
						(IsIdle || IsGatheringMinerals) &&
						IsOwned);
					// If a unit was found
					if (supplyBuilder)
					{
						if (supplyProviderType.isBuilding())
						{
							TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
							if (targetBuildLocation)
							{
								// Register an event that draws the target build location
								Broodwar->registerEvent([targetBuildLocation, supplyProviderType](Game*)
								{
									Broodwar->drawBoxMap(Position(targetBuildLocation),
										Position(targetBuildLocation + supplyProviderType.tileSize()),
										Colors::Blue);
								},
									nullptr,  // condition
									supplyProviderType.buildTime() + 100);  // frames to run

																			// Order the builder to construct the supply structure
								supplyBuilder->build(supplyProviderType, targetBuildLocation);
							}
						}
						else
						{
							// Train the supply provider (Overlord) if the provider is not a structure
							supplyBuilder->train(supplyProviderType);
						}
					} // closure: supplyBuilder is valid
				} // closure: insufficient supply
			} // closure: failed to train idle unit

		}

	} // closure: unit iterator
#endif
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
