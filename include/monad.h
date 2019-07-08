#ifndef TEFRI_MONAD_H
#define TEFRI_MONAD_H

#include <memory>

#include "tuple.h"
#include "monadbase.h"
#include "args.h"

namespace tefri
{
    template <typename Variants, typename... Functions>
    class Monad;

    namespace detail
    {
        template <typename Variants>
        struct DraftMonad
        {
            template <typename... Functions>
            using Monad = ::tefri::Monad<Variants, Functions...>;
        };

        template <typename Monad, typename... Args>
        struct Invoker;

        struct Unspecified {};

        template <typename T>
        struct MapArgs 
        {
            using type = typename metaxxa::If<metaxxa::is_instatiation_of<T, Args>()>
                ::template Then<T>
                ::template Else<Args<T>>
                ::Type;
        };

        template <typename... RawVariants>
        using MonadFromRawVariants = Monad
        <
            metaxxa::Map
            <
                Args,
                Args<RawVariants...>,
                detail::MapArgs
            >
        >;
    }

    template <typename Variants, typename... Functions>
    class Monad final : public MonadBase<Monad<Variants, Functions...>, Variants>
    {
        using FunctionsTuple    = Tuple<Functions...>;
        using FunctionsTuplePtr = std::shared_ptr<FunctionsTuple>;
    public:
        using Base = MonadBase<Monad<Variants, Functions...>, Variants>;

        template <std::size_t N>
        using NextMonad = metaxxa::TakeRange
        <
            detail::DraftMonad<Variants>::template Monad,
            metaxxa::TypeTuple<Functions...>,
            N, sizeof...(Functions)
        >;

        Monad();

        Monad(const Monad &);

        Monad(Monad &&);

        ~Monad();

        Monad &operator=(const Monad &);

        Monad &operator=(Monad &&);

        Monad(FunctionsTuplePtr functions);

        Monad(FunctionsTuple &&functions);

        template <typename Function>
        auto operator>>(Function &&) && 
            -> Monad<Variants, Functions..., Function>;

        template <typename Function>
        auto operator>>(Function &) && 
            -> Monad<Variants, Functions..., Function>;

        template <typename... Args>
        auto operator()(const Args &... args);

        template <std::size_t N>
        auto next() -> NextMonad<N>;

    private:
        template <typename... AnotherVariants>
        friend auto monad() -> detail::MonadFromRawVariants<AnotherVariants...>;

        template <typename Monad, typename... Args>
        friend struct detail::Invoker;

        FunctionsTuplePtr functions;
    };

    template <typename... Variants>
    auto monad() -> detail::MonadFromRawVariants<Variants...>;
}

#endif // TEFRI_MONAD_H