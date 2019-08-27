#ifndef TEFRI_SEQ_H
#define TEFRI_SEQ_H

#include <tuple>

#include "../lib/metaxxa/metaxxa.hpp"

namespace tefri
{
    template <typename... Ts>
    struct Seq : public std::tuple<Ts...>
    {
        static_assert
        (
            sizeof...(Ts) > 0, 
            "Sequence can't be empty"
        );
        
        static_assert
        (
            !metaxxa::contains<std::tuple<Ts...>, void>(),
            "Sequence can't contain void"
        );

        using std::tuple<Ts...>::tuple;
    };

    template <typename... Args>
    Seq(Args...) -> Seq<Args...>;
}

namespace std
{
    template <std::size_t INDEX, typename... Ts>
    class tuple_element<INDEX, tefri::Seq<Ts...>>
        : public tuple_element<INDEX, std::tuple<Ts...>>
    {};

    template <typename... Ts>
    class tuple_size<tefri::Seq<Ts...>> 
        : public std::tuple_size<std::tuple<Ts...>>
    {};

    template <std::size_t INDEX, typename... Ts>
    auto &get(tefri::Seq<Ts...> &sequence)
    {
        return std::get<INDEX>(static_cast<std::tuple<Ts...> &>(sequence));
    }

    template <std::size_t INDEX, typename... Ts>
    const auto &get(const tefri::Seq<Ts...> &sequence)
    {
        return std::get<INDEX>(static_cast<const std::tuple<Ts...> &>(sequence));
    }
}

#endif // TEFRI_SEQ_H