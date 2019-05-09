#include "tests.h"

using namespace tefri;

TEST_CASE("Tuple creation/getting", "[tefri::Tuple]")
{
    SECTION("Move types... constructor")
    {
        Tuple<std::vector, int, double> tuple(1, 3.14);

        REQUIRE(tuple.template get<0>() == 1);
        REQUIRE(tuple.template get<1>() == Approx(3.14));
    }

    SECTION("Ref types... constructor")
    {
        int i = 42;
        char c = 'z';

        Tuple<std::vector, int, char> tuple(i, c);
        REQUIRE(tuple.template get<0>() == i);
        REQUIRE(tuple.template get<1>() == c);
    }

    SECTION("Ptr types... constructor")
    {
        int *i = new int(42);
        char *c = new char('z');

        Tuple<std::vector, int, char> tuple(i, c);
        REQUIRE(tuple.template get<0>() == *i);
        REQUIRE(tuple.template get<1>() == *c);
        REQUIRE(tuple.template get_ptr<0>().get() == i);
        REQUIRE(tuple.template get_ptr<1>().get() == c);
        REQUIRE(tuple.get_ptr(0).get() == static_cast<void*>(i));
        REQUIRE(tuple.get_ptr(1).get() == static_cast<void*>(c));
    }

    SECTION("Shared ptr types... constructor")
    {
        auto i = std::make_shared<int>(42);
        auto c = std::make_shared<char>('z');

        Tuple<std::vector, int, char> tuple(i, c);
        REQUIRE(tuple.template get<0>() == *i);
        REQUIRE(tuple.template get<1>() == *c);
        REQUIRE(tuple.template get_ptr<0>() == i);
        REQUIRE(tuple.template get_ptr<1>() == c);
        REQUIRE(tuple.get_ptr(0) == std::static_pointer_cast<void>(i));
        REQUIRE(tuple.get_ptr(1) == std::static_pointer_cast<void>(c));
    }
}

TEST_CASE("Test types", "[tefri::Tuple]")
{
    using T = Tuple<std::vector, int, double>;

    static_assert(std::is_same_v<std::tuple_element_t<0, T>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<1, T>, double>);
    static_assert(std::tuple_size_v<T> == 2);

    T tuple(1, 3.14);

    REQUIRE(std::get<0>(tuple) == 1);
    REQUIRE(std::get<1>(tuple) == Approx(3.14));
}

TEST_CASE("Test share", "[tefri::Tuple]")
{
    Tuple<std::vector, int, char> tuple(42, 'z');
    auto shared = tuple.share();

    REQUIRE(tuple.raw_objects() == shared.raw_objects());
}