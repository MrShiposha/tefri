#ifndef METAXXA_IMPLEMENTATION_TYPEHASOPERATORMETHODMACROS_H
#define METAXXA_IMPLEMENTATION_TYPEHASOPERATORMETHODMACROS_H

#define ___METAXXA___TYPE___DECLARE_UNARY_OPERATOR(SomeType, OPERATOR) \
	template <typename Callable> \
	static constexpr bool has_operator_##OPERATOR() \
	{ \
		return implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, Callable>::has(); \
	} \
	static constexpr bool has_single_operator_##OPERATOR() \
	{ \
		return implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has(); \
	} 

#define ___METAXXA___TYPE___DECLARE_BINARY_OPERATOR(SomeType, OPERATOR) \
	template <typename Callable> \
	static constexpr bool has_operator_##OPERATOR() \
	{ \
		return implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, Callable>::has(); \
	} \
	static constexpr bool has_single_operator_##OPERATOR() \
	{ \
		return implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has(); \
	} 

#define ___METAXXA___TYPE___DECLARE_UNARY_AND_BINARY_OPERATOR(SomeType, OPERATOR) \
	template <typename Callable> \
	static constexpr bool has_operator_##OPERATOR() \
	{ \
		return implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, Callable>::has(); \
	} \
	static constexpr bool has_single_operator_unary_##OPERATOR() \
	{ \
		return implementation::___METAXXA___POSTCONDITION_OPERATOR_CHECKER_unary_##OPERATOR<SomeType, implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has()>::check(); \
	} \
	static constexpr bool has_single_operator_binary_##OPERATOR() \
	{ \
		return implementation::___METAXXA___POSTCONDITION_OPERATOR_CHECKER_binary_##OPERATOR<SomeType, implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has()>::check(); \
	}

#define ___METAXXA___TYPE___DECLARE_NAMED_UNARY_AND_BINARY_OPERATOR(SomeType, OPERATOR, UNARY_NAME, BINARY_NAME) \
	template <typename Callable> \
	static constexpr bool has_operator_##UNARY_NAME() \
	{ \
		return implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, Callable>::has(); \
	} \
	static constexpr bool has_single_operator_##OPERATOR##_##UNARY_NAME() \
	{ \
		return implementation::___METAXXA___POSTCONDITION_OPERATOR_CHECKER_##UNARY_NAME<SomeType, implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has()>::check(); \
	} \
	template <typename Callable> \
	static constexpr bool has_operator_##BINARY_NAME() \
	{ \
		return implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, Callable>::has(); \
	} \
	static constexpr bool has_single_operator_##OPERATOR##_##BINARY_NAME() \
	{ \
		return implementation::___METAXXA___POSTCONDITION_OPERATOR_CHECKER_##BINARY_NAME<SomeType, implementation::___METAXXA___OPERATOR_TESTER_##OPERATOR<SomeType, void>::has()>::check(); \
	}

#endif // METAXXA_IMPLEMENTATION_TYPEHASOPERATORMETHODMACROS_H
