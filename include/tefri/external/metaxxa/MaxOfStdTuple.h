
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_MAXOFSTDTUPLE_H
#define METAXXA_MAXOFSTDTUPLE_H

#include <tuple>
#include <algorithm>
#include "IsSameOfStdTuple.h"

#ifdef max
#define ___METAXXA___MAXOFSTDTUPLE_MAX_UNDEF___ max
#undef max
#endif // max

namespace metaxxa
{
	namespace detail
	{
		template <typename Tuple, size_t INDEX>
		constexpr auto max(Tuple &tuple)
		{
			if constexpr (INDEX + 2 == std::tuple_size<Tuple>::value)
				return std::max(std::get<INDEX>(tuple), std::get<INDEX + 1>(tuple));
			else
				return std::max(std::get<INDEX>(tuple), max<Tuple, INDEX + 1>(tuple));
		}
	}

	template <typename Tuple>
	constexpr auto max(Tuple &tuple)
	{
		return detail::max<Tuple, 0>(tuple);
	}
}

#ifdef ___METAXXA___MAXOFSTDTUPLE_MAX_UNDEF___
#define max ___METAXXA___MAXOFSTDTUPLE_MAX_UNDEF___
#endif // max

#endif // METAXXA_MAXOFSTDTUPLE_H