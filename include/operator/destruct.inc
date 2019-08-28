#ifndef TEFRI_OPERATOR_DESTRUCT_INC
#define TEFRI_OPERATOR_DESTRUCT_INC

#include <tuple>

#include "detail/unwrap.inc"

#define _T_() std::tuple_element_t<INDEX, U>

#define TEFRI_DETAIL_DESTRUCT_CHECK_GET_DEEP(R, modifier) \
    template <typename U, std::size_t INDEX>              \
    static Present is_present(Check<decltype(static_cast<R (U::*)() modifier>(&U::template get<INDEX>))> *)

#define TEFRI_DETAIL_DESTRUCT_CHECK_GET(modifier)                                                         \
    TEFRI_DETAIL_DESTRUCT_CHECK_GET_DEEP(_T_(), modifier);                                                \
    TEFRI_DETAIL_DESTRUCT_CHECK_GET_DEEP(std::add_const<_T_()>, modifier);                                \
    TEFRI_DETAIL_DESTRUCT_CHECK_GET_DEEP(std::add_lvalue_reference_t<_T_()>, modifier);                   \
    TEFRI_DETAIL_DESTRUCT_CHECK_GET_DEEP(std::add_lvalue_reference_t<std::add_const_t<_T_()>>, modifier); \
    TEFRI_DETAIL_DESTRUCT_CHECK_GET_DEEP(std::add_rvalue_reference_t<_T_()>, modifier)

#define TEFRI_DETAIL_GET_MEMBER_CHECKER(i, i_1, ...)                           \
    template <typename T>                                                      \
    struct GetMemberChecker##i                                                 \
    {                                                                          \
        template <typename U> struct Check;                                    \
        using Present    = uint8_t;                                            \
        using NotPresent = uint16_t;                                           \
                                                                               \
        __VA_ARGS__;                                                           \
                                                                               \
        template <typename U, std::size_t INDEX>                               \
        static auto is_present(...)                                            \
        {                                                                      \
            return GetMemberChecker##i_1<T>::template is_present<U, INDEX>(0); \
        }                                                                      \
                                                                               \
        template <std::size_t INDEX>                                           \
        static constexpr bool check()                                          \
        {                                                                      \
            return sizeof(is_present<T, INDEX>(0)) == sizeof(Present);         \
        }                                                                      \
    }


namespace tefri
{
    namespace detail
    {
        template <typename T>
        struct GetMemberChecker24
        {
            using NotPresent = uint16_t;
            
            template <typename U, std::size_t INDEX>
            static NotPresent is_present(...);
        };

        TEFRI_DETAIL_GET_MEMBER_CHECKER(23, 24, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const volatile && noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(22, 23, TEFRI_DETAIL_DESTRUCT_CHECK_GET(volatile && noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(21, 22, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const && noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(20, 21, TEFRI_DETAIL_DESTRUCT_CHECK_GET(&& noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(19, 20, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const volatile & noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(18, 19, TEFRI_DETAIL_DESTRUCT_CHECK_GET(volatile & noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(17, 18, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const & noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(16, 17, TEFRI_DETAIL_DESTRUCT_CHECK_GET(& noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(15, 16, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const volatile noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(14, 15, TEFRI_DETAIL_DESTRUCT_CHECK_GET(volatile noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(13, 14, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(12, 13, TEFRI_DETAIL_DESTRUCT_CHECK_GET(noexcept));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(11, 12, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const volatile &&));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(10, 11, TEFRI_DETAIL_DESTRUCT_CHECK_GET(volatile &&));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(9, 10, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const &&));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(8, 9, TEFRI_DETAIL_DESTRUCT_CHECK_GET(&&));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(7, 8, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const volatile &));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(6, 7, TEFRI_DETAIL_DESTRUCT_CHECK_GET(volatile &));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(5, 6, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const &));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(4, 5, TEFRI_DETAIL_DESTRUCT_CHECK_GET(&));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(3, 4, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const volatile));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(2, 3, TEFRI_DETAIL_DESTRUCT_CHECK_GET(volatile));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(1, 2, TEFRI_DETAIL_DESTRUCT_CHECK_GET(const));
        TEFRI_DETAIL_GET_MEMBER_CHECKER(0, 1, TEFRI_DETAIL_DESTRUCT_CHECK_GET());

#undef _T_

        template <typename T>
        struct DestructGetter
        {
            template <std::size_t INDEX, bool HAS_GET_MEMBER>
            struct GetImpl;

            template <std::size_t INDEX>
            struct GetImpl<INDEX, true>
            {
                static inline auto get(T &&obj)
                {
                    return obj.template get<INDEX>();
                }
            };

            template <std::size_t INDEX>
            struct GetImpl<INDEX, false>
            {
                static inline auto get(T &&obj)
                {
                    return std::get<INDEX>(std::forward<T>(obj));
                }
            };

            template <std::size_t INDEX>
            static inline auto get(T &&obj)
            {
                return GetImpl<INDEX, GetMemberChecker0<T>::template check<INDEX>()>
                    ::get(std::forward<T>(obj));
            }
        };

        template <typename Struct, typename Next, std::size_t... INDICES>
        inline auto destruct_impl_indices(Struct &&obj, Next &&next, std::index_sequence<INDICES...>)
        {
            return next
            (
                std::forward<std::decay_t<std::tuple_element_t<INDICES, Struct>>>
                (
                    DestructGetter<Struct>::template get<INDICES>(std::forward<Struct>(obj))
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