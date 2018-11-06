
//
// Created by Daniel Shiposha on 12 August 2018
// 



#ifndef METAXXA_FUNCTION_H
#define METAXXA_FUNCTION_H

#include <functional>

#include "Tuple.h"
#include "Type.h"

namespace metaxxa
{
	namespace detail
	{
		template <typename ArgumentType>
		struct ArgumentsSignatureBuilder
		{
			ArgumentsSignatureBuilder(std::string &result)
				: result(result)
			{}

			void operator()()
			{
				if (!result.empty())
					result += ", ";
				result += Type<ArgumentType>::name_with_modifiers();
			}

		private:
			std::string &result;
		};
	}

	template<typename T>
	struct Function : Function<decltype(&T::operator())>
	{
	};


	template<typename ResultType, typename... FunctionArguments>
	struct Function<ResultType(FunctionArguments...)>
	{
		using Result = ResultType;
		using Arguments = Tuple<FunctionArguments...>;

		template <size_t INDEX>
		using Argument = std::tuple_element_t<INDEX, typename Arguments::StdTuple>;

		static constexpr size_t argument_count() { return Arguments::size(); }

		static std::string get_signature()
		{
			std::string arguments;
			Arguments::template for_each_types<detail::ArgumentsSignatureBuilder>(arguments);

			return Type<Result>::name_with_modifiers() + " (" + arguments + ")";
		}

		static std::string signature()
		{
			return get_signature();
		}
	};


	template<typename ResultType, typename... FunctionArguments>
	struct Function<ResultType(*)(FunctionArguments...)>
	{
		using Result = ResultType;
		using Arguments = Tuple<FunctionArguments...>;
		
		template <size_t INDEX>
		using Argument = std::tuple_element_t<INDEX, typename Arguments::StdTuple>;

		static constexpr size_t argument_count() { return Arguments::size(); }

		static std::string get_signature()
		{
			std::string arguments;
			Arguments::template for_each_types<detail::ArgumentsSignatureBuilder>(arguments);

			return Type<Result>::name_with_modifiers() + " (" + arguments + ")";
		}

		static std::string signature()
		{
			return get_signature();
		}

	};


	template<typename ResultType, typename... FunctionArguments>
	struct Function<std::function<ResultType(FunctionArguments...)>>
	{
		using Result = ResultType;
		using Arguments = Tuple<FunctionArguments...>;
		
		template <size_t INDEX>
		using Argument = std::tuple_element_t<INDEX, typename Arguments::StdTuple>;

		static constexpr size_t argument_count() { return Arguments::size(); }

		static std::string get_signature()
		{
			std::string arguments;
			Arguments::template for_each_types<detail::ArgumentsSignatureBuilder>(arguments);

			return Type<Result>::name_with_modifiers() + " (" + arguments + ")";
		}

		static std::string signature()
		{
			return get_signature();
		}
	};


	template<typename SomeType, typename ResultType, typename... FunctionArguments>
	struct Function<ResultType(SomeType::*)(FunctionArguments...)>
	{
		using Result = ResultType;
		using Arguments = Tuple<FunctionArguments...>;
		
		template <size_t INDEX>
		using Argument = std::tuple_element_t<INDEX, typename Arguments::StdTuple>;

		static constexpr size_t argument_count() { return Arguments::size(); }

		static constexpr bool is_const() { return false; }

		static std::string get_signature()
		{
			std::string arguments;
			Arguments::template for_each_types<detail::ArgumentsSignatureBuilder>(arguments);

			return Type<Result>::name_with_modifiers() + " (" + arguments + ")";
		}

		static std::string signature()
		{
			return get_signature();
		}
	};

	template<typename SomeType, typename ResultType, typename... FunctionArguments>
	struct Function<ResultType(SomeType::*)(FunctionArguments...) const>
	{
		using Result = ResultType;
		using Arguments = Tuple<FunctionArguments...>;
		
		template <size_t INDEX>
		using Argument = std::tuple_element_t<INDEX, typename Arguments::StdTuple>;

		static constexpr size_t argument_count() { return Arguments::size(); }

		static constexpr bool is_const() { return true; }

		static std::string get_signature()
		{
			std::string arguments;
			Arguments::template for_each_types<detail::ArgumentsSignatureBuilder>(arguments);

			return Type<Result>::name_with_modifiers() + " (" + arguments + ") const";
		}

		static std::string signature()
		{
			return get_signature();
		}
	};


	// Specializations for functions without arguments
	// {

	template<typename ResultType>
	struct Function<ResultType()>
	{
		using Result = ResultType;
		using Arguments = Tuple<>;

		static constexpr size_t argument_count() { return 0; }

		static std::string get_signature()
		{
			std::string result = Type<Result>::name_with_modifiers() + " ()";

			return result;
		}

		static std::string signature()
		{
			return get_signature();
		}
	};


	template<typename ResultType>
	struct Function<ResultType(*)()>
	{
		using Result = ResultType;
		using Arguments = Tuple<>;

		static constexpr size_t argument_count() { return 0; }

		static std::string get_signature()
		{
			std::string result = Type<Result>::name_with_modifiers() + " ()";

			return result;
		}

		static std::string signature()
		{
			return get_signature();
		}

	};


	template<typename ResultType>
	struct Function<std::function<ResultType()>>
	{
		using Result = ResultType;
		using Arguments = Tuple<>;

		static constexpr size_t argument_count() { return 0; }

		static std::string get_signature()
		{
			std::string result = Type<Result>::name_with_modifiers() + " ()";

			return result;
		}

		static std::string signature()
		{
			return get_signature();
		}
	};


	template<typename SomeType, typename ResultType>
	struct Function<ResultType(SomeType::*)()>
	{
		using Result = ResultType;
		using Arguments = Tuple<>;

		static constexpr size_t argument_count() { return 0; }

		static constexpr bool is_const() { return false; }

		static std::string get_signature()
		{
			std::string result = Type<Result>::name_with_modifiers() + " ()";

			return result;
		}

		static std::string signature()
		{
			return get_signature();
		}
	};

	template<typename SomeType, typename ResultType>
	struct Function<ResultType(SomeType::*)() const>
	{
		using Result = ResultType;
		using Arguments = Tuple<>;

		static constexpr size_t argument_count() { return 0; }

		static constexpr bool is_const() { return true; }

		static std::string get_signature()
		{
			std::string result = Type<Result>::name_with_modifiers() + " () const";

			return result;
		}

		static std::string signature()
		{
			return get_signature();
		}
	};

	// }

	template <typename FirstCallable, typename SecondCallable>
	constexpr auto operator==(Function<FirstCallable> &&, Function<SecondCallable> &&)
	{
		return Type<typename Function<FirstCallable>::Arguments>() == Type<typename Function<SecondCallable>::Arguments>()
			&& Type<typename Function<FirstCallable>::Result>() == Type<typename Function<SecondCallable>::Result>();
	}

	template <typename FirstCallable, typename SecondCallable>
	constexpr auto operator!=(Function<FirstCallable> &&first, Function<SecondCallable> &&second)
	{
		return !(first == second);
	}
}

#endif //METAXXA_FUNCTION_H