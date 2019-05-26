#ifndef TEFRI_MONAD_H
#define TEFRI_MONAD_H

#include <memory>

#include "tuple.h"
#include "monadbase.h"

namespace tefri
{
    template <typename InputTupleVariants, typename... Functions>
    class Monad;

    namespace detail
    {
        template <typename InputTupleVariants>
        struct DraftMonad
        {
            template <typename... Functions>
            using Monad = ::tefri::Monad<InputTupleVariants, Functions...>;
        };

        template <typename Monad, typename... Args>
        struct Invoker;

        struct Unspecified {};
    }

    template <typename InputTupleVariants, typename... Functions>
    class Monad : public MonadBase<Monad<InputTupleVariants, Functions...>, InputTupleVariants>
    {
        using FunctionsTuple    = Tuple<Functions...>;
        using FunctionsTuplePtr = std::shared_ptr<FunctionsTuple>;
    public:
        using Base = MonadBase<Monad<InputTupleVariants, Functions...>, InputTupleVariants>;

        template <std::size_t N>
        using NextMonad = metaxxa::TakeRange
        <
            detail::DraftMonad<InputTupleVariants>::template Monad,
            metaxxa::TypeTuple<Functions...>,
            N, sizeof...(Functions)
        >;

        Monad(FunctionsTuplePtr functions);

        Monad(FunctionsTuple &&functions);

        template <typename Function>
        auto operator>>(Function &&) && 
            -> Monad<InputTupleVariants, Functions..., Function>;

        template <typename Function>
        auto operator>>(Function &) && 
            -> Monad<InputTupleVariants, Functions..., Function>;

        template <typename... Args>
        void operator()(const Args &... args);

        template <std::size_t N>
        auto next() -> NextMonad<N>;

    private:
        template <typename Variants>
        friend auto monad() -> Monad<Variants>;

        template <typename Monad, typename... Args>
        friend struct detail::Invoker;

        FunctionsTuplePtr functions;
    };

    template <typename InputTupleVariants = metaxxa::TypeTuple<>>
    auto monad() -> Monad<InputTupleVariants>;
}

#endif // TEFRI_MONAD_H