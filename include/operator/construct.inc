#ifndef TEFRI_OPERATOR_CONSTRUCT_INC
#define TEFRI_OPERATOR_CONSTRUCT_INC

#include "detail/unwrap.h"

namespace tefri
{
    template <typename T>
    inline auto construct()
    {
        return [](auto &&next, auto&&... args)
        {
            return T { std::forward<std::decay_t<decltype(detail::unwrap(args))>>(args.get_ref())... };
        };
    }

    template <template <typename...> typename Template>
    inline auto construct()
    {
        return [](auto &&next, auto&&... args)
        {
            return Template
            <
                std::decay_t<decltype(detail::unwrap(args))>...
            > { std::forward<std::decay_t<decltype(detail::unwrap(args))>>(args.get_ref())... };
        };
    }
}

#endif // TEFRI_OPERATOR_CONSTRUCT_INC