#ifndef TEFRI_DETAIL_MONADIMPL_H
#define TEFRI_DETAIL_MONADIMPL_H

#include <memory>
#include <tuple>

#include "../monad.h"
#include "monadinvoker.inc"
#include "seqmapper.h"

namespace tefri::detail
{
    template <typename T>
    struct NotVoid : std::bool_constant<!std::is_same_v<T, void>> {};

    template <typename Monad, typename MonadImpl, typename... Seqs>
    struct MonadImplBase : MonadImplBase<Monad, MonadImpl, Seqs>...
    {
        static_assert
        (
            (metaxxa::is_instatiation_of<Seqs, Seq>() && ...),
            "Invalid use of monad. Did you forget Seq<...>?"
        );

        using MonadImplBase<Monad, MonadImpl, Seqs>::invoke...;
    };

    template <typename Monad, typename MonadImpl, typename... Types>
    struct MonadImplBase<Monad, MonadImpl, Seq<Types...>> 
        : virtual public ::tefri::Monad<Seq<Types...>>
    { virtual void invoke(Types&&... args) override; };

    template <typename _Functions, std::size_t _INDEX, typename... Seqs>
    struct MonadImpl : public MonadImplBase
    <
        Monad<Seqs...>,
        MonadImpl<_Functions, _INDEX, Seqs...>, 
        Seqs...
    >
    {
        using InputSeqTuple = std::tuple<Seqs...>;

        static_assert
        (
            metaxxa::is_unique<InputSeqTuple>(),
            "Sequences must be unique"  
        );

        using Functions = _Functions;

        static constexpr std::size_t INDEX = _INDEX;

        template <std::size_t N>
        using Next = MonadImpl<Functions, INDEX + N, Seqs...>;

        template <typename Function>
        using WithFunction = MonadImpl
        <
            metaxxa::Concat<std::tuple, Functions, metaxxa::TemplateContainer<Function>>,
            INDEX,
            Seqs...
        >;

    public:
        template <long long N>
        using SeqTuple = typename
            metaxxa::Type<InputSeqTuple>
                ::template Map<SeqMapper<MonadImpl<Functions, 0, Seqs...>, INDEX + N>::template Functor>
                ::template Filter<NotVoid>
                ::Unique
                ::template MoveParameters<std::tuple>
                ::Unwrap;

        MonadImpl();

        MonadImpl(Functions &&);

        template <typename Function>
        WithFunction<Function> operator>>(Function &) &&;

        template <typename Function>
        WithFunction<Function> operator>>(const Function &) &&;

        template <typename Function>
        WithFunction<Function> operator>>(Function &&) &&;

        template <typename... Args>
        auto operator()(Args&&... args);

        template <typename... Args>
        auto operator()(Seq<Args...> &&args);

        template <std::size_t N>
        Next<N> &next();

    private:
        template <typename, std::size_t>
        friend struct MonadInvoker;

        template <typename Seq, std::size_t... INDICES>
        auto invoke_seq(Seq &&args, std::index_sequence<INDICES...>);

        Functions functions;
    };

    template <typename... Seqs>
    MonadImpl<std::tuple<>, 0, Seqs...> monad();
}

/*
    Seq<int, char>  |----\
    Seq<double>     |------> std::variant<int, char, double, A, C>
    Seq<A, int, C>  |----/
*/

#endif // TEFRI_DETAIL_MONADIMPL_H