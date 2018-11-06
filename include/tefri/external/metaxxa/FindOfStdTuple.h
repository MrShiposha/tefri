
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_FINDOFSTDTUPLE_H
#define METAXXA_FINDOFSTDTUPLE_H

#include <tuple>
#include <optional>
#include <variant>

#include "MoveStdTupleUniqueTypes.h"
#include "ContainsTypesOfStdTuple.h"
#include "WrapOfStdTuple.h"

namespace metaxxa
{
	namespace detail
	{
		template <typename Tuple, typename Callable, size_t INDEX>
		auto find(Tuple &tuple, Callable &callable)
			-> std::optional
			<
				metaxxa::MoveStdTupleUniqueTypes<std::variant, decltype(::metaxxa::wrap_of_std_tuple_types<Tuple, std::remove_cv_t>())>
			>
		{
			if (callable(std::get<INDEX>(tuple)))
				return std::optional
				<
					metaxxa::MoveStdTupleUniqueTypes<std::variant, decltype(::metaxxa::wrap_of_std_tuple_types<Tuple, std::remove_cv_t>())>
				>(std::get<INDEX>(tuple));

			else if (INDEX + 1 < std::tuple_size<Tuple>::value)
				return find<Tuple, Callable, INDEX + 1>(tuple, callable);

			else
				return std::optional
				<
					metaxxa::MoveStdTupleUniqueTypes<std::variant, decltype(::metaxxa::wrap_of_std_tuple_types<Tuple, std::remove_cv_t>())>
				>();
		}

		template <typename Tuple, size_t FOUND_INDEX>
		struct OptionalFind
		{
			static constexpr bool FOUND = true;
			static constexpr size_t INDEX = FOUND_INDEX;

			using Type = typename std::tuple_element_t<FOUND_INDEX, Tuple>;

			template <typename OrType>
			using TypeOr = Type;
		};

		template <typename Tuple>
		struct OptionalFind<Tuple, static_cast<size_t>(-1)>
		{
			static constexpr bool FOUND = false;

			template <typename OrType>
			using TypeOr = OrType;
		};

		template 
		<
			typename Tuple, 
			template <typename ValueType> typename Functor, 
			size_t INDEX,
			typename... FunctorArguments
		>
		constexpr size_t find_types_index(FunctorArguments&&... arguments)
		{
			using FunctorInstance = Functor<typename std::tuple_element<INDEX, Tuple>::type>;

			static_assert
			(
				Type<FunctorInstance>::template has_operator_call<bool()>(),
				"metaxxa STATIC ERROR:  Functor has no 'bool operator()()'"
			);

			FunctorInstance functor(std::forward<FunctorArguments>(arguments)...);

			if constexpr (functor())
				return INDEX;
			
			else if constexpr (INDEX + 1 < std::tuple_size_v<Tuple>)
				return find_types_index<Tuple, Functor, INDEX + 1, FunctorArguments...>
				(
					std::forward<FunctorArguments>(arguments)...
				);

			else
				return static_cast<size_t>(-1); // -1 -- is NOT FOUND value
		}

		template 
		<
			typename Tuple, 
			template <typename ValueType> typename Functor,
			typename... FunctorArguments
		>
		constexpr auto find_types(FunctorArguments&&... arguments)
			
		{
			return OptionalFind
			<
				Tuple,
				find_types_index<Tuple, Functor, 0, FunctorArguments...>
				(
					std::forward<FunctorArguments>(arguments)...
				)
			>();
		}

	}

	template <typename Tuple, typename Callable>
	auto find(Tuple &tuple, Callable &callable) 
	{
		return detail::find<Tuple, Callable, 0>(tuple, callable);
	}

	template 
	<
		typename Tuple, 
		template <typename ValueType> typename Functor,
		typename... FunctorArguments
	>
	constexpr auto find_types(FunctorArguments&&... arguments)
	{
		return detail::find_types<Tuple, Functor, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
	}
}

#endif // METAXXA_FINDOFSTDTUPLE_H