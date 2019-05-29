#ifndef TEFRI_MONAD_INC
#define TEFRI_MONAD_INC

#include "monad.h"
#include "objectholder.h"
#include "tuple.h"

namespace tefri
{
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

        template <typename Monad, typename... Args>
        struct Invoker
        {
        public:
            static void invoke(Monad &monad, const HoldType<Args> &... args)
            {
                using FunctionsType = typename Monad::FunctionsTuple;

                if constexpr 
                (
                    std::is_invocable_v
                    <
                        std::tuple_element_t<0, FunctionsType>,
                        decltype(monad.template next<1>()),
                        HoldType<Args>...
                    >
                ) monad.functions->template get<0>()(monad.template next<1>(), args...);
            }

            static void invoke(Monad &monad, const Args &... args)
            {
                auto hold = [](const auto &arg)
                { 
                    using Arg = std::remove_cv_t<std::remove_reference_t<decltype(arg)>>;

                    return HoldType<Arg> {arg}; 
                };

                invoke(monad, hold(args)...);
            }
        };

        template <typename InputTupleVariants, typename... Args>
        struct Invoker<Monad<InputTupleVariants>, Args...>
        {
        public:
            static void invoke(Monad<InputTupleVariants> &, const Args &...)
            {}

            static void invoke(Monad<InputTupleVariants> &monad, const HoldType<Args> &... args)
            {}
        };

        template <typename InputTupleVariants>
        struct Invoker<Monad<InputTupleVariants>>
        {
        public:
            static void invoke(Monad<InputTupleVariants> &)
            {}
        };
    }

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad() = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(const Monad &) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(Monad &&) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::~Monad() = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...> &Monad<Types, Functions...>::operator=(const Monad &) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...> &Monad<Types, Functions...>::operator=(Monad &&) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(FunctionsTuplePtr functions)
        : functions(functions)
    {}

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(FunctionsTuple &&functions)
        : functions(std::make_shared<FunctionsTuple>(std::forward<FunctionsTuple>(functions)))
    {}

    template <typename Types, typename... Functions>
    template <typename Function>
    auto Monad<Types, Functions...>::operator>>(Function &&function) &&
        -> Monad<Types, Functions..., Function>
    {
        return Monad<Types, Functions..., Function>(functions->template push_back<Function>(function));
    }

    template <typename Types, typename... Functions>
    template <typename Function>
    auto Monad<Types, Functions...>::operator>>(Function &function) &&
        -> Monad<Types, Functions..., Function>
    {
        return Monad<Types, Functions..., Function>(functions->template push_back<Function>(function));
    }

    template <typename Types, typename... Functions>
    template <typename... Args>
    void Monad<Types, Functions...>::operator()(const Args &... args)
    {
        detail::Invoker<Monad<Types, Functions...>, Args...>
            ::invoke(*this, args...);
    }

    template <typename Types, typename... Functions>
    template <typename... Args>
    void Monad<Types, Functions...>::operator()(const ObjectHolder<Args> &... args)
    {
        detail::Invoker<Monad<Types, Functions...>, Args...>
            ::invoke(*this, args...);
    }

    template <typename Types, typename... Functions>
    void Monad<Types, Functions...>::operator()()
    {
        detail::Invoker<Monad<Types, Functions...>>
            ::invoke(*this);
    }

    template <typename Types, typename... Functions>
    template <std::size_t N>
    auto Monad<Types, Functions...>::next() 
        -> NextMonad<N>
    {
        return NextMonad<N>(functions->template take_range_shared<N, decltype(functions)::element_type::size()>());
    }

    template <typename InputTupleVariants>
    auto monad() -> Monad<InputTupleVariants>
    {
        return Monad<InputTupleVariants>
        (
            std::make_shared<typename Monad<InputTupleVariants>::FunctionsTuple>()
        );
    }
}

#endif // TEFRI_MONAD_INC