#include "components.hpp"
#include <BWAPI.h>
#include <sstream>

using BWAPI::Broodwar;
using BWAPI::PlayerInterface;

extern int get_worker_count(PlayerInterface const * p);
extern int get_workers_lost(PlayerInterface const * p);

using BWAPI::Position;
using BWAPI::Color;
namespace Colors = BWAPI::Colors;

const Color gauge_background = Colors::Black;
const Color gauge_border = Colors::White;
const Color gauge_solid = Colors::Blue;

const int row_height = 16; // we do fixed row height, not look at constituent.height
const int gauge_width = 40;
const int sep = 3;

typedef std::vector<Drawer> DrawList;

struct DrawInformation
{
	Drawer drawer;
	int width;
	int height;
};

typedef std::function<int()> Gauge;

void draw_gauge(Position p, int v, int col_min, int col_max)
{
	int metric_span = col_max - col_min;
	int value_from_bottom = v - col_min;
	double proportion = 
		(metric_span == 0)
		? 0
		: static_cast<double>(value_from_bottom) / metric_span;
	int pixel_width = static_cast<int>(proportion * (gauge_width - 2));

	Broodwar->drawBoxScreen(p.x, p.y, p.x + gauge_width, p.y + row_height, gauge_background, true);
	Broodwar->drawBoxScreen(p.x, p.y, p.x + gauge_width, p.y + row_height, gauge_border, false);
	Broodwar->drawBoxScreen(p.x + 1, p.y + 1, p.x + 1 + pixel_width, p.y + row_height - 2, gauge_solid, true);

	std::stringstream ss;
	ss << v;
	std::string s = ss.str();
	int assumed_render_width = 5 * s.size();
	int size_on_side = (gauge_width - assumed_render_width) / 2;
	Broodwar->drawTextScreen(p.x + size_on_side, p.y + 2, "%s", s.c_str());
}

DrawInformation text(std::string const & s)
{
	return {
		[s](Position p) {
			Broodwar->drawTextScreen(p, "%s", s.c_str());
		},
		static_cast<int>(s.size()) * 5, // XXX
		15
	};
}

DrawInformation column(std::initializer_list<DrawInformation> rows)
{
	std::vector<DrawInformation> rvec = rows;

	int max_width = 0;
	int total_height = 0;
	for (auto & row_di : rvec) {
		max_width = std::max(max_width, row_di.width);
		total_height += std::max(row_height, row_di.height) + sep;
	}

	return {
		[drawers = std::move(rvec)](Position p) {
			for (auto const & row_di : drawers) {
				row_di.drawer(p);
				p.y += std::max(row_height, row_di.height) + sep;
			}
		},
		max_width,
		total_height - sep
	};
}

DrawInformation row(std::initializer_list<DrawInformation> cols)
{
	std::vector<DrawInformation> cvec = cols;

	int total_width = 0;
	int max_height = 0;
	for (auto & col_di : cvec) {
		total_width += col_di.width + sep;
		max_height = std::max(max_height, col_di.height);
	}

	return {
		[drawers = std::move(cvec)](Position p) {
			for (auto const & col_di : drawers) {
				col_di.drawer(p);
				p.x += col_di.width + sep;
			}
		},
		total_width - sep,
		max_height
	};
}

Gauge gauge_worker_count(PlayerInterface const * p)
{
	return [p]() { return get_worker_count(p); };
}

Gauge gauge_workers_lost(PlayerInterface const * p)
{
	return [p]() { return get_workers_lost(p); };
}

Gauge gauge_army_supply(PlayerInterface const * p)
{
	return [p]() {
		return p->supplyUsed()/2 - get_worker_count(p);
	};
}

DrawInformation gauge_column(std::initializer_list<Gauge> gauges)
{
	std::vector<Gauge> gvec = gauges;

	return {
		[gauges = std::move(gvec)](Position p) {
			int col_min = 0, col_max = INT_MIN;
			for (auto const & gauge : gauges) {
				int v = gauge();
				col_min = std::min(col_min, v);
				col_max = std::max(col_max, v);
			}

			for (auto const & gauge : gauges) {
				draw_gauge(p, gauge(), col_min, col_max);
				p.y += row_height + sep;
			}
		},
		gauge_width,
		static_cast<int>(gvec.size()) - sep
	};
}

Drawer
build_economy_tab(std::vector<PlayerInterface*> const & players)
{
	return column({
		text("Economy"),
		row({
			column({
		        text(""),
				text(players[0]->getName()),
				text(players[1]->getName())
			}),
			column({
				text("Workers"),
				gauge_column({
					gauge_worker_count(players[0]),
					gauge_worker_count(players[1])
				})
			}),
			column({
				text("Lost"),
				gauge_column({
					gauge_workers_lost(players[0]),
					gauge_workers_lost(players[1])
				})
			})
		}),

		text("Army"),
		row({
			column({
				text(""),
				text(players[0]->getName()),
				text(players[1]->getName())
			}),
			column({
				text("Supply"),
				gauge_column({
					gauge_army_supply(players[0]),
					gauge_army_supply(players[1])
				})
			})
		})
	}).drawer;
}
