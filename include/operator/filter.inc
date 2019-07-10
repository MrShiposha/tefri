#ifndef TEFRI_FILTER_OPERATOR_INC
#define TEFRI_FILTER_OPERATOR_INC

#include "filter.h"

namespace tefri
{
    template <typename Callable>
    template <typename Next, typename... Args>
    auto Filter<Callable>::operator()(Next &&next, const Args &... args)
    {
        if constexpr(std::is_invocable_v<Callable, decltype(args.get_copy())...>)
        {
            if(std::invoke(this->callable, args.get_ref()...))
                return next(args...);
        }
    }

    template <typename Callable>
    template <typename Next, typename... Args>
    auto FilterSeq<Callable>::operator()(Next &&next, const Args &... args)
    {
        if constexpr((true && ... && std::is_invocable_v<Callable, decltype(args.get_copy())>))
        {
            if((true && ... && std::invoke(this->callable, args.get_ref())))
                return next(args...);
        }
    }

    template <typename Callable>
    auto filter(const Callable &callable)
    {
        return Filter<Callable>(callable);
    }

    template <typename Callable>
    auto filter(Callable &callable)
    {
        return Filter<Callable>(callable);
    }

    template <typename Callable>
    auto filter(Callable &&callable)
    {
        return Filter<Callable>(std::forward<Callable>(callable));
    }

    template <typename Callable>
    auto filter_seq(const Callable &callable)
    {
        return FilterSeq<Callable>(callable);
    }

    template <typename Callable>
    auto filter_seq(Callable &callable)
    {
        return FilterSeq<Callable>(callable);
    }

    template <typename Callable>
    auto filter_seq(Callable &&callable)
    {
        return FilterSeq<Callable>(std::forward<Callable>(callable));
    }
}

#endif // TEFRI_FILTER_OPERATOR_INC