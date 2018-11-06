
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_SUMOFSTDTUPLE_H
#define METAXXA_SUMOFSTDTUPLE_H

#include "Type.h"

namespace metaxxa
{
	namespace detail
	{
		template <typename Tuple, size_t INDEX>
		constexpr auto sum(Tuple &tuple)
		{
			if constexpr (INDEX + 1 == std::tuple_size<Tuple>::value)
				return std::get<INDEX>(tuple);
			else
				return std::get<INDEX>(tuple) + sum<Tuple, INDEX + 1>(tuple);
		}
	}

	template <typename Tuple>
	constexpr auto sum(Tuple &tuple)
	{
		return detail::sum<Tuple, 0>(tuple);
	}
}

#endif // METAXXA_SUMOFSTDTUPLE_H