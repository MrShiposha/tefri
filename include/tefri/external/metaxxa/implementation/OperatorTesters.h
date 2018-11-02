#ifndef METAXXA_IMPLEMENTATION_OPERATORTESTERS_H
#define METAXXA_IMPLEMENTATION_OPERATORTESTERS_H

#include "implementation/Function.h"
#include "CallableToMethod.h"

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

// TODO: MOVE IT
#define ___METAXXA___EXPAND_DEEP(...) __VA_ARGS__

#define ___METAXXA___EXPAND(...) ___METAXXA___EXPAND_DEEP(__VA_ARGS__)

#define ___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_UNARY(OPERATOR_NAME, OPERATOR) \
	___METAXXA___EXPAND(___METAXXA___DECLARE_POSTCINDITION_OPERATOR_CHECKER(OPERATOR_NAME, implementation::Function<decltype(&SomeType::OPERATOR)>::ARGUMENT_COUNT == 0))

#define ___METAXXA___DECLARE_POSTCONDITION_OPERATOR_CHECKER_FOR_BINARY(OPERATOR_NAME, OPERATOR) \
	___METAXXA___EXPAND(___METAXXA___DECLARE_POSTCINDITION_OPERATOR_CHECKER(OPERATOR_NAME, implementation::Function<decltype(&SomeType::OPERATOR)>::ARGUMENT_COUNT > 0))


namespace metaxxa::implementation
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

#endif // METAXXA_IMPLEMENTATION_OPERATORTESTERS_H