#ifndef TEFRI_DETAIL_MONADINVOKER_INC
#define TEFRI_DETAIL_MONADINVOKER_INC

#include <iostream>
#include "demangle.inc"

#include "monadinvoker.h"
#include "../objectholder.h"

namespace tefri::detail
{
    template <typename MonadImpl, std::size_t LAST_INDEX>
    template <typename... Args>
    auto MonadInvoker<MonadImpl, LAST_INDEX>::invoke(MonadImpl &&monad, HoldArg<Args>&&... args)
    {
        static constexpr bool IS_NOT_LAST = (FN_INDEX != LAST_INDEX);
        static constexpr bool IS_INVOCABLE = std::is_invocable_v<Fn, Next, HoldArg<Args>...>;
        if constexpr (IS_INVOCABLE && IS_NOT_LAST)
        {
            Fn &f = std::get<FN_INDEX>(monad.functions);
            Next &next = monad.template next<1>();

            return f(next, std::forward<HoldArg<Args>>(args)...);
        }
        else
            return /*void*/;
    }
}

#endif // TEFRI_DETAIL_MONADINVOKER_INC