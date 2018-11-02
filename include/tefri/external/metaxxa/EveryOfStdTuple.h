
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_EVERYOFSTDTUPLE_H
#define METAXXA_EVERYOFSTDTUPLE_H


#include <tuple>
#include "Type.h"

namespace metaxxa
{
	namespace implementation
	{
		template <typename Tuple, typename Callable, size_t INDEX>
		constexpr bool every(Tuple &tuple, Callable callable)
		{
			if constexpr (INDEX + 1 == std::tuple_size<Tuple>::value)
				return callable(std::get<INDEX>(tuple));
			else
				return callable(std::get<INDEX>(tuple)) && every<Tuple, Callable, INDEX + 1>(tuple, callable);
		}

		template 
		<
			typename Tuple,
			template <typename ValueType> typename Functor,
			size_t INDEX,
			typename... FunctorArguments
		>
		constexpr bool every_types(FunctorArguments&&... arguments)
		{
			using FunctorInstance = Functor<typename std::tuple_element<INDEX, Tuple>::type>;

			static_assert
			(
				Type<FunctorInstance>::template has_operator_call<bool()>(),
				"metaxxa STATIC ERROR:  Functor has no non-const 'bool operator()()'"
			);


			if constexpr (INDEX + 1 == std::tuple_size<Tuple>::value)
				return FunctorInstance(std::forward<FunctorArguments>(arguments)...)();
			else
				return FunctorInstance(std::forward<FunctorArguments>(arguments)...)() 
				&& every_types<Tuple, Functor, INDEX + 1, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
		}
	}

	template <typename Tuple, typename Callable>
	constexpr bool every(Tuple &tuple, Callable callable)
	{
		return implementation::every<Tuple, Callable, 0>(tuple, callable);
	}

	template 
	<
		typename Tuple, 
		template <typename ValueType> typename Functor, 
		typename... FunctorArguments
	>
	constexpr bool every_types(FunctorArguments&&... arguments)
	{
		return implementation::every_types<Tuple, Functor, 0, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
	}
}

#endif // METAXXA_EVERYOFSTDTUPLE_H