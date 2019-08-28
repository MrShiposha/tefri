#ifndef TEFRI_MONAD_H
#define TEFRI_MONAD_H

#include "seq.h"

namespace tefri
{
    template <typename... Seqs>
    struct Monad : public Monad<Seqs>...
    {};

    template <typename... Types>
    struct Monad<Seq<Types...>>
    {
        virtual ~Monad() = default;

        virtual void invoke(Types&&... args) = 0;
    };
}

#endif // TEFRI_MONAD_H