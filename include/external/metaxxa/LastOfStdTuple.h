
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_LASTOFSTDTUPLE_H
#define METAXXA_LASTOFSTDTUPLE_H

#include <tuple>

namespace metaxxa
{
	template <typename Tuple>
	constexpr auto &last(Tuple &tuple)
	{
		return std::get<std::tuple_size_v<Tuple> - 1>(tuple);
	}

	template <typename Tuple>
	constexpr auto last_types() -> typename std::tuple_element_t<std::tuple_size_v<Tuple> - 1, Tuple>;
}

#endif // METAXXA_LASTOFSTDTUPLE_H