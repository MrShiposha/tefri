#ifndef TEFRI_DETAIL_MONADINVOKER_H
#define TEFRI_DETAIL_MONADINVOKER_H

#include "../../lib/metaxxa/metaxxa.hpp"

namespace tefri::detail
{
    template <typename Functions, std::size_t INDEX, typename... Seqs>
    struct MonadImpl;

    template <typename T>
    using HoldArg 
        = typename metaxxa::If<metaxxa::is_instatiation_of<std::decay_t<T>, ObjectHolder>()>
        ::template Then<std::decay_t<T>>
        ::template Else<ObjectHolder<std::decay_t<T>>>
        ::Type;

    template <typename T>
    struct ArgUnwrapper
    {
        using type = std::decay_t<T>;
    };

    template <typename T>
    struct ArgUnwrapper<ObjectHolder<T>>
    {
        using type = std::decay_t<T>;
    };

    template <typename T>
    using UnwrapArg = typename ArgUnwrapper<T>::type;

    template 
    <
        typename MonadImpl, 
        std::size_t LAST_INDEX = std::tuple_size_v<typename MonadImpl::Functions>
    >
    struct MonadInvoker
    {
    private:
        static constexpr std::size_t FN_INDEX = MonadImpl::INDEX;
        using Fns = typename MonadImpl::Functions;
        using Fn = std::tuple_element_t<FN_INDEX, Fns>;

        using Next = typename MonadImpl::template Next<1>;

    public:
        template <typename... Args>
        static inline auto invoke(MonadImpl &&, HoldArg<Args>&&...);
    };

    template
    <
        typename Functions, 
        std::size_t INDEX,
        typename... Seqs
    >
    struct MonadInvoker<MonadImpl<Functions, INDEX, Seqs...>, INDEX>
    {
    public:
        template <typename... Args>
        static inline auto invoke(MonadImpl<Functions, INDEX, Seqs...> &&, HoldArg<Args>&&...)
        { return; }
    };
}

#endif // TEFRI_DETAIL_MONADINVOKER_H