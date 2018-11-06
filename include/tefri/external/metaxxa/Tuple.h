
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_TUPLE_H
#define METAXXA_TUPLE_H

#include <tuple>
#include <optional>

#include "MoveStdTupleTypes.h"
#include "MoveStdTupleUniqueTypes.h"
#include "SkipFirstOfStdTuple.h"
#include "ForEachOfStdTuple.h"
#include "EveryOfStdTuple.h"
#include "IsSameOfStdTuple.h"
#include "MaxOfStdTuple.h"
#include "FirstOfStdTuple.h"
#include "LastOfStdTuple.h"
#include "MapOfStdTuple.h"
#include "SumOfStdTuple.h"
#include "ContainsTypesOfStdTuple.h"
#include "IsConvertsToTypesOfStdTuple.h"
#include "FindOfStdTuple.h"
#include "ToStringOfStdTuple.h"
#include "ExecuteFunctionsOfStdTuple.h"
#include "CallFunctionOfStdTuple.h"
#include "FilterOfStdTuple.h"
#include "WrapOfStdTuple.h"

namespace metaxxa
{
	template <typename... Arguments>
	class Tuple
	{
	public:
		using StdTuple = std::tuple<Arguments...>;

		template <template <typename...> typename TemplateType>
		using MoveTypesTo = MoveStdTupleTypes<TemplateType, StdTuple>;

		template <template <typename...> typename TemplateType>
		using MoveUniqueTypesTo = MoveStdTupleUniqueTypes<TemplateType, StdTuple>;

		template <size_t INDEX>
		using Parameter = typename std::tuple_element<INDEX, StdTuple>::type;

		Tuple() = default;

		Tuple(const Arguments&... arguments)
			: std_tuple(arguments...)
		{}

		template <typename... OtherArguments>
		Tuple(const Tuple<OtherArguments...> &other)
			: std_tuple(other.std_tuple)
		{}

		Tuple(const StdTuple &tuple)
			: std_tuple(tuple)
		{}
		
		Tuple(const Tuple &) = default;
		
		Tuple &operator=(const Tuple &tuple) = default;

		template <size_t INDEX>
		auto &get() const
		{
			return std::get<INDEX>(std_tuple);
		}

		template <size_t INDEX>
		auto &get()
		{
			return std::get<INDEX>(std_tuple);
		}

		static constexpr size_t get_size()
		{
			return std::tuple_size<StdTuple>::value;
		}

		static constexpr size_t size()
		{
			return get_size();
		}
		
		static constexpr bool is_empty()
		{
			return size() == 0;
		}

		template<typename Tuple>
		constexpr auto operator<=(Tuple &tuple) const
		{
			return std_tuple <= tuple.std_tuple;
		}

		template<typename Tuple>
		constexpr auto operator>=(Tuple &tuple) const
		{
			return std_tuple >= tuple.std_tuple;
		}

		template<typename Tuple>
		constexpr auto operator<(Tuple &tuple) const
		{
			return std_tuple < tuple.std_tuple;
		}

		template<typename Tuple>
		constexpr auto operator>(Tuple &tuple) const
		{
			return std_tuple > tuple.std_tuple;
		}

		template<typename... RHSArguments>
		constexpr auto operator==(const Tuple<RHSArguments...> &rhs) const
		{
			return std_tuple == rhs.std_tuple;
		}

		template<typename... RHSArguments>
		constexpr auto operator!=(const Tuple<RHSArguments...> &rhs) const
		{
			return std_tuple != rhs.std_tuple;
		}

		template <typename... RHSArguments>
		constexpr auto operator=(const Tuple<RHSArguments...> &rhs)
		{
			std_tuple = rhs.std_tuple;
			return *this;
		}

		template <typename Tuple>
		constexpr auto concat(Tuple &tuple) const
		{
			auto result = std::tuple_cat
			(
				std_tuple,
				tuple.std_tuple
			);

			return MoveStdTupleTypes<::metaxxa::Tuple, decltype(result)>(result);
		}

		template <typename Tuple>
		constexpr auto operator+(Tuple &tuple) const
		{
			return concat(tuple);
		}
		
		template <typename Tuple>
		constexpr auto concat(Tuple &&tuple)
		{
			auto result = std::tuple_cat
			(
				std_tuple,
				tuple.std_tuple
				);
			
			return MoveStdTupleTypes<::metaxxa::Tuple, decltype(result)>(result);
		}
		
		template <typename Tuple>
		constexpr auto operator+(Tuple &&tuple)
		{
			return concat(tuple);
		}

		template <size_t COUNT>
		constexpr auto skip_first() const
		{
			auto tuple = ::metaxxa::skip_first<COUNT>(std_tuple);

			return MoveStdTupleTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);
		}

		template <size_t COUNT>
		static constexpr auto skip_first_types() ->
			MoveStdTupleTypes
			<
				::metaxxa::Tuple,
				decltype(::metaxxa::skip_first_types<COUNT, StdTuple>())
			>;

		template <typename Callable>
		constexpr void for_each(Callable callable) const
		{
			::metaxxa::for_each(std_tuple, callable);
		}

		template 
		<
			template <typename ValueType> typename Functor,
			typename... FunctorArguments
		>
		static constexpr void for_each_types(FunctorArguments&&... arguments)
		{
			::metaxxa::for_each_types<StdTuple, Functor>(std::forward<FunctorArguments>(arguments)...);
		}

		template <typename Callable>
		constexpr bool every(Callable callable) const
		{
			return ::metaxxa::every(std_tuple, callable);
		}

		template
		<
			template <typename ValueType> typename Functor,
			typename... FunctorArguments
		>
		static constexpr bool every_types(FunctorArguments&&... arguments)
		{
			return ::metaxxa::every_types<StdTuple, Functor>(std::forward<FunctorArguments>(arguments)...);
		}

		constexpr bool is_same() const
		{
			return ::metaxxa::is_same(std_tuple);
		}

		static constexpr bool is_same_types() 
		{
			return ::metaxxa::is_same_types<StdTuple>();
		}

		constexpr auto max() const
		{
			return ::metaxxa::max(std_tuple);
		}

		constexpr auto &first() const
		{
			return ::metaxxa::first(std_tuple);
		}

		constexpr auto &first()
		{
			return ::metaxxa::first(std_tuple);
		}

		constexpr auto first_types() const -> decltype(::metaxxa::first_types<StdTuple>());

		constexpr auto &last() const
		{
			return ::metaxxa::last(std_tuple);
		}

		constexpr auto &last()
		{
			return ::metaxxa::last(std_tuple);
		}

		constexpr auto last_types() const -> decltype(::metaxxa::last_types<StdTuple>());


		template <typename Callable>
		constexpr auto map(Callable callable) const
		{
			auto tuple = ::metaxxa::map(std_tuple, callable);

			return MoveStdTupleTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);
		}

		template 
		<
			template <typename ValueType> typename Functor,
			typename... FunctorArguments
		>
		static constexpr auto map_types(FunctorArguments&&... arguments)
		{
			auto tuple = ::metaxxa::map_types<StdTuple, Functor, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);

			return MoveStdTupleTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);
		}

		constexpr auto sum() const
		{
			return ::metaxxa::sum(std_tuple);
		}

		template <typename Callable>
		auto find(Callable callable) const
		{
			return ::metaxxa::find(std_tuple, callable);
		}

		template
		<
			template <typename ValueType> typename Functor,
			typename... FunctorArguments
		>
		static constexpr auto find_types(FunctorArguments&&... arguments)
		{
			return ::metaxxa::find_types<StdTuple, Functor, FunctorArguments...>
			(
				std::forward<FunctorArguments>(arguments)...
			);
		}

		template <typename Callable>
		auto constexpr filter(Callable callable)
		{
			auto tuple = ::metaxxa::filter(std_tuple, callable);

			return MoveStdTupleTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);
		}

		template
		<
			template <typename ValueType> typename Functor,
			typename... FunctorArguments
		>
		static constexpr auto filter_types(FunctorArguments&&... arguments)
		{
			auto tuple = ::metaxxa::filter_types<StdTuple, Functor, FunctorArguments...>
			(
				std::forward<FunctorArguments>(arguments)...
			);

			return MoveStdTupleTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);

		} 

		template <typename Type>
		static constexpr bool contains_type()
		{
			return ::metaxxa::contains_type<StdTuple, Type>();
		}

		template <typename... Types>
		static constexpr bool contains_types()
		{
			return ::metaxxa::contains_types<StdTuple, Types...>();
		}

		template <typename Type>
		static constexpr bool is_converts_to_types()
		{
			return ::metaxxa::is_converts_to_types<StdTuple, Type>();
		}

		template <template <typename> typename TemplateType>
		constexpr auto wrap()
		{
			auto tuple = wrap_of_std_tuple<StdTuple, TemplateType>(std_tuple);

			return MoveStdTupleTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);
		}

		template <template <typename> typename TemplateType>
		static constexpr auto wrap_types()
		{
			return std::declval<MoveStdTupleTypes<::metaxxa::Tuple, decltype(wrap_of_std_tuple_types<StdTuple, TemplateType>())>>();
		}

		template <typename... RHSArguments>
		void execute_functions(RHSArguments&&... arguments) const
		{
			::metaxxa::execute_functions(std_tuple, std::forward<RHSArguments>(arguments)...);
		}

		template <typename Callable>
		auto call_function(Callable &callable) const
		{
			return ::metaxxa::call_function(std_tuple, callable);
		}

		std::string to_string() const
		{
			return ::metaxxa::to_string(std_tuple);
		}

		static std::string to_string_types() 
		{
			return ::metaxxa::to_string_types<StdTuple>();
		}

		template<typename...> friend class Tuple;
		friend class Tuple<>;

		template<typename...> friend class Tuple;
	
	private:
		StdTuple std_tuple;
	};

	template<>
	class Tuple<>
	{
	public:
		using StdTuple = std::tuple<>;

		template <template <typename...> typename TemplateType>
		using MoveTypesTo = TemplateType<>;

		template <template <typename...> typename TemplateType>
		using MoveUniqueTypesTo = TemplateType<>;

		Tuple() = default;

		Tuple(const std::tuple<> &) {}

		template<typename... Arguments>
		constexpr auto operator==(const Tuple<Arguments...> &rhs) const
		{
			return std_tuple == rhs.std_tuple;
		}

		template<typename... Arguments>
		constexpr auto operator!=(const Tuple<Arguments...> &rhs) const
		{
			return std_tuple != rhs.std_tuple;
		}

		template <typename Tuple>
		constexpr auto concat(Tuple &tuple) const
		{
			auto result = std::tuple_cat
			(
				std_tuple,
				tuple.std_tuple
			);

			return MoveStdTupleTypes<::metaxxa::Tuple, decltype(result)>(result);
		}

		template <typename Tuple>
		constexpr auto operator+(Tuple &tuple) const
		{
			return concat(tuple);
		}

		template <typename Tuple>
		constexpr auto concat(Tuple &&tuple)
		{
			auto result = std::tuple_cat
			(
				std_tuple,
				tuple.std_tuple
			);

			return MoveStdTupleTypes<::metaxxa::Tuple, decltype(result)>(result);
		}

		template <typename Tuple>
		constexpr auto operator+(Tuple &&tuple)
		{
			return concat(tuple);
		}

		static constexpr size_t get_size()
		{
			return 0;
		}

		static constexpr size_t size()
		{
			return get_size();
		}
		
		static constexpr bool is_empty()
		{
			return true;
		}

		template <typename Callable>
		constexpr void for_each(Callable callable) const
		{}

		template
		<
			template <typename ValueType> typename Functor,
			typename... FunctorArguments
		>
		static constexpr void for_each_types(FunctorArguments&&... arguments)
		{}

		template <typename Type>
		static constexpr bool contains_type()
		{
			return ::metaxxa::contains_type<StdTuple, Type>();
		}

		template <typename Callable>
		constexpr bool every(Callable callable) const
		{
			return false;
		}

		template
		<
			template <typename ValueType> typename Functor,
			typename... FunctorArguments
		>
		static constexpr bool every_types()
		{
			return false;
		}
		
		template <typename... Types>
		static constexpr bool contains_types()
		{
			return ::metaxxa::contains_types<StdTuple, Types...>();
		}

		template <typename Type>
		static constexpr bool is_converts_to_types()
		{
			return false;
		}

		template <template <typename> typename TemplateType>
		static constexpr auto wrap_all_types()
		{
			return std::tuple<>();
		}

		template <typename... RHSArguments>
		void execute_functions(RHSArguments&&... arguments) const
		{}
		
		template <typename Callable>
		auto call_function(Callable &callable) const
		{
			return callable();
		}

		std::string to_string() const
		{
			return ::metaxxa::to_string(std::tuple<>());
		}

	private:
		StdTuple std_tuple;

		template<typename...> friend class Tuple;
	};		

	template <typename... Arguments>
	auto tuple_ref(Arguments&&... arguments)
	{
		return Tuple<Arguments...>(std::forward<Arguments>(arguments)...);
	}

	template <typename... Arguments>
	auto tuple(Arguments... arguments)
	{
		Tuple<Arguments...> result(std::forward<Arguments>(arguments)...);

		return result;
	}
}

#endif // METAXXA_TUPLE_H