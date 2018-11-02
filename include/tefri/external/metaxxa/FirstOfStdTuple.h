
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_FIRSTOFSTDTUPLE_H
#define METAXXA_FIRSTOFSTDTUPLE_H

#include <tuple>

namespace metaxxa
{
	template <typename Tuple>
	constexpr auto &first(Tuple &tuple)
	{
		return std::get<0>(tuple);
	}

	template <typename Tuple>
	constexpr auto first_types() -> typename std::tuple_element_t<0, Tuple>;
}

#endif // METAXXA_FIRSTOFSTDTUPLE_H