
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_FOREACHOFSTDTUPLE_H
#define METAXXA_FOREACHOFSTDTUPLE_H

#include <tuple>
#include "Type.h"

namespace metaxxa
{
	namespace implementation
	{
		template <typename Tuple, typename Callable, size_t INDEX>
		constexpr void for_each(Tuple &tuple, Callable callable)
		{
			callable(std::get<INDEX>(tuple));

			if constexpr(INDEX + 1 < std::tuple_size<Tuple>::value)
				for_each<Tuple, Callable, INDEX + 1>(tuple, callable);
		}

		template
			<
			typename Tuple,
			template <typename ValueType> typename Functor,
			size_t INDEX,
			typename... FunctorArguments
			>
			constexpr void for_each_types(FunctorArguments&&... arguments)
		{
			using FunctorInstance = Functor<typename std::tuple_element<INDEX, Tuple>::type>;

			static_assert
			(
				Type<FunctorInstance>::template has_operator_call<void()>(),
				"metaxxa STATIC ERROR:  Functor has no 'void operator()()'"
			);

			FunctorInstance instance(arguments...);
			instance();

			if constexpr (INDEX + 1 < std::tuple_size_v<Tuple>)
				for_each_types<Tuple, Functor, INDEX + 1, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
		}
	}

	template <typename Tuple, typename Callable>
	constexpr void for_each(Tuple &tuple, Callable callable)
	{
		implementation::for_each<Tuple, Callable, 0>(tuple, callable);
	}

	template <typename Tuple, template <typename ValueType> typename Functor, typename... FunctorArguments>
	constexpr void for_each_types(FunctorArguments&... arguments)
	{
		implementation::for_each_types<Tuple, Functor, 0, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
	}

	template <typename Tuple, template <typename ValueType> typename Functor, typename... FunctorArguments>
	constexpr void for_each_types(FunctorArguments&&... arguments)
	{
		implementation::for_each_types<Tuple, Functor, 0, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
	}
}

#endif // METAXXA_FOREACHOFSTDTUPLE_H