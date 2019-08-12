#ifndef TEFRI_MONAD_H
#define TEFRI_MONAD_H

#include <memory>

#include "tuple.h"
#include "monadbase.h"

#include "detail/monad.h"

namespace tefri
{
    namespace detail
    {
        template <typename Monad, typename... Args>
        struct MonadInvoker;

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
        static_assert(!std::is_same_v<Variants, Args<void>>);
        static_assert(!std::is_same_v<Variants, void>);

        using FunctionsTuple    = Tuple<Functions...>;
        using FunctionsTuplePtr = std::shared_ptr<FunctionsTuple>;
    public:
        using Base          = MonadBase<Monad<Variants, Functions...>, Variants>;
        using InputVariants = Variants;

        template <std::size_t N>
        using NextMonadVariants = detail::NextMonadVariants<N, Variants, Functions...>;

        template <std::size_t N>
        using NextMonad = metaxxa::TakeRange
        <
            DraftMonad<detail::NextMonadVariants<N, Variants, Functions...>>::template Monad,
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
        auto operator()(const Args &... args) const;

    private:
        template <std::size_t N>
        using RawNextMonad = metaxxa::TakeRange
        <
            DraftMonad<Args<>>::template Monad,
            metaxxa::TypeTuple<Functions...>,
            N, sizeof...(Functions)
        >;

        template <std::size_t N>
        auto raw_next() const -> RawNextMonad<N>;

        template <typename... AnotherVariants>
        friend auto monad() -> detail::MonadFromRawVariants<AnotherVariants...>;

        template <typename Monad, typename... Args>
        friend struct detail::MonadInvoker;

        FunctionsTuplePtr functions;
    };

    template <typename... Variants>
    auto monad() -> detail::MonadFromRawVariants<Variants...>;
}

#endif // TEFRI_MONAD_H