#ifndef TEFRI_FILTER_OPERATOR_INC
#define TEFRI_FILTER_OPERATOR_INC

#include "filter.h"

namespace tefri
{
    template <typename Callable>
    template <typename Next, typename... Seq>
    auto Filter<Callable>::operator()(Next &&next, const Seq &... args)
    {
        if constexpr(std::is_invocable_v<Callable, decltype(unwrap(args))...>)
        {
            if(std::invoke(this->callable, unwrap_ref(args)...))
                return next(args...);
        }
    }

    template <typename Callable>
    template <typename Next, typename... Seq>
    auto FilterSeq<Callable>::operator()(Next &&next, const Seq &... args)
    {
        if constexpr((std::is_invocable_v<Callable, decltype(unwrap(args))> && ...))
        {
            if((std::invoke(this->callable, unwrap_ref(args)) && ...))
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