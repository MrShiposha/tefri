#ifndef TEFRI_SEQ_H
#define TEFRI_SEQ_H

#include "def.h"

namespace tefri
{
    template <typename... Ts>
    struct Seq
    {
        using type = metaxxa::TypeList<Ts...>;
    };
}

namespace std
{
    template <std::size_t INDEX, typename... Ts>
    class tuple_element<INDEX, tefri::Seq<Ts...>>
    {
    public:
        using type = std::tuple_element_t<INDEX, typename tefri::Seq<Ts...>::type>;
    };

    template <typename... Ts>
    class tuple_size<tefri::Seq<Ts...>> 
        : public std::integral_constant<std::size_t, std::tuple_size_v<typename tefri::Seq<Ts...>::type>>
    {};
}

#endif // TEFRI_SEQ_H