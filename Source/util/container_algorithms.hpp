#pragma once

#include <algorithm>

namespace algo {

template<typename Container, typename Value>
auto find(Container const & c, Value const & v)
{
	return std::find(std::begin(c), std::end(c), v);
}

template<typename Container, typename Value>
auto contains(Container const & c, Value const & v)
{
	return find(c, v) != std::end(c);
}

}
