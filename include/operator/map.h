#ifndef TEFRI_MAP_OPERTAOR_H
#define TEFRI_MAP_OPERTAOR_H

#include "mapping.h"

namespace tefri
{
    template <typename Callable>
    class Map : public Mapping<Callable>
    {
    public:
        using Mapping<Callable>::Mapping;

        template <typename Next, typename... Seq>
        auto operator()(Next &&, const Seq &...);
    };

    template <typename Callable>
    class MapSeq : public Mapping<Callable>
    {
    public:
        using Mapping<Callable>::Mapping;

        template <typename Next, typename... Seq>
        auto operator()(Next &&, const Seq &...);
    };

    template <typename Callable>
    auto map(const Callable &);

    template <typename Callable>
    auto map(Callable &);

    template <typename Callable>
    auto map(Callable &&);

    template <typename Callable>
    auto map_seq(const Callable &);

    template <typename Callable>
    auto map_seq(Callable &);

    template <typename Callable>
    auto map_seq(Callable &&);
}

#endif // TEFRI_MAP_OPERTAOR_H