#ifndef TEFRI_FILTER_OPERATOR_H
#define TEFRI_FILTER_OPERATOR_H

#include "mapping.h"

namespace tefri
{
    template <typename Callable>
    class Filter : public Mapping<Callable>
    {
    public:
        using Mapping<Callable>::Mapping;

        template <typename Next, typename... Args>
        auto operator()(Next &&, const Args &...);
    };

    template <typename Callable>
    class FilterSeq : public Mapping<Callable>
    {
    public:
        using Mapping<Callable>::Mapping;

        template <typename Next, typename... Args>
        auto operator()(Next &&, const Args &...);
    };

    template <typename Callable>
    auto filter(const Callable &);

    template <typename Callable>
    auto filter(Callable &);

    template <typename Callable>
    auto filter(Callable &&);

    template <typename Callable>
    auto filter_seq(const Callable &);

    template <typename Callable>
    auto filter_seq(Callable &);

    template <typename Callable>
    auto filter_seq(Callable &&);
}

#endif // TEFRI_FILTER_OPERATOR_H