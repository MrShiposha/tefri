
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_ISEXPLICITLYCONSTRUCTIBLE_H
#define METAXXA_ISEXPLICITLYCONSTRUCTIBLE_H

#include <type_traits>

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

#endif // METAXXA_ISEXPLICITLYCONSTRUCTIBLE_H