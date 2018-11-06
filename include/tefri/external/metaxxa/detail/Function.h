#ifndef METAXXA_detail_FUNCTION_H
#define METAXXA_detail_FUNCTION_H

#include <functional>
#include <type_traits>
#include <tuple>

namespace metaxxa::detail
{
	template<typename T>
	struct Function : Function<decltype(&T::operator())>
	{
	};


	template<typename ResultType, typename... FunctionArguments>
	struct Function<ResultType(FunctionArguments...)>
	{
		using Result = ResultType;
		using StdTupleOfArguments = typename std::tuple<FunctionArguments...>;

		template <size_t INDEX>
		using Argument = typename std::tuple_element_t<INDEX, StdTupleOfArguments>;

		static constexpr size_t ARGUMENT_COUNT = std::tuple_size_v<StdTupleOfArguments>;
	};


	template<typename ResultType, typename... FunctionArguments>
	struct Function<ResultType(*)(FunctionArguments...)>
	{
		using Result = ResultType;
		using StdTupleOfArguments = typename std::tuple<FunctionArguments...>;

		template <size_t INDEX>
		using Argument = typename std::tuple_element_t<INDEX, StdTupleOfArguments>;

		static constexpr size_t ARGUMENT_COUNT = std::tuple_size_v<StdTupleOfArguments>;
	};


	template<typename ResultType, typename... FunctionArguments>
	struct Function<std::function<ResultType(FunctionArguments...)>>
	{
		using Result = ResultType;
		using StdTupleOfArguments = typename std::tuple<FunctionArguments...>;

		template <size_t INDEX>
		using Argument = typename std::tuple_element_t<INDEX, StdTupleOfArguments>;

		static constexpr size_t ARGUMENT_COUNT = std::tuple_size_v<StdTupleOfArguments>;
	};


	template<typename SomeType, typename ResultType, typename... FunctionArguments>
	struct Function<ResultType(SomeType::*)(FunctionArguments...)>
	{
		using Result = ResultType;
		using StdTupleOfArguments = typename std::tuple<FunctionArguments...>;

		template <size_t INDEX>
		using Argument = typename std::tuple_element_t<INDEX, StdTupleOfArguments>;

		static constexpr size_t ARGUMENT_COUNT = std::tuple_size_v<StdTupleOfArguments>;
	};

	template<typename SomeType, typename ResultType, typename... FunctionArguments>
	struct Function<ResultType(SomeType::*)(FunctionArguments...) const>
	{
		using Result = ResultType;
		using StdTupleOfArguments = typename std::tuple<FunctionArguments...>;

		template <size_t INDEX>
		using Argument = typename std::tuple_element_t<INDEX, StdTupleOfArguments>;

		static constexpr size_t ARGUMENT_COUNT = std::tuple_size_v<StdTupleOfArguments>;
	};


	// Specializations for functions without arguments
	// {

	template<typename ResultType>
	struct Function<ResultType()>
	{
		using Result = ResultType;
		using StdTupleOfArguments = typename std::tuple<>;

		static constexpr size_t ARGUMENT_COUNT = 0;
	};


	template<typename ResultType>
	struct Function<ResultType(*)()>
	{
		using Result = ResultType;
		using StdTupleOfArguments = typename std::tuple<>;

		static constexpr size_t ARGUMENT_COUNT = 0;
	};


	template<typename ResultType>
	struct Function<std::function<ResultType()>>
	{
		using Result = ResultType;
		using StdTupleOfArguments = typename std::tuple<>;

		static constexpr size_t ARGUMENT_COUNT = 0;
	};


	template<typename SomeType, typename ResultType>
	struct Function<ResultType(SomeType::*)()>
	{
		using Result = ResultType;
		using StdTupleOfArguments = typename std::tuple<>;

		static constexpr size_t ARGUMENT_COUNT = 0;
	};

	template<typename SomeType, typename ResultType>
	struct Function<ResultType(SomeType::*)() const>
	{
		using Result = ResultType;
		using StdTupleOfArguments = typename std::tuple<>;

		static constexpr size_t ARGUMENT_COUNT = 0;
	};

	// }

	template <typename FirstCallable, typename SecondCallable>
	constexpr auto operator==(Function<FirstCallable> &&, Function<SecondCallable> &&)
	{
		return std::is_same_v
			<
			Function<FirstCallable>::StdTupleOfArguments,
			Function<SecondCallable>::StdTupleOfArguments
			> && std::is_same_v<Function<FirstCallable>::Result, Function<SecondCallable>::Result>;
	}

	template <typename FirstCallable, typename SecondCallable>
	constexpr auto operator!=(Function<FirstCallable> &&first, Function<SecondCallable> &&second)
	{
		return !(first == second);
	}
}

#endif // METAXXA_detail_FUNCTION_H