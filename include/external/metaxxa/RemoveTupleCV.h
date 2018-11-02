
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_REMOVETUPLECV_H
#define METAXXA_REMOVETUPLECV_H

#include <type_traits>

namespace metaxxa
{
	namespace implementation
	{
		template <typename... Types>
		using RemoveTupleCV = std::tuple<std::remove_cv_t<Types>...>;

		template <typename... Types>
		constexpr auto remove_tuple_cv(const std::tuple<Types...> &)
			->RemoveTupleCV<Types...>;
	}

	template <typename Tuple>
	using RemoveTupleCV = decltype(implementation::remove_tuple_cv(std::declval<Tuple>()));
}

#endif // METAXXA_REMOVETUPLECV_H