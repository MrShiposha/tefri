#ifndef METAXXA_detail_METHODTESTER_H
#define METAXXA_detail_METHODTESTER_H

#include "../CallableToMethod.h"

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

#endif // METAXXA_detail_METHODTESTER_H