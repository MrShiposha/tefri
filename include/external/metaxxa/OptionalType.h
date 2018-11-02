
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_OPTIONALTYPE_H
#define METAXXA_OPTIONALTYPE_H

namespace metaxxa
{
	template <typename SomeType, bool CONDITION>
	struct OptionalType {};

	template <typename SomeType>
	struct OptionalType<SomeType, true> 
	{
		static constexpr bool FOUND = true;

		using Type = SomeType;

		template <typename Or>
		using TypeOr = Type;
	};

	template <typename SomeType>
	struct OptionalType<SomeType, false> 
	{
		static constexpr bool FOUND = false;

		template <typename Or>
		using TypeOr = Or;
	};
}

#endif // METAXXA_OPTIONALTYPE_H