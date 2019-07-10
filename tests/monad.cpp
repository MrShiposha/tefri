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

TEST_CASE("Test MonadFromRawVariants, [metaxxa::Monad]")
{
    using M = tefri::detail::MonadFromRawVariants<Args<int, char>, char, double>;
    static_assert(is_same_v<M, Monad<Args<Args<int, char>, Args<char>, Args<double>>>>);
}


TEST_CASE("Test NextMonadVariants", "[metaxxa::Monad]")
{
    auto f1 = [](auto &&next, const auto &a)
    {
        return next(std::to_string(a.get_ref()));
    };

    auto f2 = [](auto &&next, const auto &str_hld)
    {
        return next(str_hld.get_ref().c_str());
    };

    auto m = monad<Args<int>>() >> f1 >> f2;

    using M = decltype(m);
    using Args0 = typename M::template NextMonadVariants<0>;
    using Args1 = typename M::template NextMonadVariants<1>;
    using Args2 = typename M::template NextMonadVariants<2>;

    static_assert(is_same_v<Args0, Args<Args<int>>>);
    static_assert(is_same_v<Args1, Args<Args<std::string>>>);
    static_assert(is_same_v<Args2, Args<Args<const char *>>>);
}