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

class UiComponent
{
public:
	UiComponent(int w, int h)
		: m_width(w), m_height(h)
	{}

	virtual ~UiComponent() {}

	virtual void draw(BWAPI::Position p) const = 0;

	const int m_width;
	const int m_height;
};

class Text
	: public UiComponent
{
	std::string m_text;
public:
	Text(std::string s)
		: UiComponent(5 * s.size(), 20), m_text(std::move(s))
	{}

	virtual void draw(BWAPI::Position p) const override;
};

class Background
	: public UiComponent
{
	std::unique_ptr<UiComponent> m_contents;
	BWAPI::Color m_color;
public:
	Background(std::unique_ptr<UiComponent> && c, BWAPI::Color color)
		: UiComponent(c->m_width + 2, c->m_height + 2)
		, m_contents(std::move(c))
		, m_color(color)
	{}

	virtual void draw(BWAPI::Position p) const override;
};

class StatsBar
{
	const BWAPI::Color m_background, m_border, m_solid, m_text;
	std::function<int()> m_func;

public:
	StatsBar(std::function<int()> f)
		: m_func(std::move(f))
	{}

	void draw(BWAPI::Position p, int width, int height, int min_in_column, int max_in_column) const;
	int value() const;
};

class StatsColumn
	: public UiComponent
{
	std::vector<StatsBar> m_gauges;
	int m_gauge_width, m_gauge_height, m_gauge_sep;

public:
	StatsColumn(std::vector<StatsBar> gauges, int column_width, int gauge_height, int gauge_sep)
		: UiComponent(column_width, (gauge_height + gauge_sep) * gauges.size() - gauge_sep)
		, m_gauges(std::move(gauges))
		, m_gauge_width(column_width)
		, m_gauge_height(gauge_height)
		, m_gauge_sep(gauge_sep)
	{}

	virtual void draw(BWAPI::Position p) const override;
};

class Columns
	: public UiComponent
{
	std::vector<std::unique_ptr<UiComponent>> m_columns;
	int m_col_sep;

	static int total_width(std::vector<std::unique_ptr<UiComponent>> const & cols, int sep)
	{
		assert(!cols.empty());
		int w = 0;
		for (auto const & col : cols) {
			w += col->m_width;
		}
		w += (cols.size() - 1)*sep;
		return w;
	}

	static int max_height(std::vector<std::unique_ptr<UiComponent>> const & cols)
	{
		int w = 0;
		for (auto const & col : cols) {
			w = std::max(col->m_height, w);
		}
		return w;
	}

public:
	Columns(std::vector<std::unique_ptr<UiComponent>> && columns, int col_sep)
		: UiComponent(total_width(columns, col_sep), max_height(columns))
		, m_columns(std::move(columns))
		, m_col_sep(col_sep)
	{}

	virtual void draw(BWAPI::Position p) const override;
};

class Rows
	: public UiComponent
{
	std::vector<std::unique_ptr<UiComponent>> m_rows;
	int m_row_sep;

	static int total_height(std::vector<std::unique_ptr<UiComponent>> const & rows, int sep)
	{
		assert(!rows.empty());
		int w = 0;
		for (auto const & row : rows) {
			w += row->m_height;
		}
		w += (rows.size() - 1)*sep;
		return w;
	}

	static int max_width(std::vector<std::unique_ptr<UiComponent>> const & rows)
	{
		int w = 0;
		for (auto const & row : rows) {
			w = std::max(row->m_width, w);
		}
		return w;
	}

public:
	Rows(std::vector<std::unique_ptr<UiComponent>> && rows, int row_sep)
		: UiComponent(max_width(rows), total_height(rows, row_sep))
		, m_rows(std::move(rows))
		, m_row_sep(row_sep)
	{}

	virtual void draw(BWAPI::Position p) const override;
};

std::unique_ptr<UiComponent>
build_economy_tab(std::vector<BWAPI::PlayerInterface *> const & players);
