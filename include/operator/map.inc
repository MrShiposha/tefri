#ifndef TEFRI_MAP_OPERATOR_INC
#define TEFRI_MAP_OPERATOR_INC

#include <functional>

#include "map.h"

namespace tefri
{
    template <typename Callable>
    template <typename Next, typename... Args>
    void Map<Callable>::operator()(Next &&next, const Args &... args)
    {
        if constexpr(std::is_invocable_v<Callable, decltype(args.get_copy())...>)
            next(std::invoke(this->callable, args.get_ref()...));
    }

    template <typename Callable>
    template <typename Next, typename... Args>
    void MapSeq<Callable>::operator()(Next &&next, const Args &... args)
    {
        if constexpr((true && ... && std::is_invocable_v<Callable, decltype(args.get_copy())>))
            next(std::invoke(this->callable, args.get_ref())...);
    }

    template <typename Callable>
    auto map(const Callable &callable)
    {
        return Map<Callable>(callable);
    }

    template <typename Callable>
    auto map(Callable &callable)
    {
        return Map<Callable>(callable);
    }

    template <typename Callable>
    auto map(Callable &&callable)
    {
        return Map<Callable>(std::forward<Callable>(callable));
    }

    template <typename Callable>
    auto map_seq(const Callable &callable)
    {
        return MapSeq<Callable>(callable);
    }

    template <typename Callable>
    auto map_seq(Callable &callable)
    {
        return MapSeq<Callable>(callable);
    }

    template <typename Callable>
    auto map_seq(Callable &&callable)
    {
        return MapSeq<Callable>(std::forward<Callable>(callable));
    }
}

#endif // TEFRI_MAP_OPERATOR_INC