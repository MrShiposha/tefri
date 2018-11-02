
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_JUSTOBJECTSOFSTDTUPLE_H
#define METAXXA_JUSTOBJECTSOFSTDTUPLE_H

#include <tuple>

#include "JustObject.h"

namespace metaxxa
{
	namespace implementation
	{
		template <typename Tuple, size_t INDEX>
		auto just_objects(Tuple &tuple)
		{
			if constexpr(INDEX + 1 >= std::tuple_size_v<Tuple>)
				return std::make_tuple
				(
					static_cast<JustObject<std::tuple_element_t<INDEX, Tuple>>>(std::get<INDEX>(tuple))
				);
			else
				return std::tuple_cat
				(
					std::make_tuple
					(
						static_cast<JustObject<std::tuple_element_t<INDEX, Tuple>>>(std::get<INDEX>(tuple))
					),
					just_objects<Tuple, INDEX + 1>(tuple)
				);
		}
        
        template <typename Tuple, size_t INDEX, bool ENOUGH, typename... Parameters>
        struct JustObjects
            : public JustObjects
                    <
                        Tuple,
                        INDEX + 1,
                        INDEX + 1 >= std::tuple_size_v<Tuple>,
                        Parameters..., JustObject<std::tuple_element_t<INDEX, Tuple>>
                    >
        {};
        
        template <typename Tuple, size_t INDEX, typename... Parameters>
        struct JustObjects<Tuple, INDEX, true, Parameters...>
        {
            using Result = std::tuple<Parameters...>;
        };
	}

	template <typename Tuple>
	auto just_objects(Tuple &tuple)
	{
		return implementation::just_objects<Tuple, 0>(tuple);
	}

	template <typename Tuple>
    auto just_objects_types() -> typename implementation::JustObjects<Tuple, 0, std::tuple_size_v<Tuple> == 0>::Result;
}

#endif // METAXXA_JUSTOBJECTSOFSTDTUPLE_H