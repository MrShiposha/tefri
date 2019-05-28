#ifndef TEFRI_MAPPING_OPERATOR_INC
#define TEFRI_MAPPING_OPERATOR_INC

#include <type_traits>
#include <functional>

#include "mapping.h"

namespace tefri
{
    template <typename Callable>
    Mapping<Callable>::Mapping(const Callable &callable)
    : callable(callable)
    {}

    template <typename Callable>
    Mapping<Callable>::Mapping(Callable &callable)
    : callable(callable)
    {}

    template <typename Callable>
    Mapping<Callable>::Mapping(Callable &&callable)
    : callable(std::forward<Callable>(callable))
    {}

    template <typename Callable>
    template <typename Args, typename Next>
    void Mapping<Callable>::operator()(const Args &args, Next &next)
    {
        args.apply([&](const auto&... holders)
        {
            if constexpr(std::is_invocable_v<Callable, decltype(holders.get_copy())...>)
                next(std::invoke(callable, holders.get_ref()...));
        });
    }
}

#endif // TEFRI_MAPPING_OPERATOR_INC