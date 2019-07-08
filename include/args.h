#ifndef TEFRI_ARGS_H
#define TEFRI_ARGS_H

#include "def.h"

namespace tefri
{
    template <typename... Ts>
    struct Args
    {
        using type = metaxxa::TypeList<Ts...>;
    };
}

namespace std
{
    template <std::size_t INDEX, typename... Ts>
    class tuple_element<INDEX, tefri::Args<Ts...>>
    {
    public:
        using type = std::tuple_element_t<INDEX, typename tefri::Args<Ts...>::type>;
    };

    template <typename... Ts>
    class tuple_size<tefri::Args<Ts...>> 
        : public std::integral_constant<std::size_t, std::tuple_size_v<typename tefri::Args<Ts...>::type>>
    {};
}

#endif // TEFRI_ARGS_H