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

template <typename T1, typename T2>
auto zip(T1 && seq1, T2 && seq2)
{
	auto item1 = std::begin(seq1);
	auto item2 = std::begin(seq2);

	auto end1 = std::end(seq1);
	auto end2 = std::end(seq2);

	for (; item1 != end1 && item2 != end2; ++item1, ++item2) {
		co_yield std::make_pair(*item1, *item2);
	}
}
