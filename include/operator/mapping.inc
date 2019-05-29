#ifndef TEFRI_MAPPING_OPERATOR_INC
#define TEFRI_MAPPING_OPERATOR_INC

#include <type_traits>

#include "mapping.h"

namespace tefri
{
    template <typename Callable>
    Mapping<Callable>::Mapping() = default;

    template <typename Callable>
    Mapping<Callable>::Mapping(const Mapping &) = default;

    template <typename Callable>
    Mapping<Callable>::Mapping(Mapping &&) = default;

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
    Mapping<Callable> &Mapping<Callable>::operator=(const Mapping &) = default;

    template <typename Callable>
    Mapping<Callable> &Mapping<Callable>::operator=(Mapping &&) = default;

    template <typename Callable>
    Mapping<Callable>::~Mapping() = default;
}

#endif // TEFRI_MAPPING_OPERATOR_INC