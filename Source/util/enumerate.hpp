#pragma once

#include <experimental/generator>

template <typename T>
auto enumerate(T && sequence)
{
	int n = 0;
	for (auto && item : sequence) {
		co_yield std::make_pair(n, item); // probably should std::forward item
		++n;
	}
}
