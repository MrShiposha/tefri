#ifndef TEFRI_DETAIL_MONADIMPL_INC
#define TEFRI_DETAIL_MONADIMPL_INC

#include "monadimpl.h"

namespace tefri::detail
{
    template <typename Tuple, std::size_t... INDICES>
    auto rvalue_tuple_impl(Tuple &tuple, std::index_sequence<INDICES...>)
    {
        return std::tuple
        <
            std::add_rvalue_reference_t
            <
                std::tuple_element_t<INDICES, Tuple>
            >...
        >(std::move(std::get<INDICES>(tuple))...);
    }

    template <typename... Args>
    auto rvalue_tuple(std::tuple<Args...> &tuple)
        -> std::tuple<std::add_rvalue_reference_t<Args>...>
    {
        return rvalue_tuple_impl(tuple, std::make_index_sequence<sizeof...(Args)>());
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    MonadImpl<Fns, I, Seqs...>::MonadImpl()
    {}

    template <typename Fns, std::size_t I, typename... Seqs>
    MonadImpl<Fns, I, Seqs...>::MonadImpl(Functions &&fns)
        : functions(std::forward<Functions>(fns))
    {}

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename Function>
    typename MonadImpl<Fns, I, Seqs...>::template WithFunction<Function> 
    MonadImpl<Fns, I, Seqs...>::operator>>(Function &fn) &&
    {
        return (*this >> std::forward<Function>(fn));
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename Function>
    typename MonadImpl<Fns, I, Seqs...>::template WithFunction<Function> 
    MonadImpl<Fns, I, Seqs...>::operator>>(const Function &fn) &&
    {
        return (*this >> std::forward<Function>(fn));
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename Function>
    typename MonadImpl<Fns, I, Seqs...>::template WithFunction<Function> 
    MonadImpl<Fns, I, Seqs...>::operator>>(Function &&fn) &&
    {
        return WithFunction<Function>
        (
            std::tuple_cat(rvalue_tuple(functions), std::tuple<Function&&>(std::forward<Function>(fn)))
        );
    }

    template <typename Monad, typename MonadImpl, typename... Types>
    void MonadImplBase<Monad, MonadImpl, Seq<Types...>>::invoke(Types&&... args)
    {
        (*static_cast<MonadImpl*>(this))(std::forward<Types>(args)...);
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename... Args>
    auto MonadImpl<Fns, I, Seqs...>::operator()(Args&&... args)
    {
        return MonadInvoker<MonadImpl<Fns, I, Seqs...>>::template invoke<Args...>
        (
            std::forward<MonadImpl<Fns, I, Seqs...>>(*this),
            std::forward<Args>(args)...
        );
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename... Args>
    auto MonadImpl<Fns, I, Seqs...>::operator()(Seq<Args...> &&args)
    {
        return invoke_seq(args, std::make_index_sequence<sizeof...(Args)>());
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename Seq, std::size_t... INDICES>
    auto MonadImpl<Fns, I, Seqs...>::invoke_seq(Seq &&args, std::index_sequence<INDICES...>)
    {
        return (*this)
        (
            std::forward
            <
                std::tuple_element_t<INDICES, std::decay_t<Seq>>
            >(std::get<INDICES>(args))...
        );
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <std::size_t N>
    typename MonadImpl<Fns, I, Seqs...>::template Next<N> &
    MonadImpl<Fns, I, Seqs...>::next()
    {
        return *reinterpret_cast<Next<N>*>(this);
    }

    template <typename... Seqs>
    MonadImpl<std::tuple<>, 0, Seqs...> monad()
    {
        return MonadImpl<std::tuple<>, 0, Seqs...>();
    }
}

#endif // TEFRI_DETAIL_MONADIMPL_INC