#pragma once

#include <BWAPI.h>
#include <array>

typedef std::array<std::array<BWAPI::Color, 14>, 14> Bitmap;

extern const Bitmap minerals_image;
extern const Bitmap gas_image;
extern const Bitmap food_image;

void draw_bitmap(int left, int top, Bitmap const & bitmap);
