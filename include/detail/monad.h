#ifndef TEFRI_DETAIL_INVOKER_H
#define TEFRI_DETAIL_INVOKER_H

#include "../args.h"

namespace tefri
{   
    template <typename Variants, typename... Functions>
    class Monad;

    template <typename Variants>
    struct DraftMonad
    {
        template <typename... Functions>
        using Monad = ::tefri::Monad<Variants, Functions...>;
    };

    namespace detail
    {
        template <typename T>
        using HoldType = ObjectHolder
        <
            typename metaxxa::If<metaxxa::is_array_of<T, char>()>
                ::template Then<std::string>
                ::template Else<T>
                ::Type
        >;

        template <typename Holder>
        using UnwrapHolder = typename Holder::Object;

        inline auto type_getter = [](auto &&next_monad, const auto &... arg_holders)
            -> Args<UnwrapHolder<std::remove_cv_t<std::remove_reference_t<decltype(arg_holders)>>>...>
        { throw; };

        template <std::size_t N, typename Variants, typename... Functions>
        using TypeGetterMonad = metaxxa::MoveParameters
        <
            DraftMonad<Variants>::template Monad,
            metaxxa::Concat
            <
                metaxxa::TypeList,
                metaxxa::TakeFirst<metaxxa::TypeList, metaxxa::TypeList<Functions...>, N>,
                metaxxa::TemplateContainer<decltype(type_getter)>
            >
        >;

        template <typename Monad, typename... Args>
        struct Invoker
        {
        public:
            static auto invoke(const Monad &monad, const Args &... args)
            {
                auto hold = [](const auto &arg)
                { 
                    using Arg = std::remove_cv_t<std::remove_reference_t<decltype(arg)>>;

                    if constexpr (metaxxa::is_instatiation_of<Arg, ObjectHolder>())
                        return arg;
                    else
                        return HoldType<Arg> {arg}; 
                };

                using FunctionsType = typename Monad::FunctionsTuple;

                if constexpr 
                (
                    std::is_invocable_v
                    <
                        std::tuple_element_t<0, FunctionsType>,
                        decltype(monad.template raw_next<1>()),
                        decltype(hold(args))...
                    >
                ) return std::invoke(monad.functions->template get<0>(), monad.template raw_next<1>(), hold(args)...);
                else return;
            }
        };

        template <typename InputTupleVariants, typename... Args>
        struct Invoker<Monad<InputTupleVariants>, Args...>
        {
        public:
            static void invoke(const Monad<InputTupleVariants> &, const Args &...)
            {}
        };

        template <typename TypeGetterMonad>
        struct MonadVariantMapper
        {
            using Variants = typename TypeGetterMonad::InputVariants;

            template <typename Variant>
            struct VariantMapper
            {
                template <typename... Args>
                struct Mapper
                {
                    using type = decltype(Invoker<TypeGetterMonad, Args...>::invoke(std::declval<TypeGetterMonad>(), std::declval<Args>()...));
                };

                using type = typename metaxxa::MoveParameters<Mapper, Variant>::type;
            };

            template <typename Arg>
            struct NotVoid : public std::integral_constant<bool, !std::is_same_v<Arg, void>>
            {};

            using type = metaxxa::Filter
            < 
                Args,
                metaxxa::Map<Args, Variants, VariantMapper>,
                NotVoid
            >;
        };

        template <std::size_t N, typename Variants, typename... Functions>
        using NextMonadVariants = typename MonadVariantMapper
        <
            TypeGetterMonad<N, Variants, Functions...>
        >::type;
    }
}

#endif // TEFRI_DETAIL_INVOKER_H