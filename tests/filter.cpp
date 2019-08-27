#include "tests.h"

TEST_CASE("[tefri::Filter]")
{
    auto m = tefri::detail::monad<Seq<int, int>>() 
        >> filter([](int a, int b) { return a > 10 && b < 10; })
        >> [](auto &&_, auto a, auto b)
        {
            REQUIRE(a.get_ref() > 10);
            REQUIRE(b.get_ref() < 10);
        };

    m(0, 11);
    m(51, 5);
    m(12, 31);
    m(nullptr, "");
}

TEST_CASE("[tefri::FilterSeq]")
{
    auto m = tefri::detail::monad<Seq<int, int>>()
        >> filter_seq([](int a) { return a > 10; })
        >> [](auto &&_, auto a, auto b)
        {
            REQUIRE(a.get_ref() > 10);
            REQUIRE(b.get_ref() > 10);
        };

    m(0, 11);
    m(51, 5);
    m(12, 31);
    m(nullptr, "");
}