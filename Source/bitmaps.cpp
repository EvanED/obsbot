#pragma once

#include <utility>
#include "bitmaps.hpp"
#include "util/enumerate.hpp"

using BWAPI::Color;

typedef std::array<std::array<Color, 14>, 14> Bitmap;

// >>> from scipy import misc
// >>> arr = misc.imread('food.png')
// >>> for r in range(14):
// ...     print("    {")
// ...     for c in range(14):
// ...         print("        Color({}, {}, {}),".format(
// ...             arr[r, c, 0], arr[r, c, 1], arr[r, c, 2]))
// ...     print("    },")
// ...


const Bitmap minerals_image = {
	std::array<Color, 14> {
	Color(32, 24, 184),
		Color(32, 24, 184),
		Color(4, 4, 4),
		Color(32, 24, 184),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(4, 32, 100),
		Color(4, 32, 100),
		Color(4, 32, 100),
		Color(4, 32, 100),
		Color(4, 32, 100),
},
{
	Color(32, 24, 184),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(48, 80, 132),
	Color(56, 92, 148),
	Color(56, 92, 148),
	Color(12, 20, 36),
	Color(8, 8, 8),
	Color(4, 32, 100),
},
{
	Color(32, 24, 184),
	Color(8, 8, 8),
	Color(36, 52, 72),
	Color(36, 52, 72),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(72, 108, 80),
	Color(204, 248, 248),
	Color(140, 192, 216),
	Color(56, 92, 148),
	Color(12, 20, 36),
	Color(8, 8, 8),
	Color(0, 8, 80),
},
{
	Color(32, 24, 184),
	Color(36, 52, 72),
	Color(100, 148, 188),
	Color(88, 128, 164),
	Color(48, 80, 132),
	Color(8, 8, 8),
	Color(112, 168, 204),
	Color(112, 168, 204),
	Color(140, 192, 216),
	Color(56, 92, 148),
	Color(56, 92, 148),
	Color(56, 92, 148),
	Color(8, 8, 8),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(36, 52, 72),
	Color(116, 164, 124),
	Color(100, 148, 188),
	Color(56, 92, 148),
	Color(48, 80, 132),
	Color(72, 116, 180),
	Color(72, 116, 180),
	Color(72, 116, 180),
	Color(72, 116, 180),
	Color(72, 116, 180),
	Color(72, 104, 140),
	Color(52, 76, 108),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(52, 76, 108),
	Color(112, 168, 204),
	Color(140, 192, 216),
	Color(72, 104, 140),
	Color(112, 168, 204),
	Color(72, 116, 180),
	Color(56, 92, 148),
	Color(88, 128, 164),
	Color(56, 92, 148),
	Color(116, 164, 124),
	Color(116, 164, 124),
	Color(88, 128, 164),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(36, 60, 100),
	Color(112, 168, 204),
	Color(204, 248, 248),
	Color(48, 80, 132),
	Color(72, 116, 180),
	Color(72, 116, 180),
	Color(72, 116, 180),
	Color(56, 92, 148),
	Color(72, 116, 180),
	Color(112, 168, 204),
	Color(112, 168, 204),
	Color(48, 80, 132),
	Color(0, 8, 80),
},
{
	Color(4, 32, 100),
	Color(8, 8, 8),
	Color(56, 92, 148),
	Color(56, 92, 148),
	Color(48, 80, 132),
	Color(88, 128, 164),
	Color(100, 148, 188),
	Color(72, 116, 180),
	Color(56, 92, 148),
	Color(148, 220, 244),
	Color(116, 164, 124),
	Color(52, 76, 108),
	Color(48, 80, 132),
	Color(0, 8, 80),
},
{
	Color(4, 32, 100),
	Color(8, 8, 8),
	Color(48, 80, 132),
	Color(56, 92, 148),
	Color(48, 80, 132),
	Color(72, 116, 180),
	Color(88, 128, 164),
	Color(72, 116, 180),
	Color(48, 80, 132),
	Color(48, 80, 132),
	Color(56, 92, 148),
	Color(64, 88, 116),
	Color(48, 80, 132),
	Color(4, 32, 100),
},
{
	Color(8, 52, 152),
	Color(8, 8, 8),
	Color(52, 76, 108),
	Color(48, 80, 132),
	Color(48, 80, 132),
	Color(56, 92, 148),
	Color(100, 148, 188),
	Color(72, 116, 180),
	Color(56, 92, 148),
	Color(56, 92, 148),
	Color(56, 92, 148),
	Color(48, 80, 132),
	Color(0, 112, 156),
	Color(4, 32, 100),
},
{
	Color(8, 52, 152),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(36, 60, 100),
	Color(48, 80, 132),
	Color(24, 172, 184),
	Color(24, 172, 184),
	Color(56, 92, 148),
	Color(72, 116, 180),
	Color(72, 116, 180),
	Color(72, 116, 180),
	Color(72, 116, 180),
	Color(12, 12, 12),
	Color(4, 32, 100),
},
{
	Color(4, 32, 100),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(12, 20, 36),
	Color(48, 80, 132),
	Color(24, 172, 184),
	Color(24, 172, 184),
	Color(72, 116, 180),
	Color(100, 148, 188),
	Color(72, 116, 180),
	Color(72, 116, 180),
	Color(36, 60, 100),
	Color(12, 12, 12),
	Color(8, 52, 152),
},
{
	Color(4, 32, 100),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(28, 28, 44),
	Color(88, 128, 164),
	Color(24, 172, 184),
	Color(24, 172, 184),
	Color(24, 172, 184),
	Color(24, 172, 184),
	Color(24, 172, 184),
	Color(12, 20, 36),
	Color(8, 8, 8),
	Color(8, 52, 152),
},
{
	Color(4, 32, 100),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(4, 32, 100),
	Color(4, 32, 100),
	Color(4, 32, 100),
	Color(4, 32, 100),
	Color(8, 52, 152),
	Color(8, 52, 152),
	Color(32, 24, 184),
	Color(32, 24, 184),
},
};

const Bitmap gas_image = {
	std::array<Color, 14> {
	Color(32, 24, 184),
		Color(32, 24, 184),
		Color(4, 4, 4),
		Color(32, 24, 184),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(4, 32, 100),
		Color(4, 32, 100),
		Color(4, 32, 100),
		Color(4, 32, 100),
		Color(4, 32, 100),
},
{
	Color(32, 24, 184),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(140, 128, 52),
	Color(92, 84, 20),
	Color(92, 84, 20),
	Color(56, 48, 36),
	Color(56, 48, 36),
	Color(92, 84, 20),
	Color(116, 104, 56),
	Color(72, 48, 24),
	Color(8, 8, 8),
	Color(4, 32, 100),
},
{
	Color(32, 24, 184),
	Color(8, 8, 8),
	Color(44, 36, 24),
	Color(140, 84, 48),
	Color(0, 68, 0),
	Color(0, 100, 0),
	Color(36, 152, 36),
	Color(36, 152, 36),
	Color(36, 152, 36),
	Color(0, 100, 0),
	Color(0, 100, 0),
	Color(104, 60, 36),
	Color(40, 24, 12),
	Color(0, 8, 80),
},
{
	Color(32, 24, 184),
	Color(8, 8, 8),
	Color(40, 24, 12),
	Color(8, 8, 8),
	Color(0, 68, 0),
	Color(36, 152, 36),
	Color(60, 156, 60),
	Color(88, 176, 88),
	Color(60, 156, 60),
	Color(36, 152, 36),
	Color(0, 100, 0),
	Color(56, 48, 36),
	Color(40, 24, 12),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(56, 48, 36),
	Color(0, 100, 0),
	Color(0, 100, 0),
	Color(36, 152, 36),
	Color(36, 152, 36),
	Color(36, 152, 36),
	Color(0, 100, 0),
	Color(36, 152, 36),
	Color(56, 48, 36),
	Color(8, 8, 8),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(56, 48, 36),
	Color(56, 48, 36),
	Color(0, 68, 0),
	Color(0, 68, 0),
	Color(56, 48, 36),
	Color(56, 48, 36),
	Color(56, 48, 36),
	Color(88, 88, 88),
	Color(76, 76, 76),
	Color(8, 8, 8),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(56, 48, 36),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(104, 148, 108),
	Color(144, 184, 148),
	Color(60, 156, 60),
	Color(8, 8, 8),
	Color(0, 8, 80),
},
{
	Color(4, 32, 100),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(0, 68, 0),
	Color(76, 128, 80),
	Color(104, 148, 108),
	Color(104, 148, 108),
	Color(104, 148, 108),
	Color(104, 148, 108),
	Color(144, 184, 148),
	Color(104, 148, 108),
	Color(0, 100, 0),
	Color(8, 8, 8),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(0, 40, 0),
	Color(0, 100, 0),
	Color(0, 100, 0),
	Color(0, 100, 0),
	Color(36, 152, 36),
	Color(36, 152, 36),
	Color(0, 100, 0),
	Color(0, 100, 0),
	Color(0, 68, 0),
	Color(8, 8, 8),
	Color(4, 32, 100),
},
{
	Color(8, 52, 152),
	Color(8, 8, 8),
	Color(24, 20, 16),
	Color(56, 48, 36),
	Color(0, 68, 0),
	Color(0, 100, 0),
	Color(0, 100, 0),
	Color(0, 100, 0),
	Color(0, 100, 0),
	Color(0, 100, 0),
	Color(60, 156, 60),
	Color(92, 80, 64),
	Color(24, 20, 16),
	Color(4, 32, 100),
},
{
	Color(8, 52, 152),
	Color(8, 8, 8),
	Color(40, 24, 12),
	Color(72, 48, 24),
	Color(88, 88, 88),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(104, 104, 104),
	Color(104, 60, 36),
	Color(44, 36, 24),
	Color(4, 32, 100),
},
{
	Color(4, 32, 100),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(72, 48, 24),
	Color(92, 84, 20),
	Color(120, 76, 44),
	Color(120, 76, 44),
	Color(116, 104, 56),
	Color(116, 104, 56),
	Color(116, 104, 56),
	Color(112, 100, 32),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 52, 152),
},
{
	Color(4, 32, 100),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(44, 36, 24),
	Color(104, 60, 36),
	Color(120, 76, 44),
	Color(120, 76, 44),
	Color(104, 60, 36),
	Color(104, 60, 36),
	Color(44, 36, 24),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 52, 152),
},
{
	Color(4, 32, 100),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(4, 32, 100),
	Color(4, 32, 100),
	Color(4, 32, 100),
	Color(4, 32, 100),
	Color(8, 52, 152),
	Color(8, 52, 152),
	Color(32, 24, 184),
	Color(32, 24, 184),
},
};

const Bitmap food_image = {
	std::array<Color, 14> {
	Color(32, 24, 184),
		Color(32, 24, 184),
		Color(4, 4, 4),
		Color(32, 24, 184),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(8, 52, 152),
		Color(4, 32, 100),
		Color(4, 32, 100),
		Color(4, 32, 100),
		Color(4, 32, 100),
		Color(4, 32, 100),
},
{
	Color(32, 24, 184),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(4, 32, 100),
},
{
	Color(32, 24, 184),
	Color(96, 96, 124),
	Color(184, 184, 232),
	Color(255, 255, 255),
	Color(255, 255, 255),
	Color(104, 104, 148),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(144, 144, 184),
	Color(184, 184, 232),
	Color(255, 255, 255),
	Color(184, 184, 232),
	Color(96, 96, 124),
	Color(0, 8, 80),
},
{
	Color(32, 24, 184),
	Color(96, 96, 124),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(104, 104, 148),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(96, 96, 124),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(96, 96, 124),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(96, 96, 124),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(96, 96, 124),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(96, 96, 124),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(96, 96, 124),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(96, 96, 124),
	Color(184, 184, 232),
	Color(144, 144, 184),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(144, 144, 184),
	Color(144, 144, 184),
	Color(96, 96, 124),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(72, 72, 88),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(72, 72, 88),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(0, 8, 80),
},
{
	Color(4, 32, 100),
	Color(96, 96, 124),
	Color(8, 8, 8),
	Color(104, 104, 148),
	Color(255, 255, 255),
	Color(72, 72, 88),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(72, 72, 88),
	Color(144, 144, 184),
	Color(72, 72, 88),
	Color(8, 8, 8),
	Color(96, 96, 124),
	Color(0, 8, 80),
},
{
	Color(8, 52, 152),
	Color(96, 96, 124),
	Color(144, 144, 184),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(28, 28, 44),
	Color(8, 8, 8),
	Color(144, 144, 184),
	Color(184, 184, 232),
	Color(144, 144, 184),
	Color(96, 96, 124),
	Color(4, 32, 100),
},
{
	Color(8, 52, 152),
	Color(96, 96, 124),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(72, 72, 88),
	Color(8, 8, 8),
	Color(104, 104, 148),
	Color(144, 144, 184),
	Color(8, 8, 8),
	Color(72, 72, 88),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(96, 96, 124),
	Color(4, 32, 100),
},
{
	Color(8, 52, 152),
	Color(96, 96, 124),
	Color(184, 184, 232),
	Color(144, 144, 184),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(8, 8, 8),
	Color(8, 8, 8),
	Color(144, 144, 184),
	Color(184, 184, 232),
	Color(96, 96, 124),
	Color(4, 32, 100),
},
{
	Color(4, 32, 100),
	Color(96, 96, 124),
	Color(184, 184, 232),
	Color(64, 88, 116),
	Color(8, 8, 8),
	Color(40, 44, 104),
	Color(184, 184, 232),
	Color(184, 184, 232),
	Color(40, 44, 104),
	Color(8, 8, 8),
	Color(64, 88, 116),
	Color(184, 184, 232),
	Color(96, 96, 124),
	Color(8, 52, 152),
},
{
	Color(4, 32, 100),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(96, 96, 124),
	Color(8, 52, 152),
},
{
	Color(4, 32, 100),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(0, 8, 80),
	Color(4, 32, 100),
	Color(4, 32, 100),
	Color(4, 32, 100),
	Color(4, 32, 100),
	Color(8, 52, 152),
	Color(8, 52, 152),
	Color(32, 24, 184),
	Color(32, 24, 184),
},
};

void draw_bitmap(int left, int top, Bitmap const & bitmap)
{
	for (auto const & row : enumerate(bitmap)) {
		for (auto pixel : enumerate(row.second))
			BWAPI::Broodwar->drawDotScreen(left + pixel.first, top + row.first, pixel.second);
	}
}
