
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_CONTAINSTYPEOFSTDTUPLE_H
#define METAXXA_CONTAINSTYPEOFSTDTUPLE_H

#include <tuple>

namespace metaxxa
{
	namespace implementation
	{
		template <typename Tuple, typename Type, size_t INDEX>
		constexpr bool contains_type()
		{
			if constexpr (INDEX + 1 < std::tuple_size_v<Tuple>)
				return contains_type<Tuple, Type, INDEX + 1>();

			else
				return std::is_same_v<typename std::tuple_element<INDEX, Tuple>::type, Type>;
		}

		template <typename Tuple, typename Type, typename... Types>
		constexpr bool contains_types()
		{
			if constexpr (sizeof...(Types) == 1)
				return contains_type<Tuple, Type, 0>();
			else
				return contains_type<Tuple, Type, 0>() && contains_types<Tuple, Types...>();
		}
	}

	template <typename Tuple, typename Type>
	constexpr bool contains_type(typename std::enable_if<!std::is_same_v<Tuple, std::tuple<>>>::type * = nullptr)
	{
		return implementation::contains_type<Tuple, Type, 0>();
	}

	template <typename Tuple, typename Type>
	constexpr bool contains_type(typename std::enable_if<std::is_same_v<Tuple, std::tuple<>>>::type * = nullptr)
	{
		return false;
	}

	template <typename Tuple, typename... Types>
	constexpr bool contains_types(typename std::enable_if<!std::is_same_v<Tuple, std::tuple<>>>::type * = nullptr)
	{
		return implementation::contains_types<Tuple, Types...>();
	}

	template <typename Tuple, typename... Types>
	constexpr bool contains_types(typename std::enable_if<std::is_same_v<Tuple, std::tuple<>>>::type * = nullptr)
	{
		return false;
	}
}

#endif // METAXXA_CONTAINSTYPEOFSTDTUPLE_H