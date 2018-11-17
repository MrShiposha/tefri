// MIT License
// 
// Copyright (c) 2018 Daniel Shiposha
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 

#ifndef METAXXA_H
#define METAXXA_H

#if defined(_MSC_VER)
    #define METAXXA_TEMPLATE
#elif defined(__clang__)
    #define METAXXA_TEMPLATE template
#elif defined(__GNUC__)
    #define METAXXA_TEMPLATE template
#else
    #ifndef METAXXA_TEMPLATE
        #error METAXXA: You are using an unknown compiler. Please specify the METAXXA_TEMPLATE macro
    #endif // METAXXA_TEMPLATE
#endif // Compilers


#include <tuple>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <functional>
#include <utility>
#include <string>
#include <cstring>
#include <memory>
#include <algorithm>

#if __has_include(<optional>)
    #include <optional>

    #define METAXXA_OPTIONAL std::optional
    

#elif __has_include(<experimental/optional>)
    #include <experimental/optional>

    #define METAXXA_OPTIONAL std::experimental::optional
#else
    #error METAXXA: Sorry, your compiler does not support neither std::optional or std::experimental::optional
#endif // Check optional



namespace metaxxa
{
	namespace detail
	{
		template <size_t INDEX, typename Tuple, bool ENOUGH>
		struct SkipFirstTypes
		{
			static constexpr auto skip_first_types() ->
				decltype
				(
					std::tuple_cat
					(
						std::declval<typename std::tuple<typename std::tuple_element<INDEX, Tuple>::type>>(),
						std::declval
						<
							decltype
							(
								SkipFirstTypes<INDEX + 1, Tuple, INDEX + 2 == std::tuple_size<Tuple>::value>
								::skip_first_types()
							)
						>()
					)
				);
		};

		template <size_t INDEX, typename Tuple>
		struct SkipFirstTypes<INDEX, Tuple, true>
		{
			static constexpr auto skip_first_types() ->
				typename std::tuple<typename std::tuple_element<INDEX, Tuple>::type>;
		};
	}

	template <size_t INDEX, typename Tuple>
	constexpr auto skip_first(Tuple &tuple)
	{
		if constexpr(INDEX + 1 > std::tuple_size<Tuple>::value)
			return std::tuple<>();
		else
			return std::tuple_cat
			(
				std::make_tuple(std::get<INDEX>(tuple)),
				skip_first<INDEX + 1>(tuple)
			);
	}

	template <size_t INDEX, typename Tuple>
	constexpr auto skip_first_types() ->
		decltype
		(
			detail::SkipFirstTypes<INDEX, Tuple, INDEX + 1 == std::tuple_size<Tuple>::value>::skip_first_types()
		);
}





namespace metaxxa::detail
{
    template <typename Type>
    struct IsTemplate
    {
        template <template <typename...> typename TemplateType, typename... Args>
        static int16_t check(TemplateType<Args...> &&);

        static int8_t check(...);

        static constexpr bool RESULT = sizeof(int16_t) == sizeof(check(std::declval<Type>()));
    };

    template <>
    struct IsTemplate<void>
    {
        static constexpr bool RESULT = false;
    };

    template <typename Type>
    constexpr bool IS_TEMPLATE = IsTemplate<Type>::RESULT; 
}


namespace metaxxa::detail
{
    template <template <typename...> typename DestTemplate, template <typename...> typename SrcTemplate, typename... Args>
    constexpr auto move_template_types(SrcTemplate<Args...> &&) -> DestTemplate<Args...>;    

    template <template <typename...> typename DestTemplate, typename SrcTemplate, bool IS_TEMPLATE = IS_TEMPLATE<SrcTemplate>>
    struct MoveTemplateTypesImpl 
    {
        using Result = void;
    };

    template <template <typename...> typename DestTemplate, typename SrcTemplate>
    struct MoveTemplateTypesImpl<DestTemplate, SrcTemplate, true>
    {
        using Result = decltype(move_template_types<DestTemplate>(std::declval<SrcTemplate>()));
    };

#ifndef _MSC_VER
    template <template <typename...> typename DestTemplate, typename SrcTemplate>
    using MoveTemplateTypes = typename MoveTemplateTypesImpl<DestTemplate, SrcTemplate>::Result;

#else
    template 
    <
        template <typename...> typename TemplateType,
        typename Tuple, 
        bool ENOUGH,
        size_t INDEX,
        size_t TYPES_COUNT,
        typename... Types
    >
    struct MoveStdTupleTypesProxy : MoveStdTupleTypesProxy
    <
        TemplateType, 
        Tuple, 
        TYPES_COUNT == INDEX + 1,
        INDEX + 1,
        TYPES_COUNT,
        Types...,
        typename std::tuple_element<INDEX, Tuple>::type
    >
    {
    };

    template
    <
        template <typename...> typename TemplateType,
        typename Tuple,
        size_t INDEX,
        size_t TYPES_COUNT,
        typename... Types
    >
    struct MoveStdTupleTypesProxy<TemplateType, Tuple, true, INDEX, TYPES_COUNT, Types...>
    {
        using ResultType = TemplateType<Types...>;
    };

    template <template <typename...> typename TemplateType, typename Tuple>
    struct MoveStdTupleTypes
    {
        using ResultType = typename MoveStdTupleTypesProxy
            <
                TemplateType, 
                Tuple, 
                std::tuple_size<Tuple>::value == 1,
                1,
                std::tuple_size<Tuple>::value,
                typename std::tuple_element<0, Tuple>::type
            >::ResultType;
    };

    template <template <typename...> typename TemplateType>
    struct MoveStdTupleTypes<TemplateType, std::tuple<>>
    {
        using ResultType = TemplateType<>;
    };

    template <template <typename...> typename DestTemplate, typename SrcTemplate>
    using MoveTemplateTypes = typename MoveStdTupleTypes<DestTemplate, typename MoveTemplateTypesImpl<std::tuple, SrcTemplate>::Result>::ResultType;

#endif // _MSC_VER
}





namespace metaxxa
{
	namespace detail
	{
		template <typename Tuple, typename Type, size_t INDEX>
		constexpr bool contains(Tuple &tuple, Type &value)
		{
			if constexpr (INDEX == std::tuple_size_v<Tuple>)
				return false;
			else
			{
				if (std::get<INDEX>(tuple) == value)
					return true;
				else
					return contains<Tuple, Type, INDEX + 1>(tuple, value);
			}
		}

		template <typename Tuple, typename Type, size_t INDEX>
		constexpr bool contains_type()
		{
			if constexpr (INDEX + 1 == std::tuple_size_v<Tuple>)
				return std::is_same_v<typename std::tuple_element_t<INDEX, Tuple>, Type>;
			else if constexpr (std::is_same_v<typename std::tuple_element_t<INDEX, Tuple>, Type>)
				return true;
			else
				return contains_type<Tuple, Type, INDEX + 1>();
		}

		struct NoType {};

		template <typename Tuple, typename Type = NoType, typename... Types>
		constexpr bool contains_types()
		{
			if constexpr (std::is_same_v<Type, NoType>)
				return true;
			else if constexpr (sizeof...(Types) == 0)
				return contains_type<Tuple, Type, 0>();
			else
				return contains_type<Tuple, Type, 0>() && contains_types<Tuple, Types...>();
		}
	}

	template <typename Tuple, typename Type>
	constexpr bool contains(Tuple &tuple, Type &value)
	{
		return detail::contains<Tuple, Type, 0>(tuple, value);
	}

	template <typename Tuple, typename Type>
	constexpr bool contains(Tuple &tuple, Type &&value)
	{
		return detail::contains<Tuple, Type, 0>(tuple, value);
	}

	template <typename Tuple, typename... Types>
	constexpr bool contains_types(typename std::enable_if<!std::is_same_v<Tuple, std::tuple<>>>::type * = nullptr)
	{
		return detail::contains_types<Tuple, Types...>();
	}

	template <typename Tuple, typename... Types>
	constexpr bool contains_types(typename std::enable_if<std::is_same_v<Tuple, std::tuple<>>>::type * = nullptr)
	{
		return false;
	}
}



namespace metaxxa
{
    namespace detail
    {
        template <typename SrcTuple, typename DistinctTuple, size_t INDEX>
        constexpr auto distinct_types()
        {
            using ConcatTuple = decltype(std::tuple_cat(std::declval<DistinctTuple>(), std::declval<std::tuple<std::tuple_element_t<INDEX, SrcTuple>>>())); 

            if constexpr (INDEX + 1 == std::tuple_size_v<SrcTuple> && !::metaxxa::contains_types<DistinctTuple, std::tuple_element_t<INDEX, SrcTuple>>())
                return std::declval<ConcatTuple>();
            else if constexpr (INDEX + 1 == std::tuple_size_v<SrcTuple>)
                return std::declval<DistinctTuple>();
            else if constexpr (!::metaxxa::contains_types<DistinctTuple, std::tuple_element_t<INDEX, SrcTuple>>())
                return distinct_types<SrcTuple, ConcatTuple, INDEX + 1>();
            else
                return distinct_types<SrcTuple, DistinctTuple, INDEX + 1>();
        }
    }

    template <typename Tuple>
    constexpr auto distinct_types(std::enable_if_t<!std::is_same_v<Tuple, std::tuple<>>> * = nullptr)
    {
        return detail::distinct_types<Tuple, std::tuple<>, 0>();
    }

    template <typename Tuple>
    constexpr auto distinct_types(std::enable_if_t<std::is_same_v<Tuple, std::tuple<>>> * = nullptr)
    {
        return std::tuple<>();
    }
}



namespace metaxxa::detail
{
    template <template <typename...> typename TemplateType, typename Type, bool IS_TYPE_TEMPLATE = IS_TEMPLATE<Type>>
    struct MoveTemplateTypesUniqueImpl
    {};

    template <template <typename...> typename TemplateType, typename Type>
    struct MoveTemplateTypesUniqueImpl<TemplateType, Type, true>
    {
        using Result = detail::MoveTemplateTypes<TemplateType, decltype(::metaxxa::distinct_types<detail::MoveTemplateTypes<std::tuple, Type>>())>;
    };

    template <template <typename...> typename TemplateType, typename Type>
	using MoveTemplateTypesUnique = typename MoveTemplateTypesUniqueImpl<TemplateType, Type>::Result;
}




namespace metaxxa
{
    namespace detail
    {
        template <template <typename...> typename TemplateType>
        struct IsInstantiationOf
        {
            template <typename... Parameters>
            struct check : std::false_type {};

            template <typename... Parameters>
            struct check<TemplateType<Parameters...>> : std::true_type {};
        };

        template <template <typename...> typename TemplateType, typename... Parameters>
		constexpr bool IS_INSTANTIATION_OF = IsInstantiationOf<TemplateType>::template check<Parameters...>::value;
    }
}



namespace metaxxa
{
    namespace detail
    {
        template <template <typename...> typename Template, typename TypeToWrap, bool IS_WRAPPED = IS_INSTANTIATION_OF<Template, TypeToWrap>>
        struct WrapToTemplateIfNotWrapped
        {};

        template <template <typename...> typename Template, typename TypeToWrap>
        struct WrapToTemplateIfNotWrapped<Template, TypeToWrap, false> 
        {
            using Result = Template<TypeToWrap>;
        };

        template <template <typename...> typename Template, typename TypeToWrap>
        struct WrapToTemplateIfNotWrapped<Template, TypeToWrap, true> 
        {
            using Result = TypeToWrap;
        };
    }
}



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



#define ___METAXXA___EXPAND_DEEP(...) __VA_ARGS__

#define METAXXA_EXPAND_MACRO(...) ___METAXXA___EXPAND_DEEP(__VA_ARGS__)



namespace metaxxa
{
	namespace detail
	{
		template <typename SomeType, typename Callable, bool IS_ENOUGH, typename... Arguments>
		struct CallableToMethod 
			: CallableToMethod
			<
				SomeType, 
				Callable, 
				sizeof...(Arguments) + 1 >= Function<Callable>::ARGUMENT_COUNT,
				Arguments..., typename Function<Callable>::template Argument<sizeof...(Arguments)>
			>
			{
			};

		template <typename SomeType, typename Callable, typename... Arguments>
		struct CallableToMethod<SomeType, Callable, true, Arguments...>
		{
			using MethodResult = typename Function<Callable>::Result;

			using Method = MethodResult(SomeType::*)(Arguments...);
		};
	}

	template <typename SomeType, typename Callable>
	using CallableToMethod = typename detail::CallableToMethod<SomeType, Callable, detail::Function<Callable>::ARGUMENT_COUNT == 0>::Method;
}


#define ___METAXXA___DECLARE_OPERATOR_TESTER(TESTER_NAME, OPERATOR) \
	template <typename Type, typename Method> \
	struct ___METAXXA___OPERATOR_TESTER_##TESTER_NAME \
	{\
	private:\
		template <typename T> struct helper;\
	\
		template <typename T>\
		constexpr static std::uint8_t check(helper<decltype(static_cast<metaxxa::CallableToMethod<T, Method>>(&T::OPERATOR))> *);\
	\
		template <typename T> \
		constexpr static std::uint16_t check(...); \
		static constexpr bool __check_value = sizeof(check<Type>(0)) == sizeof(std::uint8_t); \
	public: \
		static constexpr bool has() \
		{\
			return __check_value;\
		}\
	}; \
	template <typename Type> \
	struct ___METAXXA___OPERATOR_TESTER_##TESTER_NAME <Type, void> \
	{\
	private:\
		template <typename T> struct helper;\
	\
		template <typename T>\
		constexpr static std::uint8_t check(helper<decltype(&T::OPERATOR)> *);\
	\
		template <typename T> \
		constexpr static std::uint16_t check(...); \
		static constexpr bool __check_value = sizeof(check<Type>(0)) == sizeof(std::uint8_t); \
	public: \
		static constexpr bool has() \
		{\
			return __check_value;\
		}\
	}

#define ___METAXXA___OPERATOR_TESTER(OPERATOR) \
	___METAXXA___OPERATOR_TESTER_##OPERATOR

#define ___METAXXA___DECLARE_POSTCINDITION_OPERATOR_CHECKER(OPERATOR, CONDITION) \
	template <typename SomeType, bool PRECONDITION> \
	struct ___METAXXA___POSTCONDITION_OPERATOR_CHECKER_##OPERATOR \
    {}; \
	template <typename SomeType> \
	struct ___METAXXA___POSTCONDITION_OPERATOR_CHECKER_##OPERATOR<SomeType, true> \
    { static constexpr bool check() { return CONDITION; } }; \
	template <typename SomeType> \
	struct ___METAXXA___POSTCONDITION_OPERATOR_CHECKER_##OPERATOR<SomeType, false> \
    { static constexpr bool check() { return false; } }

#define ___METAXXA___POSTCONDITION_OPERATOR_CHECKER(OPERATOR) \
	___METAXXA___POSTCONDITION_OPERATOR_CHECKER_##OPERATOR


#define ___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_UNARY(OPERATOR_NAME, OPERATOR) \
	METAXXA_EXPAND_MACRO(___METAXXA___DECLARE_POSTCINDITION_OPERATOR_CHECKER(OPERATOR_NAME, detail::Function<decltype(&SomeType::OPERATOR)>::ARGUMENT_COUNT == 0))

#define ___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_BINARY(OPERATOR_NAME, OPERATOR) \
	METAXXA_EXPAND_MACRO(___METAXXA___DECLARE_POSTCINDITION_OPERATOR_CHECKER(OPERATOR_NAME, detail::Function<decltype(&SomeType::OPERATOR)>::ARGUMENT_COUNT > 0))


namespace metaxxa::detail
{
	___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_UNARY(unary_plus,    operator+);
	___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_UNARY(unary_minus,   operator-);
	___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_UNARY(dereference,   operator*);
	___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_UNARY(address,       operator&);
	___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_BINARY(binary_plus,  operator+);
	___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_BINARY(binary_minus, operator-);
	___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_BINARY(multiply,     operator*);
	___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_BINARY(bit_and,      operator&);


	// ASSIGNMENT 
	// {
	___METAXXA___DECLARE_OPERATOR_TESTER(assign,             operator=);
	___METAXXA___DECLARE_OPERATOR_TESTER(plus_assign,        operator+=);
	___METAXXA___DECLARE_OPERATOR_TESTER(subtract_assign,    operator-=);
	___METAXXA___DECLARE_OPERATOR_TESTER(multiply_assign,    operator*=);
	___METAXXA___DECLARE_OPERATOR_TESTER(divide_assign,      operator/=);
	___METAXXA___DECLARE_OPERATOR_TESTER(mod_assign,         operator%=);
	___METAXXA___DECLARE_OPERATOR_TESTER(bit_and_assign,     operator&=);
	___METAXXA___DECLARE_OPERATOR_TESTER(bit_or_assign,      operator|=);
	___METAXXA___DECLARE_OPERATOR_TESTER(bit_xor_assign,     operator^=);
	___METAXXA___DECLARE_OPERATOR_TESTER(left_shift_assign,  operator<<=);
	___METAXXA___DECLARE_OPERATOR_TESTER(right_shift_assign, operator>>=);
	// }

	// (IN|DE)CREMENT
	// {
	___METAXXA___DECLARE_OPERATOR_TESTER(increment, operator++);
	___METAXXA___DECLARE_OPERATOR_TESTER(decrement, operator--);
	// }

	// ARITHMETIC
	// {
	___METAXXA___DECLARE_OPERATOR_TESTER(plus,        operator+);
	___METAXXA___DECLARE_OPERATOR_TESTER(minus,       operator-);
	___METAXXA___DECLARE_OPERATOR_TESTER(divide,      operator/);
	___METAXXA___DECLARE_OPERATOR_TESTER(mod,         operator%);
	___METAXXA___DECLARE_OPERATOR_TESTER(tilde,       operator~);
	___METAXXA___DECLARE_OPERATOR_TESTER(bit_or,      operator|);
	___METAXXA___DECLARE_OPERATOR_TESTER(bit_xor,     operator^);
	___METAXXA___DECLARE_OPERATOR_TESTER(left_shift,  operator<<);
	___METAXXA___DECLARE_OPERATOR_TESTER(right_shift, operator>>);
	// }


	// LOGICAL
	// {
	___METAXXA___DECLARE_OPERATOR_TESTER(not,         operator!);
	___METAXXA___DECLARE_OPERATOR_TESTER(logical_and, operator&&);
	___METAXXA___DECLARE_OPERATOR_TESTER(logical_or,  operator||);
	// }

	// COMPARISON
	// {
	___METAXXA___DECLARE_OPERATOR_TESTER(equal,         operator==);
	___METAXXA___DECLARE_OPERATOR_TESTER(not_equal,     operator!=);
	___METAXXA___DECLARE_OPERATOR_TESTER(less,          operator<);
	___METAXXA___DECLARE_OPERATOR_TESTER(greater,       operator>);
	___METAXXA___DECLARE_OPERATOR_TESTER(less_equal,    operator<=);
	___METAXXA___DECLARE_OPERATOR_TESTER(greater_equal, operator>=);
	// }

	// MEMBER ACCESS
	// {
	___METAXXA___DECLARE_OPERATOR_TESTER(subscript,                operator[]);
	___METAXXA___DECLARE_OPERATOR_TESTER(pointer_access,           operator->);
	___METAXXA___DECLARE_OPERATOR_TESTER(pointer_to_member_access, operator->*);
	// }

	// OTHER
	// {
	___METAXXA___DECLARE_OPERATOR_TESTER(asterisk,  operator*);
	___METAXXA___DECLARE_OPERATOR_TESTER(ampersand, operator&);
	___METAXXA___DECLARE_OPERATOR_TESTER(call,      operator());

	template <typename Type, typename Method> 
	struct ___METAXXA___OPERATOR_TESTER(comma) 
	{
	private:
		template <typename T> struct helper;
			
		template <typename T>
		constexpr static std::uint8_t check(helper<decltype(static_cast<metaxxa::CallableToMethod<T, Method>>(&T::operator,))> *);
			
		template <typename T> 
		constexpr static std::uint16_t check(...); 
		static constexpr bool __check_value = sizeof(check<Type>(0)) == sizeof(std::uint8_t); 
	public: 
		static constexpr bool has() 
		{
			return __check_value;
		}
	}; 
	template <typename Type> 
	struct ___METAXXA___OPERATOR_TESTER(comma) <Type, void> 
	{
		private:
		template <typename T> struct helper;
			
		template <typename T>
		constexpr static std::uint8_t check(helper<decltype(&T::operator,)> *);
			
		template <typename T> 
		constexpr static std::uint16_t check(...); 
		static constexpr bool __check_value = sizeof(check<Type>(0)) == sizeof(std::uint8_t);
	public: 
		static constexpr bool has() 
		{
			return __check_value;
		}
	};


	template <typename Type, typename ToType, typename Method>
	struct ___METAXXA___OPERATOR_TESTER(cast_operator)
	{
	private:
		template <typename T> struct helper;

		template <typename T>
		constexpr static std::uint8_t check(helper<decltype(static_cast<metaxxa::CallableToMethod<T, Method>>(&T::operator ToType))> *);

		template <typename T>
		constexpr static std::uint16_t check(...);
		static constexpr bool __check_value = sizeof(check<Type>(0)) == sizeof(std::uint8_t);
	public:
		static constexpr bool has()
		{
			return __check_value;
		}
	};
	template <typename Type, typename ToType>
	struct ___METAXXA___OPERATOR_TESTER(cast_operator) <Type, ToType, void>
	{
	private:
		template <typename T> struct helper;

		template <typename T>
		constexpr static std::uint8_t check(helper<decltype(&T::operator ToType)> *);

		template <typename T>
		constexpr static std::uint16_t check(...);
		static constexpr bool __check_value = sizeof(check<Type>(0)) == sizeof(std::uint8_t);
	public:
		static constexpr bool has()
		{
			return __check_value;
		}
	};

	// }
}


#define ___METAXXA___TYPE___DECLARE_UNARY_OPERATOR(SomeType, OPERATOR) \
	template <typename Callable> \
	static constexpr bool has_operator_##OPERATOR() \
	{ \
		return detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, Callable>::has(); \
	} \
	static constexpr bool has_single_operator_##OPERATOR() \
	{ \
		return detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has(); \
	} 

#define ___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, OPERATOR) \
	template <typename Callable> \
	static constexpr bool has_operator_##OPERATOR() \
	{ \
		return detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, Callable>::has(); \
	} \
	static constexpr bool has_single_operator_##OPERATOR() \
	{ \
		return detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has(); \
	} 

#define ___METAXXA___TYPE___DECLARE_UNARY_AND_BINARY_OPERATOR(SomeType, OPERATOR) \
	template <typename Callable> \
	static constexpr bool has_operator_##OPERATOR() \
	{ \
		return detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, Callable>::has(); \
	} \
	static constexpr bool has_single_operator_unary_##OPERATOR() \
	{ \
		return detail::___METAXXA___POSTCONDITION_OPERATOR_CHECKER_unary_##OPERATOR<SomeType, detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has()>::check(); \
	} \
	static constexpr bool has_single_operator_binary_##OPERATOR() \
	{ \
		return detail::___METAXXA___POSTCONDITION_OPERATOR_CHECKER_binary_##OPERATOR<SomeType, detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has()>::check(); \
	}

#define ___METAXXA___TYPE___DECLARE_NAMED_UNARY_AND_BINARY_OPERATOR(SomeType, OPERATOR, UNARY_NAME, BINARY_NAME) \
	template <typename Callable> \
	static constexpr bool has_operator_##UNARY_NAME() \
	{ \
		return detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, Callable>::has(); \
	} \
	static constexpr bool has_single_operator_##OPERATOR##_##UNARY_NAME() \
	{ \
		return detail::___METAXXA___POSTCONDITION_OPERATOR_CHECKER_##UNARY_NAME<SomeType, detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has()>::check(); \
	} \
	template <typename Callable> \
	static constexpr bool has_operator_##BINARY_NAME() \
	{ \
		return detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, Callable>::has(); \
	} \
	static constexpr bool has_single_operator_##OPERATOR##_##BINARY_NAME() \
	{ \
		return detail::___METAXXA___POSTCONDITION_OPERATOR_CHECKER_##BINARY_NAME<SomeType, detail::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has()>::check(); \
	}





#define ___METAXXA___DECLARE_HAS_METHOD(METHOD) \
	template <typename Type, typename Method> \
	struct ___METAXXA___METHOD_TESTER_##METHOD \
	{\
	private:\
		template <typename T> struct helper;\
	\
		template <typename T>\
		constexpr static std::uint8_t check(helper<decltype(static_cast<metaxxa::CallableToMethod<T, Method>>(&T::METHOD))> *);\
	\
		template <typename T> \
		constexpr static std::uint16_t check(...); \
		static constexpr bool __check_value = sizeof(check<Type>(0)) == sizeof(std::uint8_t); \
	public: \
		static constexpr bool has() \
		{\
			return __check_value;\
		}\
	}; \
	template <typename Type> \
	struct ___METAXXA___METHOD_TESTER_##METHOD <Type, void> \
	{\
	private:\
		template <typename T> struct helper;\
	\
		template <typename T>\
		constexpr static std::uint8_t check(helper<decltype(&T::METHOD)> *);\
	\
		template <typename T> \
		constexpr static std::uint16_t check(...); \
		static constexpr bool __check_value = sizeof(check<Type>(0)) == sizeof(std::uint8_t); \
	public: \
		static constexpr bool has() \
		{\
			return __check_value;\
		}\
	};\
	template <typename SomeType, typename Callable> \
	static constexpr bool has_method_##METHOD() \
	{ \
		return ___METAXXA___METHOD_TESTER_##METHOD<SomeType, Callable>::has(); \
	} \
	template <typename SomeType>\
	static constexpr bool has_method_##METHOD() \
	{ \
		return ___METAXXA___METHOD_TESTER_##METHOD<SomeType, void>::has(); \
	} 



namespace metaxxa::detail
{
	___METAXXA___DECLARE_HAS_METHOD(to_string);		
}



namespace metaxxa
{
	namespace detail
	{
		template <typename Type, typename Argument>
		struct IsExplicitlyConstructible
			: std::integral_constant
			<
				bool,
				std::is_constructible<Type, Argument>::value && !std::is_convertible<Argument, Type>::value
			>
		{
		};

		template <typename Type, typename Argument>
		constexpr bool IS_EXPLICITLY_CONSTRUCTIBLE = IsExplicitlyConstructible<Type, Argument>::value;
	}
}



namespace metaxxa
{
	namespace detail
	{
		template <typename Type, typename Argument>
		struct IsImplicitlyConstructible
			: std::integral_constant
			<
				bool,
				std::is_constructible<Type, Argument>::value && std::is_convertible<Argument, Type>::value
			>
		{
		};

		template <typename Type, typename Argument>
		constexpr bool IS_IMPLICITLY_CONSTRUCTIBLE = IsImplicitlyConstructible<Type, Argument>::value;
	}
}



#if __has_include(<cxxabi.h>)
#define ___METAXXA___HAS_CXX_ABI
#include <cxxabi.h>
#endif // __has_include(<cxxabi.h>)

namespace metaxxa::detail
{
    template <typename T>
    class CVRSaver
    {};

    template <typename T>
    std::string demangle()
    {
        static const char cvr_saver_name[] = "metaxxa::detail::CVRSaver<";
        static constexpr std::string::size_type cvr_saver_name_length = sizeof(cvr_saver_name) - 1;

    const char *begin = nullptr;
    #ifdef _MSC_VER
        // Nothing to do, already demangled
        begin = typeid(CVRSaver<T>).name();
    #elif defined(___METAXXA___HAS_CXX_ABI)
        int status;

        auto deleter = [](char *p) { free(p); };
        std::unique_ptr<char, decltype(deleter)> real_name
        (
            abi::__cxa_demangle(typeid(CVRSaver<T>).name(), 0, 0, &status),
            deleter
        );

        begin = real_name.get();

    #else
       begin = typeid(CVRSaver<T>).name();
    #endif

        const std::string::size_type length = std::strlen(begin);
        const char *end = begin + length;

        if(length > cvr_saver_name_length)
        {
            const char *b = std::strstr(begin, cvr_saver_name);
            if(b)
            {
                b += cvr_saver_name_length;

                while(*b == ' ')
                    ++b;

                const char *e = end - 1;
                while(e > b && *e != '>')
                    --e;

                while(e > b && *(e - 1) == ' ')
                    --e;

                if(b < e)
                {
                    begin = b;
                    end = e;
                }
            }
        }

        return std::string(begin, end);
    }
}


namespace metaxxa
{
	template <typename SomeType>
	class Type
	{
	public:
		template <template <typename...> typename TemplateType>
		using MoveTemplateTypes = detail::MoveTemplateTypes<TemplateType, SomeType>;

		template <template <typename...> typename TemplateType>
		using MoveTemplateTypesUnique = detail::MoveTemplateTypesUnique<TemplateType, SomeType>;

		template <template <typename...> typename TemplateType>
		using WrapToTemplateIfNotWrapped = typename detail::template WrapToTemplateIfNotWrapped<TemplateType, SomeType>::Result;

		static std::string get_name()
		{
			return typeid(SomeType).name();
		}

		static std::string name()
		{
			return get_name();
		}

		static std::string get_short_name()
		{
			auto type_name = name();
			
			auto template_params_begin = type_name.find_first_of("<");

			std::string::size_type namespace_end_index;
			if (template_params_begin != std::string::npos)
				namespace_end_index = type_name.substr(0, template_params_begin).find_last_of("::");
			else 
				namespace_end_index = type_name.find_last_of("::");

			if (namespace_end_index != std::string::npos)
				type_name = type_name.substr(namespace_end_index + 1);

			if constexpr (std::is_class_v<SomeType>)
				return "class " + type_name;
			else
				return type_name;
		}

		static std::string short_name()
		{
			return get_short_name();
		}

		static std::string get_name_without_templates()
		{
			auto type_name = name();

			auto template_params_begin = type_name.find_first_of("<");
			if (template_params_begin != std::string::npos)
				type_name = type_name.substr(0, template_params_begin);

			return type_name;
		}

		static std::string name_without_templates()
		{
			return get_name_without_templates();
		}

		static std::string get_short_name_without_templates()
		{
			auto type_name = name();

			auto template_params_begin = type_name.find_first_of("<");
			if (template_params_begin != std::string::npos)
				type_name = type_name.substr(0, template_params_begin);

			auto namespace_end_index = type_name.find_last_of("::");
			if (namespace_end_index != std::string::npos)
				type_name = type_name.substr(namespace_end_index + 1);

			if constexpr (std::is_class_v<SomeType>)
				return "class " + type_name;
			else
				return type_name;
		}

		static std::string short_name_without_templates()
		{
			return get_short_name_without_templates();
		}

		static std::string get_name_with_modifiers()
		{
			return detail::demangle<SomeType>();
		}

		static std::string name_with_modifiers()
		{
			return get_name_with_modifiers();
		}

		static std::type_index get_index() 
		{
			return std::type_index(typeid(SomeType));
		}

		static std::type_index index() 
		{
			return get_index();
		}

		static constexpr bool has_method_to_string()
		{
			return detail::has_method_to_string<SomeType>();
		}

		template <typename ToType>
		static constexpr bool is_convertible_to()
		{
			return std::is_convertible_v<SomeType, ToType>;
		}

		template <typename DerivedType>
		static constexpr bool is_base_of()
		{
			return std::is_base_of_v<SomeType, DerivedType>;
		}

		template <typename BaseType>
		static constexpr bool is_derived_from()
		{
			return std::is_base_of_v<BaseType, SomeType>;
		}

		template <typename FromType>
		static constexpr bool is_convertible_from()
		{
			return std::is_convertible_v<FromType, SomeType>;
		}

		static constexpr bool is_fundamental()
		{
			return std::is_fundamental_v<SomeType>;
		}

		static constexpr bool is_class()
		{
			return std::is_class_v<SomeType>;
		}

		static constexpr bool is_pod()
		{
			return std::is_pod_v<SomeType>;
		}

		static constexpr bool is_pointer()
		{
			return std::is_pointer_v<SomeType>;
		}

		static constexpr bool is_reference()
		{
			return std::is_reference_v<SomeType>;
		}

		static constexpr bool is_lvalue_reference()
		{
			return std::is_lvalue_reference_v<SomeType>;
		}

		static constexpr bool is_rvalue_reference()
		{
			return std::is_rvalue_reference_v<SomeType>;
		}

		template <typename Argument>
		static constexpr bool is_implicitly_constructible_from()
		{
			return detail::IS_IMPLICITLY_CONSTRUCTIBLE<SomeType, Argument>;
		}

		template <typename Argument>
		static constexpr bool is_explicitly_constructible_from()
		{
			return detail::IS_EXPLICITLY_CONSTRUCTIBLE<SomeType, Argument>;
		}

		template <template <typename...> typename TemplateType>
		static constexpr bool is_instantiation_of()
		{
			return detail::IS_INSTANTIATION_OF<TemplateType, SomeType>;
		}

		static constexpr bool is_template()
		{
			return detail::IS_TEMPLATE<SomeType>;
		}

		// ASSIGNMENT 
		// {
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, assign);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, plus_assign);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, subtract_assign);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, multiply_assign);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, divide_assign);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, mod_assign);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, bit_and_assign);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, bit_or_assign);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, bit_xor_assign);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, left_shift_assign);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, right_shift_assign);
		// }

		// (IN|DE)CREMENT
		// {
		___METAXXA___TYPE___DECLARE_UNARY_OPERATOR(SomeType, increment);
		___METAXXA___TYPE___DECLARE_UNARY_OPERATOR(SomeType, decrement);
		// }

		// ARITHMETIC
		// {
		___METAXXA___TYPE___DECLARE_UNARY_AND_BINARY_OPERATOR(SomeType, plus);
		___METAXXA___TYPE___DECLARE_UNARY_AND_BINARY_OPERATOR(SomeType, minus);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, divide);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, mod);
		___METAXXA___TYPE___DECLARE_UNARY_OPERATOR (SomeType, tilde);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, bit_or);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, bit_xor);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, left_shift);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, right_shift);
		// }


		// LOGICAL
		// {
		___METAXXA___TYPE___DECLARE_UNARY_OPERATOR (SomeType, not);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, logical_and);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, logical_or);
		// }

		// COMPARISON
		// {
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, equal);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, not_equal);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, less);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, greater);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, less_equal);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, greater_equal);
		// }

		// MEMBER ACCESS
		// {
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, subscript);
		___METAXXA___TYPE___DECLARE_UNARY_OPERATOR (SomeType, pointer_access);
		___METAXXA___TYPE___DECLARE_UNARY_OPERATOR (SomeType, pointer_to_member_access);
		// }

		// OTHER
		// {

		___METAXXA___TYPE___DECLARE_NAMED_UNARY_AND_BINARY_OPERATOR(SomeType, asterisk, dereference, multiply);
		___METAXXA___TYPE___DECLARE_NAMED_UNARY_AND_BINARY_OPERATOR(SomeType, ampersand, address, bit_and);
		___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, comma);
		___METAXXA___TYPE___DECLARE_UNARY_OPERATOR (SomeType, call);

		template <typename ToType>
		static constexpr bool has_operator()
		{
			return detail::___METAXXA___OPERATOR_TESTER(cast_operator)<SomeType, ToType, void>::has();
		}
		// }
	};

	template <typename FirstType, typename SecondType>
	constexpr bool operator==(const Type<FirstType> &, const Type<SecondType> &)
	{
		return std::is_same_v<FirstType, SecondType>;
	}

	template <typename FirstType, typename SecondType>
	constexpr bool operator!=(const Type<FirstType> &a, const Type<SecondType> &b)
	{
		return !(a == b);
	}
}


namespace metaxxa
{
	namespace detail
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
				"metaxxa static error:  Functor has no 'void operator()()'"
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
		detail::for_each<Tuple, Callable, 0>(tuple, callable);
	}

	template <typename Tuple, template <typename ValueType> typename Functor, typename... FunctorArguments>
	constexpr void for_each_types(FunctorArguments&... arguments)
	{
		detail::for_each_types<Tuple, Functor, 0, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
	}

	template <typename Tuple, template <typename ValueType> typename Functor, typename... FunctorArguments>
	constexpr void for_each_types(FunctorArguments&&... arguments)
	{
		detail::for_each_types<Tuple, Functor, 0, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
	}
}




namespace metaxxa
{
	namespace detail
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
				"metaxxa static error:  Functor has no non-const 'bool operator()()'"
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
		return detail::every<Tuple, Callable, 0>(tuple, callable);
	}

	template 
	<
		typename Tuple, 
		template <typename ValueType> typename Functor, 
		typename... FunctorArguments
	>
	constexpr bool every_types(FunctorArguments&&... arguments)
	{
		return detail::every_types<Tuple, Functor, 0, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
	}
}



namespace metaxxa
{
	namespace detail
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
		return detail::is_same<Tuple, 0>(tuple);
	}

	template <typename Tuple>
	constexpr bool is_same_types()
	{
		return detail::is_same_types<Tuple, 0>();
	}
}



#ifdef max
#define ___METAXXA___MAXOFSTDTUPLE_MAX_UNDEF___ max
#undef max
#endif // max

namespace metaxxa
{
	namespace detail
	{
		template <typename Tuple, size_t INDEX>
		constexpr auto max(Tuple &tuple)
		{
			if constexpr (INDEX + 2 == std::tuple_size<Tuple>::value)
				return std::max(std::get<INDEX>(tuple), std::get<INDEX + 1>(tuple));
			else
				return std::max(std::get<INDEX>(tuple), max<Tuple, INDEX + 1>(tuple));
		}
	}

	template <typename Tuple>
	constexpr auto max(Tuple &tuple)
	{
		return detail::max<Tuple, 0>(tuple);
	}
}

#ifdef ___METAXXA___MAXOFSTDTUPLE_MAX_UNDEF___
#define max ___METAXXA___MAXOFSTDTUPLE_MAX_UNDEF___
#endif // max



namespace metaxxa
{
	template <typename Tuple>
	constexpr auto &first(Tuple &tuple)
	{
		return std::get<0>(tuple);
	}

	template <typename Tuple>
	constexpr auto first_types() -> typename std::tuple_element_t<0, Tuple>;
}



namespace metaxxa
{
	template <typename Tuple>
	constexpr auto &last(Tuple &tuple)
	{
		return std::get<std::tuple_size_v<Tuple> - 1>(tuple);
	}

	template <typename Tuple>
	constexpr auto last_types() -> typename std::tuple_element_t<std::tuple_size_v<Tuple> - 1, Tuple>;
}



namespace metaxxa
{
	namespace detail
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

			static_assert
			(
				Type<typename Function<decltype(&FunctorInstance::operator())>::Result>() != Type<void>(), 
				"metaxxa static error: functor for map must return value"
			);

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
		return detail::map<Tuple, Callable, 0>(tuple, callable);
	}

	template 
	<
		typename Tuple, 
		template <typename ValueType> typename Functor,
		typename... FunctorArguments
	>
	constexpr auto map_types(FunctorArguments&&... arguments)
	{
		return detail::map_types<Tuple, Functor, 0, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
	}
}



namespace metaxxa
{
	namespace detail
	{
		template <typename Tuple, size_t INDEX>
		constexpr auto sum(Tuple &tuple)
		{
			if constexpr (INDEX + 1 == std::tuple_size<Tuple>::value)
				return std::get<INDEX>(tuple);
			else
				return std::get<INDEX>(tuple) + sum<Tuple, INDEX + 1>(tuple);
		}
	}

	template <typename Tuple>
	constexpr auto sum(Tuple &tuple)
	{
		return detail::sum<Tuple, 0>(tuple);
	}
}



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



#if __has_include(<variant>)
    #include <variant>

    #define METAXXA_VARIANT std::variant
#elif __has_include(<experimental/variant>)
    #include <experimental/variant>

    #define METAXXA_VARIANT std::experimental::variant
#else
    #error METAXXA: Sorry, your compiler does not support neither std::variant or std::experimental::variant
#endif // Check variant



namespace metaxxa
{
    namespace detail
    {
        template <typename Tuple, template <typename> typename TemplateClass, size_t INDEX>
        constexpr auto wrap_of_std_tuple(Tuple &tuple)
        {
            if constexpr (INDEX == std::tuple_size_v<Tuple>)
                return std::tuple<>();
            else
                return std::tuple_cat
                (
                    std::tuple<TemplateClass<std::tuple_element_t<INDEX, Tuple>>>(std::get<INDEX>(tuple)),
                    wrap_of_std_tuple<Tuple, TemplateClass, INDEX + 1>(tuple)
                );
        }

        template <typename Tuple, template <typename> typename TemplateClass, size_t INDEX>
        constexpr auto wrap_of_std_tuple_types()
        {
            if constexpr (INDEX == std::tuple_size_v<Tuple>)
                return std::tuple<>();
            else
                return std::tuple_cat
                (
                    std::tuple<TemplateClass<std::tuple_element_t<INDEX, Tuple>>>(std::declval<TemplateClass<std::tuple_element_t<INDEX, Tuple>>>()),
                    wrap_of_std_tuple_types<Tuple, TemplateClass, INDEX + 1>()
                );
        }
    }

    template <typename Tuple, template <typename> typename TemplateType>
    constexpr auto wrap_of_std_tuple(Tuple &tuple)
    {
        return detail::wrap_of_std_tuple<Tuple, TemplateType, 0>(tuple);
    }

    template <typename Tuple, template <typename> typename TemplateType>
    constexpr auto wrap_of_std_tuple_types()
    {
        return detail::wrap_of_std_tuple_types<Tuple, TemplateType, 0>();
    }
}



namespace metaxxa
{
	namespace detail
	{
		template <typename Tuple, typename Callable, size_t INDEX>
		auto find(Tuple &tuple, Callable &callable)
			-> METAXXA_OPTIONAL
			<
				MoveTemplateTypesUnique<METAXXA_VARIANT, decltype(::metaxxa::wrap_of_std_tuple_types<Tuple, std::remove_cv_t>())>
			>
		{
			if (callable(std::get<INDEX>(tuple)))
				return METAXXA_OPTIONAL
				<
					MoveTemplateTypesUnique<METAXXA_VARIANT, decltype(::metaxxa::wrap_of_std_tuple_types<Tuple, std::remove_cv_t>())>
				>(std::get<INDEX>(tuple));

			else if (INDEX + 1 < std::tuple_size<Tuple>::value)
				return find<Tuple, Callable, INDEX + 1>(tuple, callable);

			else
				return METAXXA_OPTIONAL
				<
					MoveTemplateTypesUnique<METAXXA_VARIANT, decltype(::metaxxa::wrap_of_std_tuple_types<Tuple, std::remove_cv_t>())>
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
				"metaxxa static error:  Functor has no 'bool operator()()'"
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




namespace metaxxa
{
	namespace functor
	{
		template <typename SomeType>
		struct TypeStringifier
		{
			TypeStringifier(std::string &result)
				: result(result)
			{
			}

			void operator()()
			{
				if (!result.empty())
					result += ", ";
				result += Type<SomeType>::name();
			}

		private:
			std::string &result;
		};
	}
}



namespace metaxxa
{
	template <typename Tuple>
	std::string to_string(Tuple &tuple)
	{
		std::string result;

		for_each(tuple, [&result](auto element)
		{
			if(!result.empty())
				result += ", ";
			result += std::to_string(element); 
		});

		return "<" + result + ">";
	}

	template <typename Tuple>
	std::string to_string(Tuple &&tuple)
	{
		std::string result;

		for_each(tuple, [&result](auto element)
		{
			if(!result.empty())
				result += ", ";
			result += std::to_string(element);
		});

		return "<" + result + ">";
	}

	template <>
	inline std::string to_string<std::tuple<>>(std::tuple<> &)
	{
		return "<>";
	}

	template <>
	inline std::string to_string<std::tuple<>>(std::tuple<> &&)
	{
		return "<>";
	}

	template <typename Tuple>
	std::string to_string_types()
	{
		std::string result;
		for_each_types<Tuple, functor::TypeStringifier>(result);

		return "<" + result + ">";
	}

	template <>
	inline std::string to_string_types<std::tuple<>>()
	{
		return "<>";
	}
}




namespace metaxxa
{
    namespace detail
    {
        template<typename Value, typename FunctionParameter, bool IS_FUNDAMENTAL = std::is_fundamental<Value>::value>
        struct IsCorrectArgument
        {};

        template<typename Value, typename FunctionParameter>
        struct IsCorrectArgument<Value, FunctionParameter, true>
                : std::integral_constant
                            <
                                    bool,
                                    std::is_same
                                        <
                                                typename std::remove_const<typename std::remove_reference<Value>::type>::type,
                                                typename std::remove_const<typename std::remove_reference<FunctionParameter>::type>::type
                                        >::value
                            >
        {};

        template<typename Value, typename FunctionParameter>
        struct IsCorrectArgument<Value, FunctionParameter, false>
                : detail::IsImplicitlyConstructible<FunctionParameter, Value>
        {};

        template <typename Values, typename Callable, bool IS_CORRECT, bool IS_ENOUGH = false, size_t INDEX = 0, size_t... INDICES>
        struct IsCorrectArgumentsProxy
                : IsCorrectArgumentsProxy
                            <
                                    Values,
                                    Callable,
                                    IsCorrectArgument
                                    <
                                    typename std::tuple_element<INDEX, Values>::type, 
                                    typename detail::Function<Callable>::template Argument<INDEX>
                                    >::value,
                                    std::tuple_size<Values>::value == INDEX + 1,
                                    sizeof...(INDICES) + 1,
                                    INDEX
                            >
        {};

        template<typename Values, typename Callable, size_t INDEX, size_t... INDICES>
        struct IsCorrectArgumentsProxy<Values, Callable, true, true, INDEX, INDICES...>
                : std::true_type
        {};

        template<typename Values, typename Callable, bool IS_ENOUGH, size_t INDEX, size_t... INDICES>
        struct IsCorrectArgumentsProxy<Values, Callable, false, IS_ENOUGH, INDEX, INDICES...>
                : std::false_type
        {};


        template <typename Values, typename Callable, size_t VALUE_COUNT = std::tuple_size<Values>::value, size_t ARGUMENT_COUNT = detail::Function<Callable>::ARGUMENT_COUNT>
        struct IsCorrectArguments
                : IsCorrectArgumentsProxy<Values, Callable, VALUE_COUNT == ARGUMENT_COUNT>
        {};

        template <typename Values, typename Callable>
        struct IsCorrectArguments<Values, Callable, 1, 0>
                : std::integral_constant<bool, std::is_same<typename std::tuple_element<0, Values>::type, void>::value>
        {};

        template <typename Values, typename SubscribersTuple, int INDEX = -1, size_t... CORRECT_INDICES>
        struct CorrectExecutorProxy
        {
            template <typename... Arguments>
            static void execute(SubscribersTuple &tuple, Arguments&&... arguments)
            {
                /*if constexpr 
                (
                    std::is_callable_v
                    <
                        typename std::tuple_element
                        <
                            INDEX, SubscribersTuple
                        >::type,
                        Arguments...
                    >
                )*/
                {
                    std::get<INDEX>(tuple)(arguments...);
                    CorrectExecutorProxy<Values, SubscribersTuple, CORRECT_INDICES...>::execute(tuple, std::forward<Arguments>(arguments)...);
                }
            }
        };

        template <typename Values, typename SubscribersTuple, int INDEX>
        struct CorrectExecutorProxy<Values, SubscribersTuple, INDEX>
        {
            template <typename... Arguments>
            static void execute(SubscribersTuple &tuple, Arguments&&... arguments)
            {
                /*if constexpr 
                (
                    std::is_callable_v
                    <
                        typename std::tuple_element
                        <
                            INDEX, SubscribersTuple
                        >::type,
                        Arguments...
                    >
                )*/
                {
                    std::get<INDEX>(tuple)(std::forward<Arguments>(arguments)...);
                }
            }
        };

        template <typename Value, typename SubscribersTuple>
        struct CorrectExecutorProxy<Value, SubscribersTuple, -1>
        {
            template <typename... Arguments>
            static void execute(SubscribersTuple &tuple, Arguments&&... arguments)
            {
            }
        };

        template <typename Values, typename SubscribersTuple, bool IS_ENOUGH = false, size_t INDEX = 0, typename... Arguments>
        struct CorrectExecutor
        {
            static void execute(SubscribersTuple &tuple, Values &values, Arguments&&... arguments)
            {
                CorrectExecutor
                        <
                                Values,
                                SubscribersTuple,
                                INDEX == std::tuple_size<Values>::value,
                                INDEX + 1,
                                typename std::tuple_element<INDEX, Values>::type
                        >::execute
                        (
                                tuple,
                                std::forward<Arguments>(arguments)...,
                                std::get<INDEX>(values)
                        );
            }
        };

        template <typename Values, typename SubscribersTuple, size_t INDEX, typename... Arguments>
        struct CorrectExecutor<Values, SubscribersTuple, true, INDEX, Arguments...>
        {
            static void execute(SubscribersTuple &tuple, Values &values, Arguments&&... arguments)
            {
                CorrectExecutor<Values, SubscribersTuple>::execute(tuple, std::forward<Arguments>(arguments)...);
            }
        };

        ////////////////VOID//////////////////////

        template <typename SubscribersTuple, int INDEX, size_t... CORRECT_INDICES>
        struct CorrectExecutorProxy<void, SubscribersTuple, INDEX, CORRECT_INDICES...>
        {
            static void execute(SubscribersTuple &tuple)
            {
                std::get<INDEX>(tuple)();
                CorrectExecutorProxy<void, SubscribersTuple, CORRECT_INDICES...>::execute(tuple);
            }
        };

        template <typename SubscribersTuple, int INDEX>
        struct CorrectExecutorProxy<void, SubscribersTuple, INDEX>
        {
            static void execute(SubscribersTuple &tuple)
            {
                std::get<INDEX>(tuple)();
            }
        };

        template <typename SubscribersTuple>
        struct CorrectExecutorProxy<void, SubscribersTuple, -1>
        {
            static void execute(SubscribersTuple &tuple)
            {
            }
        };

        //////////////////////////



        template
                <
                        typename Values,
                        typename SubscribersTuple,
                        bool IS_CORRECT_TYPE = false,
                        size_t INDEX = std::tuple_size<SubscribersTuple>::value,
                        size_t... CORRECT_INDICES
                >
        class TupleFunctionsExecutorProxy
                : public TupleFunctionsExecutorProxy
                        <
                                Values,
                                SubscribersTuple,
                                IsCorrectArguments<Values, typename std::tuple_element<INDEX - 1, SubscribersTuple>::type>::value,
                                INDEX - 1,
                                CORRECT_INDICES...
                        >
        {
        public:

        };

        template <typename Values, typename SubscribersTuple, size_t INDEX, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<Values, SubscribersTuple, true, INDEX, CORRECT_INDICES...>
                : public TupleFunctionsExecutorProxy
                        <
                                Values,
                                SubscribersTuple,
                                IsCorrectArguments<Values, typename std::tuple_element<INDEX - 1, SubscribersTuple>::type>::value,
                                INDEX - 1,
                                INDEX,
                                CORRECT_INDICES...
                        >
        {
        public:

        };

        template <typename Values, typename SubscribersTuple, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<Values, SubscribersTuple, false, 0, CORRECT_INDICES...>
        {
        public:
            template <typename... Arguments>
            static void execute(SubscribersTuple &tuple, Arguments&&... arguments)
            {
                CorrectExecutorProxy<Values, SubscribersTuple, CORRECT_INDICES...>
                ::execute(tuple, std::forward<Arguments>(arguments)...);
            }
        };

        template <typename Values, typename SubscribersTuple, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<Values, SubscribersTuple, true, 0, CORRECT_INDICES...>
        {
        public:
            template <typename... Arguments>
            static void execute(SubscribersTuple &tuple, Arguments&&... arguments)
            {
                CorrectExecutorProxy<Values, SubscribersTuple, 0, CORRECT_INDICES...>
                ::execute(tuple, std::forward<Arguments>(arguments)...);
            }
        };


        ////////////////////////////VOID////////////////////////////////


        template
                <
                        typename SubscribersTuple,
                        bool IS_CORRECT_TYPE,
                        size_t INDEX,
                        size_t... CORRECT_INDICES
                >
        class TupleFunctionsExecutorProxy<void, SubscribersTuple, IS_CORRECT_TYPE, INDEX, CORRECT_INDICES...>
                : public TupleFunctionsExecutorProxy
                        <
                                std::tuple<void>,
                                SubscribersTuple,
                                IsCorrectArguments<std::tuple<void>, typename std::tuple_element<INDEX - 1, SubscribersTuple>::type>::value,
                                INDEX - 1,
                                CORRECT_INDICES...
                        >
        {
        public:

        };

        template <typename SubscribersTuple, size_t INDEX, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<void, SubscribersTuple, true, INDEX, CORRECT_INDICES...>
                : public TupleFunctionsExecutorProxy
                        <
                                std::tuple<void>,
                                SubscribersTuple,
                                IsCorrectArguments<std::tuple<void>, typename std::tuple_element<INDEX - 1, SubscribersTuple>::type>::value,
                                INDEX - 1,
                                INDEX,
                                CORRECT_INDICES...
                        >
        {
        public:

        };

        template <typename SubscribersTuple, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<void, SubscribersTuple, false, 0, CORRECT_INDICES...>
        {
        public:
            static void execute(SubscribersTuple &tuple)
            {
                CorrectExecutorProxy<void, SubscribersTuple, CORRECT_INDICES...>
                ::execute(tuple);
            }
        };

        template <typename SubscribersTuple, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<void, SubscribersTuple, true, 0, CORRECT_INDICES...>
        {
        public:
            static void execute(SubscribersTuple &tuple)
            {
                CorrectExecutorProxy<void, SubscribersTuple, 0, CORRECT_INDICES...>
                ::execute(tuple);
            }
        };

        template <typename SubscribersTuple, typename... Values>
        struct TupleFunctionsExecutor : public detail::TupleFunctionsExecutorProxy<std::tuple<Values...>, SubscribersTuple>
        {
        };

        template <typename SubscribersTuple>
        struct TupleFunctionsExecutor<SubscribersTuple> : public detail::TupleFunctionsExecutorProxy<std::tuple<void>, SubscribersTuple>
        {
        };
    }

    template<typename SubscribersTuple, typename... Values>
    void execute_functions(SubscribersTuple &tuple, Values... values)
    {
        detail::TupleFunctionsExecutor<SubscribersTuple, Values...>::execute(tuple, values...);
    };
}



namespace metaxxa
{
	namespace detail
	{
		template <typename Tuple, typename Callable, size_t INDEX, size_t... INDICES>
		auto call_function(Tuple &tuple, Callable &callable)
		{
			if constexpr(INDEX < std::tuple_size_v<Tuple>)
				return call_function<Tuple, Callable, INDEX + 1, INDICES..., INDEX>(tuple, callable);
			else
				return callable(std::get<INDICES>(tuple)...);
		}
	}

	template <typename Tuple, typename Callable>
	auto call_function(Tuple &tuple, Callable &callable)
	{
		return detail::call_function<Tuple, Callable, 0>(tuple, callable);
	}
}




namespace metaxxa
{
    namespace detail
    {
        template <typename TupleOfOptionals, typename Tuple, typename Callable, size_t INDEX>
        constexpr auto filter(TupleOfOptionals &tuple_of_optionals, Tuple &tuple, Callable &callable)
        {
            if constexpr (INDEX == std::tuple_size_v<TupleOfOptionals>)
                return tuple_of_optionals;
            else
            {
                {
                    auto &element = std::get<INDEX>(tuple);
                    if (callable(element))
                        std::get<INDEX>(tuple_of_optionals) = element; 
                }

                return filter<TupleOfOptionals, Tuple, Callable, INDEX + 1>(tuple_of_optionals, tuple, callable);
            }
        }

        template 
        <
            typename Tuple, 
            template <typename ValueType> typename Functor,
            size_t INDEX,
            typename... FunctorArguments
        >
        constexpr auto filter_types(FunctorArguments&&... arguments)
        {
            if constexpr (INDEX + 1 == std::tuple_size_v<Tuple>)
                return std::tuple<>();
            else
            {
                using FunctorInstance = Functor<std::tuple_element_t<INDEX, Tuple>>;

                static_assert
                (
                    Type<FunctorInstance>::template has_operator_call<bool()>(),
                    "metaxxa static error:  Functor has no non-const 'bool operator()()'"
                );

                FunctorInstance checker(std::forward<FunctorArguments>(arguments)...);

                if constexpr (checker())
                    return std::declval
                    <
                        decltype
                        (
                            std::tuple_cat
                            (   
                                std::make_tuple(std::declval<std::tuple_element_t<INDEX, Tuple>>()),
                                filter_types<Tuple, Functor, INDEX + 1, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...)
                            )
                        )
                    >();
                else
                    return std::declval<decltype(filter_types<Tuple, Functor, INDEX + 1, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...))>();
            }
        }
    }

    template <typename Tuple, typename Callable>
    constexpr auto filter(Tuple &tuple, Callable &callable)
    {
        using TupleOfOptionals = decltype(wrap_of_std_tuple_types<Tuple, METAXXA_OPTIONAL>());
        TupleOfOptionals tuple_of_optionals;

        return detail::filter<TupleOfOptionals, Tuple, Callable, 0>(tuple_of_optionals, tuple, callable);
    }

    template 
    <
        typename Tuple, 
        template <typename ValueType> typename Functor,
        typename... FunctorArguments
    >
    constexpr auto filter_types(FunctorArguments&&... arguments)
    {
        return detail::filter_types<Tuple, Functor, 0, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);
    }

}



namespace metaxxa
{
	template <typename... Arguments>
	class Tuple
	{
	public:
		using StdTuple = std::tuple<Arguments...>;

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

			return detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(result)>(result);
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
			
			return detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(result)>(result);
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

			return detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);
		}

		template <size_t COUNT>
		static constexpr auto skip_first_types() ->
			detail::MoveTemplateTypes
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

			return detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);
		}

		template 
		<
			template <typename ValueType> typename Functor,
			typename... FunctorArguments
		>
		static constexpr auto map_types(FunctorArguments&&... arguments)
		{
			auto tuple = ::metaxxa::map_types<StdTuple, Functor, FunctorArguments...>(std::forward<FunctorArguments>(arguments)...);

			return detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);
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

			return detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);
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

			return detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);

		} 

		template <typename Type>
		constexpr auto contains(Type &value)
		{
			return ::metaxxa::contains(std_tuple, value);
		}

		template <typename Type>
		constexpr auto contains(Type &&value)
		{
			return ::metaxxa::contains(std_tuple, std::forward<Type>(value));
		}

		template <typename... Types>
		static constexpr bool contains_types()
		{
			return ::metaxxa::contains_types<StdTuple, Types...>();
		}

		static constexpr auto distinct_types()
		{
			return std::declval<detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(::metaxxa::distinct_types<StdTuple>())>>();
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

			return detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(tuple)>(tuple);
		}

		template <template <typename> typename TemplateType>
		static constexpr auto wrap_types()
		{
			return std::declval<detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(wrap_of_std_tuple_types<StdTuple, TemplateType>())>>();
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

			return detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(result)>(result);
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

			return detail::MoveTemplateTypes<::metaxxa::Tuple, decltype(result)>(result);
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

		static constexpr auto distinct_types() -> Tuple;

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



namespace metaxxa
{
	template<template<typename...> typename TemplateType, typename Callable>
	using MoveFunctionArgumentTypes = detail::MoveTemplateTypes<TemplateType, typename Function<Callable>::Arguments>;
}


#endif // METAXXA_H