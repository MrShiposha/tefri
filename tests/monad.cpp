#include "tests.h"

#include <string>
#include <iostream>

using namespace tefri;
using namespace tefri::detail;

TEST_CASE("InputSeqTuple", "[tefri::detail::MonadImpl]")
{
    using Fns = std::tuple<>;

    SECTION("Empty")
    {
        using M = MonadImpl<Fns, 0>;

        static_assert(std::is_same_v<typename M::InputSeqTuple, std::tuple<>>);
    }

    SECTION("One")
    {
        using M1 = MonadImpl<Fns, 0, Seq<int>>;
        static_assert(std::is_same_v<typename M1::InputSeqTuple, std::tuple<Seq<int>>>);

        using M2 = MonadImpl<Fns, 0, Seq<int, double>>;
        static_assert(std::is_same_v<typename M2::InputSeqTuple, std::tuple<Seq<int, double>>>);
    }

    SECTION("Two")
    {
        using M1 = MonadImpl<Fns, 0, Seq<int>, Seq<char>>;
        static_assert(std::is_same_v<typename M1::InputSeqTuple, std::tuple<Seq<int>, Seq<char>>>);

        using M2 = MonadImpl<Fns, 0, Seq<int, double>, Seq<char>>;
        static_assert(std::is_same_v<typename M2::InputSeqTuple, std::tuple<Seq<int, double>, Seq<char>>>);
    }
}

TEST_CASE("SeqTuple", "[tefri::detail::MonadImpl]")
{
    SECTION("Empty")
    {
        using Fns = std::tuple<>;
        using M = MonadImpl<Fns, 0, Seq<int>, Seq<double, char>>;

        static_assert(std::is_same_v<typename M::template SeqTuple<0>, typename M::InputSeqTuple>);
    }

    SECTION("One | Not unique")
    {
        auto m = monad<Seq<int>, Seq<double, char>>()
            >> [](auto &&next, auto&&... args)
            {
                return next(static_cast<double>((args.get_ref() + ...)));
            };

        using M = decltype(m);
        using Next = typename M::template Next<1>;
        using ExpectedSeqTuple = std::tuple<Seq<double>>;

        static_assert(std::is_same_v<typename M::template SeqTuple<0>, typename M::InputSeqTuple>);
        static_assert(std::is_same_v<typename M::template SeqTuple<1>, ExpectedSeqTuple>);

        static_assert(std::is_same_v<typename Next::template SeqTuple<0>, ExpectedSeqTuple>);
        static_assert(std::is_same_v<typename Next::template SeqTuple<-1>, typename M::InputSeqTuple>);
    }

    SECTION("One | Unique")
    {
        auto m = monad<Seq<int>, Seq<double, char>>()
            >> [](auto &&next, auto&&... args)
            {
                return next((args.get_ref() + ...));
            };

        using M = decltype(m);
        using Next = typename M::template Next<1>;
        using ExpectedSeqTuple = std::tuple<Seq<int>, Seq<double>>;

        static_assert(std::is_same_v<typename M::template SeqTuple<0>, typename M::InputSeqTuple>);
        static_assert(std::is_same_v<typename M::template SeqTuple<1>, ExpectedSeqTuple>);

        static_assert(std::is_same_v<typename Next::template SeqTuple<0>, ExpectedSeqTuple>);
        static_assert(std::is_same_v<typename Next::template SeqTuple<-1>, typename M::InputSeqTuple>);
    }

    SECTION("One | Unique | Seq")
    {
        auto m = monad<Seq<int>, Seq<double, char>>()
            >> [](auto &&next, auto&&... args)
            {
                return next(Seq { static_cast<int>(args.get_ref())... });
            };

        using M = decltype(m);
        using Next = typename M::template Next<1>;

        using ExpectedSeqTuple = std::tuple<Seq<int>, Seq<int, int>>;

        static_assert(std::is_same_v<typename M::template SeqTuple<0>, typename M::InputSeqTuple>);
        static_assert(std::is_same_v<typename M::template SeqTuple<1>, ExpectedSeqTuple>);

        static_assert(std::is_same_v<typename Next::template SeqTuple<0>, ExpectedSeqTuple>);
        static_assert(std::is_same_v<typename Next::template SeqTuple<-1>, typename M::InputSeqTuple>);
    }

    SECTION("Two | Not unique")
    {
        auto m = monad<Seq<int>, Seq<double, char>>()
            >> [](auto &&next, auto&&... args)
            {
                static std::size_t id = 0;
                return next(Seq { id++, std::forward<decltype(args)>(args)... });
            }
            >> [](auto &&next, const ObjectHolder<std::size_t> &id, auto&&... args)
            {
                std::string suffix;
                ((suffix += "(" + std::to_string(args.get_ref())), ...);
                suffix += ')';
                return next(std::to_string(id.get_ref()) + suffix);
            };

        using M = decltype(m);
        using Next_1 = typename M::template Next<1>;
        using Next_2 = typename M::template Next<2>;
        using EST_0 = typename M::InputSeqTuple;
        using EST_1 = std::tuple<Seq<std::size_t, int>, Seq<std::size_t, double, char>>;
        using EST_2 = std::tuple<Seq<std::string>>;

        static_assert(std::is_same_v<typename M::template SeqTuple<0>, EST_0>);
        static_assert(std::is_same_v<typename M::template SeqTuple<1>, EST_1>);
        static_assert(std::is_same_v<typename M::template SeqTuple<2>, EST_2>);

        static_assert(std::is_same_v<typename Next_1::template SeqTuple<-1>, EST_0>);
        static_assert(std::is_same_v<typename Next_1::template SeqTuple<0>, EST_1>);
        static_assert(std::is_same_v<typename Next_1::template SeqTuple<1>, EST_2>);

        static_assert(std::is_same_v<typename Next_2::template SeqTuple<-2>, EST_0>);
        static_assert(std::is_same_v<typename Next_2::template SeqTuple<-1>, EST_1>);
        static_assert(std::is_same_v<typename Next_2::template SeqTuple<0>, EST_2>);
    }

    SECTION("Two | Unique")
    {
        auto m = monad<Seq<int>, Seq<double, char>>()
            >> [](auto &&next, auto&&... args)
            {
                static std::size_t id = 0;
                return next(Seq { id++, std::forward<decltype(args)>(args)... });
            }
            >> [](auto &&next, const ObjectHolder<std::size_t> &id, auto&&... args)
            {
                std::string suffix;
                ((suffix += "(" + std::to_string(args.get_ref())), ...);
                suffix += ')';
                return next(std::to_string(id.get_ref()) + suffix, (&args.get_ref())...);
            };

        using M = decltype(m);
        using Next_1 = typename M::template Next<1>;
        using Next_2 = typename M::template Next<2>;
        using EST_0 = typename M::InputSeqTuple;
        using EST_1 = std::tuple<Seq<std::size_t, int>, Seq<std::size_t, double, char>>;
        using EST_2 = std::tuple<Seq<std::string, int*>, Seq<std::string, double*, char*>>;

        static_assert(std::is_same_v<typename M::template SeqTuple<0>, EST_0>);
        static_assert(std::is_same_v<typename M::template SeqTuple<1>, EST_1>);
        static_assert(std::is_same_v<typename M::template SeqTuple<2>, EST_2>);

        static_assert(std::is_same_v<typename Next_1::template SeqTuple<-1>, EST_0>);
        static_assert(std::is_same_v<typename Next_1::template SeqTuple<0>, EST_1>);
        static_assert(std::is_same_v<typename Next_1::template SeqTuple<1>, EST_2>);

        static_assert(std::is_same_v<typename Next_2::template SeqTuple<-2>, EST_0>);
        static_assert(std::is_same_v<typename Next_2::template SeqTuple<-1>, EST_1>);
        static_assert(std::is_same_v<typename Next_2::template SeqTuple<0>, EST_2>);
    }
}