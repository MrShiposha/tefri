#ifndef TEFRI_DETAIL_SEQMAPPER_H
#define TEFRI_DETAIL_SEQMAPPER_H

#include "typemappermonad.h"

namespace tefri::detail
{
    template <typename MonadImpl, std::size_t LAST_INDEX>
    struct SeqMapper;

    template
    <
        typename Functions,
        std::size_t LAST_INDEX,
        typename... Seqs
    >
    struct SeqMapper
    <
        MonadImpl<Functions, 0, Seqs...>,
        LAST_INDEX
    >
    {
        template <typename Seq>
        struct Functor;

        template <typename... Types>
        struct Functor<Seq<Types...>>
        {
            using type = decltype
            (
                MonadInvoker
                <
                    TypeMapperMonad<MonadImpl<Functions, 0, Seqs...>, LAST_INDEX>
                >
                ::template invoke<Types...>
                (
                    std::declval<TypeMapperMonad<MonadImpl<Functions, 0, Seqs...>, LAST_INDEX>>(),
                    std::declval<Types>()...
                )
            );
        };
    };
}

#endif // TEFRI_DETAIL_SEQMAPPER_H