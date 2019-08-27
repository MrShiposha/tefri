#ifndef TEFRI_OPERATOR_DESTRUCT_INC
#define TEFRI_OPERATOR_DESTRUCT_INC

#include <tuple>

#include "detail/unwrap.inc"

namespace tefri
{
    namespace detail
    {
        template <typename Struct, typename Next, std::size_t... INDICES>
        inline auto destruct_impl_indices(Struct &&obj, Next &&next, std::index_sequence<INDICES...>)
        {
            return next
            (
                std::forward<std::decay_t<std::tuple_element_t<INDICES, Struct>>>
                (
                    std::get<INDICES>(std::forward<Struct>(obj))
                )...
            );
        };
    }

    inline auto destruct() 
    { 
        return [](auto &&next, auto&&... args)
        {
            static_assert
            (
                sizeof...(args) == 1,
                "Can't destruct sequence"
            );

            return [](auto &&next, auto &&arg)
            {
                using Arg = std::decay_t<decltype(detail::unwrap(arg))>;

                return detail::destruct_impl_indices<Arg>
                (
                    std::forward<Arg>(arg.get_ref()),
                    std::forward<std::decay_t<decltype(next)>>(next),
                    std::make_index_sequence<std::tuple_size_v<Arg>>()
                );
            }
            (
                std::forward<std::decay_t<decltype(next)>>(next),
                std::forward<std::decay_t<decltype(args)>>(args)...
            );
        };
    }
}

#endif // TEFRI_OPERATOR_DESTRUCT_INC