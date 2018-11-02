
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_ISSAMEOFSTDTUPLE_H
#define METAXXA_ISSAMEOFSTDTUPLE_H

#include <tuple>

namespace metaxxa
{
	namespace implementation
	{
		template <typename Tuple, size_t INDEX>
		constexpr bool is_same(Tuple &tuple)
		{
			if constexpr(std::tuple_size<Tuple>::value - (INDEX + 1) < 2)
				return true;
			else
				return std::get<INDEX>(tuple) == std::get<INDEX + 1>(tuple)
						&& is_same<Tuple, INDEX + 1>(tuple);
		}

		template <typename Tuple, size_t INDEX>
		constexpr bool is_same_types()
		{
			if constexpr(std::tuple_size<Tuple>::value - (INDEX + 1) < 2)
				return true;
			else
				return std::is_same_v<typename std::tuple_element<INDEX, Tuple>::type, typename std::tuple_element<INDEX + 1, Tuple>::type>
						&& is_same_types<Tuple, INDEX + 1>();
		}
	}

	template <typename Tuple>
	constexpr bool is_same(Tuple &tuple)
	{
		return implementation::is_same<Tuple, 0>(tuple);
	}

	template <typename Tuple>
	constexpr bool is_same_types()
	{
		return implementation::is_same_types<Tuple, 0>();
	}
}

#endif // METAXXA_ISSAMEOFSTDTUPLE_H