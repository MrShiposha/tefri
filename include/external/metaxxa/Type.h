
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_TYPE_H
#define METAXXA_TYPE_H

#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include "implementation/Function.h"
#include "implementation/OperatorTesters.h"
#include "implementation/TypeHasOperatorMethodMacros.h"
#include "implementation/HasMethodToString.h"
#include "implementation/TupleTag.h"
#include "implementation/Demangle.h"

namespace metaxxa
{
	template <typename SomeType>
	class Type
	{
	public:
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
			return implementation::demangle<SomeType>();
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
			return implementation::has_method_to_string<SomeType>();
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

		static constexpr bool is_tuple()
		{
			return std::is_base_of_v<implementation::TupleTag, SomeType>;
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
			return implementation::___METAXXA___OPERATOR_TESTER(cast_operator)<SomeType, ToType, void>::has();
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

#endif // METAXXA_TYPE_H