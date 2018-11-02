
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_SKIPFIRSTOFSTDTUPLE_H
#define METAXXA_SKIPFIRSTOFSTDTUPLE_H

#include <tuple>

namespace metaxxa
{
	namespace implementation
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
			implementation::SkipFirstTypes<INDEX, Tuple, INDEX + 1 == std::tuple_size<Tuple>::value>::skip_first_types()
		);
}

#endif // METAXXA_SKIPFIRSTOFSTDTUPLE_H