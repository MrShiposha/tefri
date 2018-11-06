
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_ISCONVERTSTOTYPESOFSTDTUPLE_H
#define METAXXA_ISCONVERTSTOTYPESOFSTDTUPLE_H

#include <tuple>

namespace metaxxa
{
	namespace detail
	{
		template <typename Tuple, typename Type, size_t INDEX>
		constexpr bool is_converts_to_types()
		{
			if constexpr 
			(
				std::is_same_v<Type, std::tuple_element_t<INDEX, Tuple>>
				|| std::is_convertible_v<Type, std::tuple_element_t<INDEX, Tuple>>
			)
				return true;

			else if constexpr (INDEX + 1 < std::tuple_size_v<Tuple>)
				return is_converts_to_types<Tuple, Type, INDEX + 1>();

			else
				return false;
		}
	}

	template <typename Tuple, typename Type>
	constexpr bool is_converts_to_types()
	{
		return detail::is_converts_to_types<Tuple, Type, 0>();
	}
}

#endif // METAXXA_ISCONVERTSTOTYPESOFSTDTUPLE_H