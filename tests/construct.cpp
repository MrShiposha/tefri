#include "tests.h"

struct TestConstruct
{
    int a, b, c;
};

TEST_CASE("[tefri::construct]")
{
    SECTION("Not template")
    {
        auto m = tefri::detail::monad<Seq<int, int, int>>()
            >> construct<TestConstruct>()
            >> [](auto &&next, ObjectHolder<TestConstruct> &&test_hld)
            {
                TestConstruct &t = test_hld.get_ref();
                REQUIRE(t.a == 1);
                REQUIRE(t.b == 2);
                REQUIRE(t.c == 3);
            };

        m(1, 2, 3);
    }

    SECTION("Template")
    {
        auto m = tefri::detail::monad<Seq<int, int, int>>()
            >> construct<std::tuple>()
            >> [](auto &&next, auto &&tuple_hld)
            {
                auto &t = tuple_hld.get_ref();
                REQUIRE(std::get<0>(t) == 1);
                REQUIRE(std::get<1>(t) == 2);
                REQUIRE(std::get<2>(t) == 3);
            };

        m(1, 2, 3);
    }
}