#include "tests.h"

#include <string>
#include <iostream>

TEST_CASE("[tefri::Monad]")
{
    SECTION("Empty")
    {
        auto m = monad();

        m(42, 'z');
    }

    SECTION("With functions")
    {
        bool f1_called = false;
        bool f2_called = false;

        auto m = monad() >> [&](auto &&next, const auto &i_hld, const auto &c_hld) 
        { 
            auto &i = i_hld.get_ref();
            auto &c = c_hld.get_ref();

            REQUIRE(i == 42);
            REQUIRE(c == 'z');

            f1_called = true;
            return next("return(" + std::to_string(i * 10) + ")");
        }
        >> [&](auto &&next, const auto &m_hld)
        {
            auto &m = m_hld.get_ref();

            REQUIRE(m == "return(420)");

            f2_called = true;
            return next();
        };

        m(42, 'z');

        REQUIRE(f1_called);
        REQUIRE(f2_called);
    }
}

TEST_CASE("Test MonadFromRawVariants, [tefri::Monad]")
{
    using M = tefri::detail::MonadFromRawVariants<Seq<int, char>, char, double>;
    static_assert(is_same_v<M, Monad<Seq<Seq<int, char>, Seq<char>, Seq<double>>>>);
}

auto f1 = [](auto &&next, const auto &a)
{
    return next(std::to_string(a.get_ref()));
};

auto f2 = [](auto &&next, const auto &str_hld)
{
    return next(str_hld.get_ref().c_str());
};

auto g1 = [](auto &&next, const auto &a_hld, const auto &b_hld)
{
    return next(a_hld.get_ref() * 0.01 + b_hld.get_ref());
};

auto g2 = [](auto &&next, const auto &a_hld)
{
    double a = a_hld.get_copy();
    int int_part = static_cast<int>(a);
    double frac_part = a - int_part;

    return next(std::to_string(int_part), std::to_string(frac_part));
};

TEST_CASE("Test NextMonadVariants", "[tefri::Monad]")
{
    SECTION("One arg")
    {
        auto m = monad<int>() >> f1 >> f2;

        using M = decltype(m);
        using Args0 = typename M::template NextMonadVariants<0>;
        using Args1 = typename M::template NextMonadVariants<1>;
        using Args2 = typename M::template NextMonadVariants<2>;

        static_assert(is_same_v<Args0, Seq<Seq<int>>>);
        static_assert(is_same_v<Args1, Seq<Seq<std::string>>>);
        static_assert(is_same_v<Args2, Seq<Seq<const char *>>>);
    }

    SECTION("Two args")
    {
        auto m = monad<int, char>() >> f1 >> f2;

        using M = decltype(m);
        using Args0 = typename M::template NextMonadVariants<0>;
        using Args1 = typename M::template NextMonadVariants<1>;
        using Args2 = typename M::template NextMonadVariants<2>;

        static_assert(is_same_v<Args0, Seq<Seq<int>, Seq<char>>>);
        static_assert(is_same_v<Args1, Seq<Seq<std::string>>>);
        static_assert(is_same_v<Args2, Seq<Seq<const char *>>>);
    }

    SECTION("Nested args")
    {
        auto m = monad<int, Seq<double, double>, char>() >> g1 >> g2;

        using M = decltype(m);
        using Args0 = typename M::template NextMonadVariants<0>;
        using Args1 = typename M::template NextMonadVariants<1>;
        using Args2 = typename M::template NextMonadVariants<2>;

        static_assert(is_same_v<Args0, Seq<Seq<int>, Seq<double, double>, Seq<char>>>);
        static_assert(is_same_v<Args1, Seq<Seq<double>>>);
        static_assert(is_same_v<Args2, Seq<Seq<std::string, std::string>>>);
    }
}

TEST_CASE("Test NextMonad", "[tefri::Monad]")
{
    using F_1 = decltype(f1);
    using F_2 = decltype(f2);

    SECTION("One arg")
    {
        auto m = monad<int>() >> f1 >> f2;

        using M = decltype(m);

        using M_0 = typename M::template NextMonad<0>;
        using M_1 = typename M::template NextMonad<1>;
        using M_2 = typename M::template NextMonad<2>;

        static_assert(is_same_v<M_0, M>);
        static_assert(is_same_v<M_0, Monad<Seq<Seq<int>>, F_1, F_2>>);
        static_assert(is_same_v<M_1, Monad<Seq<Seq<std::string>>, F_2>>);
        static_assert(is_same_v<M_2, Monad<Seq<Seq<const char *>>>>);
    }

    SECTION("Two args")
    {
        auto m = monad<int, char>() >> f1 >> f2;

        using M = decltype(m);

        using M_0 = typename M::template NextMonad<0>;
        using M_1 = typename M::template NextMonad<1>;
        using M_2 = typename M::template NextMonad<2>;

        static_assert(is_same_v<M_0, M>);
        static_assert(is_same_v<M_0, Monad<Seq<Seq<int>, Seq<char>>, F_1, F_2>>);
        static_assert(is_same_v<M_1, Monad<Seq<Seq<std::string>>, F_2>>);
        static_assert(is_same_v<M_2, Monad<Seq<Seq<const char *>>>>);
    }

    SECTION("Nested args")
    {
        auto m = monad<int, Seq<double, double>, char>() >> g1 >> g2;

        using G_1 = decltype(g1);
        using G_2 = decltype(g2);

        using M = decltype(m);

        using M_0 = typename M::template NextMonad<0>;
        using M_1 = typename M::template NextMonad<1>;
        using M_2 = typename M::template NextMonad<2>;

        static_assert(is_same_v<M_0, M>);
        static_assert(is_same_v<M_0, Monad<Seq<Seq<int>, Seq<double, double>, Seq<char>>, G_1, G_2>>);
        static_assert(is_same_v<M_1, Monad<Seq<Seq<double>>, G_2>>);
        static_assert(is_same_v<M_2, Monad<Seq<Seq<std::string, std::string>>>>);
    }
}