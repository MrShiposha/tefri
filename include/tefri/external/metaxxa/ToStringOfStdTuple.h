
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_TOSTRINGOFSTDTUPLE_H
#define METAXXA_TOSTRINGOFSTDTUPLE_H

#include <tuple>
#include <string>

#include "functor/TypeStringifier.h"
#include "ForEachOfStdTuple.h"

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
			result += metaxxa::to_string(element); 
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
			result += metaxxa::to_string(element);
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

#endif // METAXXA_TOSTRINGOFSTDTUPLE_H