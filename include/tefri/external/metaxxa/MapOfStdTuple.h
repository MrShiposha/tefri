
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_MAPOFSTDTUPLE_H
#define METAXXA_MAPOFSTDTUPLE_H

#include "Type.h"
#include <tuple>

namespace metaxxa
{
	namespace implementation
	{
		template <typename Tuple, typename Callable, size_t INDEX>
		constexpr auto map(Tuple &tuple, Callable callable)
		{
			auto mapped_tuple = std::make_tuple(callable(std::get<INDEX>(tuple)));

			if constexpr (INDEX + 1 == std::tuple_size<Tuple>::value)
				return mapped_tuple;
			else
				return std::tuple_cat(mapped_tuple, map<Tuple, Callable, INDEX + 1>(tuple, callable));
		}

		template 
		<
			typename Tuple, 
			template <typename ValueType> typename Functor,
			size_t INDEX, 
			typename... FunctorArguments
		>
		constexpr auto map_types(FunctorArguments&&... arguments)
		{
			using FunctorInstance = Functor<typename std::tuple_element<INDEX, Tuple>::type>;

			// TODO: check for non-void return of FunctorInstance::operator()

			auto mapped_tuple = std::make_tuple(FunctorInstance(std::forward<FunctorArguments>(arguments)...)());

			if constexpr (INDEX + 1 == std::tuple_size<Tuple>::value)
				return mapped_tuple;
			else
				return std::tuple_cat
				(
					mapped_tuple, 
					map_types<Tuple, Functor, INDEX + 1, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...)
				);
		}
	}


	template <typename Tuple, typename Callable>
	constexpr auto map(Tuple &tuple, Callable callable)
	{
		return implementation::map<Tuple, Callable, 0>(tuple, callable);
	}

	template 
	<
		typename Tuple, 
		template <typename ValueType> typename Functor,
		typename... FunctorArguments
	>
	constexpr auto map_types(FunctorArguments&&... arguments)
	{
		return implementation::map_types<Tuple, Functor, 0, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
	}
}

#endif // METAXXA_MAPOFSTDTUPLE_H