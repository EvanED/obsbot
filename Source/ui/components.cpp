#include "components.hpp"
#include <BWAPI.h>
#include <sstream>

using BWAPI::Broodwar;
using BWAPI::PlayerInterface;

extern int get_worker_count(PlayerInterface * p);

void Columns::draw(BWAPI::Position p) const
{
	for (auto const & column : m_columns) {
		column->draw(p);
		p.x += column->m_width + m_col_sep;
	}
};

void Rows::draw(BWAPI::Position p) const
{
	for (auto const & row : m_rows) {
		row ->draw(p);
		p.y += row->m_height + m_row_sep;
	}
};

void Text::draw(BWAPI::Position p) const
{
	Broodwar->drawTextScreen(p, "%s", m_text.c_str());
};

void Background::draw(BWAPI::Position p) const
{
	Broodwar->drawBoxScreen(p.x, p.y, p.x + m_width, p.y + m_height, m_color, true);
	p.x += 1;
	p.y += 1;
	m_contents->draw(p);
};

void StatsBar::draw(BWAPI::Position p, int width, int height, int min_in_column, int max_in_column) const
{
	int v = value();
	int metric_span = max_in_column - min_in_column;
	int value_from_bottom = v - min_in_column;
	double proportion = static_cast<double>(value_from_bottom) / metric_span;
	int pixel_width = static_cast<int>(proportion * (width - 4));

	Broodwar->drawBoxScreen(p.x, p.y, p.x + width, p.y + height, m_background, true);
	Broodwar->drawBoxScreen(p.x + 1, p.y + 1, p.x + width - 2, p.y + height - 2, m_border, false);
	Broodwar->drawBoxScreen(p.x + 2, p.y + 2, p.x + 2 + pixel_width, height - 4, m_solid, true);

	std::stringstream ss;
	ss << v;
	std::string s = ss.str();
	int assumed_render_width = 5 * s.size();
	int size_on_side = (width - assumed_render_width) / 2;
	Broodwar->drawTextScreen(p.x + size_on_side, p.y + 2, "%s", s.c_str());
};

int StatsBar::value() const
{
	return m_func();
};

void StatsColumn::draw(BWAPI::Position p) const
{
	int col_min = 0, col_max = INT_MIN;
	for (auto const & gauge : m_gauges) {
		int v = gauge.value();
		col_min = std::min(col_min, v);
		col_max = std::max(col_max, v);
	}

	for (auto const & gauge : m_gauges) {
		gauge.draw(p, m_gauge_width, m_gauge_height, col_min, col_max);
		p.y += m_gauge_height + m_gauge_sep;
	}
};

std::unique_ptr<UiComponent>
build_economy_tab(std::vector<PlayerInterface*> const & players)
{
	std::vector<std::unique_ptr<UiComponent>>
		name_column_elts;

	std::vector<StatsBar>
		worker_column_elts,
		workers_lost_column_elts,
		mineral_income_column_elts,
		gas_income_column_elt;

	for (auto player : players) {
		name_column_elts.push_back(std::make_unique<Text>(player->getName()));
		workers_lost_column_elts.push_back(
			StatsBar([player] { return get_worker_count(player); }));
	}
	// https://stackoverflow.com/questions/26379311/
	typedef std::initializer_list<std::unique_ptr<UiComponent>> Components;

	std::unique_ptr<UiComponent>
		name_column = std::make_unique<Rows>(std::move(name_column_elts), 5),
		workers_lost_column = std::make_unique<StatsColumn>(
			std::move(workers_lost_column_elts), 40, 20, 3);

	std::vector<std::unique_ptr<UiComponent>>
		body_columns_elts;
	body_columns_elts.push_back(std::move(name_column));
	body_columns_elts.push_back(std::move(workers_lost_column));

	std::unique_ptr<UiComponent>
		body = std::make_unique<Columns>(std::move(body_columns_elts), 5);

	std::vector<std::unique_ptr<UiComponent>>
		rows_elts;
	rows_elts.push_back(std::make_unique<Text>("Economy"));
	rows_elts.push_back(std::move(body));

	auto whole = std::make_unique<Background>(
		std::make_unique<Rows>(std::move(rows_elts), 5),
		BWAPI::Colors::Black
	);

	return std::move(whole);
}