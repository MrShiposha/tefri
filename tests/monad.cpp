#include "tests.h"

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

        auto m = monad() >> [&](const auto &args, auto next) 
        { 
            auto &[i_hld, c_hld] = args;
            auto &i = i_hld.get_ref();
            auto &c = c_hld.get_ref();

            REQUIRE(i == 42);
            REQUIRE(c == 'z');

            f1_called = true;
            next("mod(i)", i * 10);
        }
        >> [&](const auto &args, auto next)
        {
            auto &[m_hld, i_hld] = args;
            auto &m = m_hld.get_ref();
            auto &i = i_hld.get_ref();

            REQUIRE(m == "mod(i)");
            REQUIRE(i == 420);

            f2_called = true;
            next();
        };

        m(42, 'z');

        REQUIRE(f1_called);
        REQUIRE(f2_called);
    }
}