#include "tests.h"

TEST_CASE("[tefri::Map]")
{
    auto m = monad() 
        >> map([](int a, int b, int c) { return a + b + c; })
        >> [](auto &&_, const auto &sum_hld)
        {
            const int &sum = sum_hld.get_ref();

            REQUIRE(sum == 100);
        };

    m(10, 20, 70);
}

TEST_CASE("[tefri::MapSeq]")
{
    auto m = monad() 
        >> map_seq([](auto v) { return v * 10; })
        >> [](auto &&_, auto a, auto b, auto c, auto d)
        {
            REQUIRE(a.get_ref() == 100);
            REQUIRE(b.get_ref() == 200);
            REQUIRE(c.get_ref() == 700);
            REQUIRE(d.get_ref() == 40);
        };

    m(10, 20, 70, 4);
}