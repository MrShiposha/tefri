#ifndef TEFRI_DETAIL_TYPEMAPPERMONAD_H
#define TEFRI_DETAIL_TYPEMAPPERMONAD_H

#include "../../lib/metaxxa/metaxxa.hpp"

namespace tefri::detail
{
    template <typename, std::size_t, typename...>
    struct MonadImpl;

    template <typename... Types>
    struct UnwrapTypesStruct
    {
        using type = Seq<UnwrapArg<std::decay_t<Types>>...>;
    };

    template<>
    struct UnwrapTypesStruct<>
    {
        using type = void;
    };

    template <typename... Types>
    using UnwrapTypes = typename UnwrapTypesStruct<Types...>::type;

    inline auto unwrap_type = [](auto &&, auto&&... args)
        -> UnwrapTypes<decltype(args)...>
    { throw; };

    template <typename MonadImpl, std::size_t LAST_INDEX>
    struct TypeMapperMonadStruct;

    template 
    <
        std::size_t LAST_INDEX,
        typename Functions,
        typename... Seqs
    >
    struct TypeMapperMonadStruct<MonadImpl<Functions, 0, Seqs...>, LAST_INDEX>
    {
        using FunctionsLHS = metaxxa::TakeFirst
        <
            metaxxa::TypeList,
            Functions,
            LAST_INDEX
        >;

        using WithUnwrapperFunction = metaxxa::Concat
        <
            std::tuple,
            FunctionsLHS,
            metaxxa::TemplateContainer<decltype(unwrap_type)>
        >;

        using type = MonadImpl<WithUnwrapperFunction, 0, Seqs...>;
    };

    template <typename MonadImpl, std::size_t LAST_INDEX>
    using TypeMapperMonad = typename TypeMapperMonadStruct<MonadImpl, LAST_INDEX>::type;
}

#endif // TEFRI_DETAIL_TYPEMAPPERMONAD_H