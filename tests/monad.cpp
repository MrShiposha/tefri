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
    using M = tefri::detail::MonadFromRawVariants<Args<int, char>, char, double>;
    static_assert(is_same_v<M, Monad<Args<Args<int, char>, Args<char>, Args<double>>>>);
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

        static_assert(is_same_v<Args0, Args<Args<int>>>);
        static_assert(is_same_v<Args1, Args<Args<std::string>>>);
        static_assert(is_same_v<Args2, Args<Args<const char *>>>);
    }

    SECTION("Two args")
    {
        auto m = monad<int, char>() >> f1 >> f2;

        using M = decltype(m);
        using Args0 = typename M::template NextMonadVariants<0>;
        using Args1 = typename M::template NextMonadVariants<1>;
        using Args2 = typename M::template NextMonadVariants<2>;

        static_assert(is_same_v<Args0, Args<Args<int>, Args<char>>>);
        static_assert(is_same_v<Args1, Args<Args<std::string>>>);
        static_assert(is_same_v<Args2, Args<Args<const char *>>>);
    }

    SECTION("Nested args")
    {
        auto m = monad<int, Args<double, double>, char>() >> g1 >> g2;

        using M = decltype(m);
        using Args0 = typename M::template NextMonadVariants<0>;
        using Args1 = typename M::template NextMonadVariants<1>;
        using Args2 = typename M::template NextMonadVariants<2>;

        static_assert(is_same_v<Args0, Args<Args<int>, Args<double, double>, Args<char>>>);
        static_assert(is_same_v<Args1, Args<Args<double>>>);
        static_assert(is_same_v<Args2, Args<Args<std::string, std::string>>>);
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
        static_assert(is_same_v<M_0, Monad<Args<Args<int>>, F_1, F_2>>);
        static_assert(is_same_v<M_1, Monad<Args<Args<std::string>>, F_2>>);
        static_assert(is_same_v<M_2, Monad<Args<Args<const char *>>>>);
    }

    SECTION("Two args")
    {
        auto m = monad<int, char>() >> f1 >> f2;

        using M = decltype(m);

        using M_0 = typename M::template NextMonad<0>;
        using M_1 = typename M::template NextMonad<1>;
        using M_2 = typename M::template NextMonad<2>;

        static_assert(is_same_v<M_0, M>);
        static_assert(is_same_v<M_0, Monad<Args<Args<int>, Args<char>>, F_1, F_2>>);
        static_assert(is_same_v<M_1, Monad<Args<Args<std::string>>, F_2>>);
        static_assert(is_same_v<M_2, Monad<Args<Args<const char *>>>>);
    }

    SECTION("Nested args")
    {
        auto m = monad<int, Args<double, double>, char>() >> g1 >> g2;

        using G_1 = decltype(g1);
        using G_2 = decltype(g2);

        using M = decltype(m);

        using M_0 = typename M::template NextMonad<0>;
        using M_1 = typename M::template NextMonad<1>;
        using M_2 = typename M::template NextMonad<2>;

        static_assert(is_same_v<M_0, M>);
        static_assert(is_same_v<M_0, Monad<Args<Args<int>, Args<double, double>, Args<char>>, G_1, G_2>>);
        static_assert(is_same_v<M_1, Monad<Args<Args<double>>, G_2>>);
        static_assert(is_same_v<M_2, Monad<Args<Args<std::string, std::string>>>>);
    }
}