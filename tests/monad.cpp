#include "tests.h"

#include <string>

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
            next("return(" + std::to_string(i * 10) + ")");
        }
        >> [&](auto &&next, const auto &m_hld)
        {
            auto &m = m_hld.get_ref();

            REQUIRE(m == "return(420)");

            f2_called = true;
            next();
        };

        m(42, 'z');

        REQUIRE(f1_called);
        REQUIRE(f2_called);
    }
}