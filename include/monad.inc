#ifndef TEFRI_MONAD_INC
#define TEFRI_MONAD_INC

#include "monad.h"
#include "objectholder.h"
#include "tuple.h"

namespace tefri
{
    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad() = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(const Monad &) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(Monad &&) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::~Monad() = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...> &Monad<Types, Functions...>::operator=(const Monad &) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...> &Monad<Types, Functions...>::operator=(Monad &&) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(FunctionsTuplePtr functions)
        : functions(functions)
    {}

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(FunctionsTuple &&functions)
        : functions(std::make_shared<FunctionsTuple>(std::forward<FunctionsTuple>(functions)))
    {}

    template <typename Types, typename... Functions>
    template <typename Function>
    auto Monad<Types, Functions...>::operator>>(Function &&function) &&
        -> Monad<Types, Functions..., Function>
    {
        return Monad<Types, Functions..., Function>(functions->template push_back<Function>(function));
    }

    template <typename Types, typename... Functions>
    template <typename Function>
    auto Monad<Types, Functions...>::operator>>(Function &function) &&
        -> Monad<Types, Functions..., Function>
    {
        return Monad<Types, Functions..., Function>(functions->template push_back<Function>(function));
    }

    template <typename Types, typename... Functions>
    template <typename... Args>
    auto Monad<Types, Functions...>::operator()(const Args &... args) const
    {
        return detail::MonadInvoker<Monad<Types, Functions...>, Args...>
            ::invoke(*this, args...);
    }

    template <typename Types, typename... Functions>
    template <std::size_t N>
    auto Monad<Types, Functions...>::raw_next() const
        -> RawNextMonad<N>
    {
        return RawNextMonad<N>(functions->template take_range_shared<N, decltype(functions)::element_type::size()>());
    }

    template <typename... Variants>
    auto monad() -> detail::MonadFromRawVariants<Variants...>
    {
        using ResultMonad = detail::MonadFromRawVariants<Variants...>;

        return ResultMonad
        (
            std::make_shared<typename ResultMonad::FunctionsTuple>()
        );
    }
}

#endif // TEFRI_MONAD_INC