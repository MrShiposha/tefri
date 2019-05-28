#include "tests.h"

TEST_CASE("[tefri::Mapping]")
{
    Mapping mapping([](int a, int b, int c)
    {
        return a + b + c;
    });

    auto m = monad() >> mapping >> [](int sum) { REQUIRE(sum == 100); };

    m(10, 20, 70);
}