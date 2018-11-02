
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_JUSTOBJECT_H
#define METAXXA_JUSTOBJECT_H

#include <type_traits>

namespace metaxxa
{
	template <typename SomeType>
	using JustObject =
        std::remove_const_t
        <
            std::remove_reference_t
            <
                std::remove_const_t
                <
                    std::remove_all_extents_t
                    <
                        std::remove_const_t
                        <
                            std::remove_reference_t
                            <
                                std::remove_const_t<SomeType>
                            >
                        >
                    >
                >
            >
        >;
}

#endif // METAXXA_JUSTOBJECT_H