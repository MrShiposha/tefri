#include "tests.h"

#include <iostream>

using namespace tefri;

TEST_CASE("GeneralTuple creation/getting", "[tefri::GeneralTuple]")
{
    SECTION("Move types... constructor")
    {
        GeneralTuple<std::vector, int, double> tuple(1, 3.14);

        REQUIRE(tuple.template get<0>() == 1);
        REQUIRE(tuple.template get<1>() == Approx(3.14));
    }

    SECTION("Ref types... constructor")
    {
        int i = 42;
        char c = 'z';

        GeneralTuple<std::vector, int, char> tuple(i, c);
        REQUIRE(tuple.template get<0>() == i);
        REQUIRE(tuple.template get<1>() == c);
    }

    SECTION("Ptr types... constructor")
    {
        int *i = new int(42);
        char *c = new char('z');

        GeneralTuple<std::vector, int, char> tuple(i, c);
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

        GeneralTuple<std::vector, int, char> tuple(i, c);
        REQUIRE(tuple.template get<0>() == *i);
        REQUIRE(tuple.template get<1>() == *c);
        REQUIRE(tuple.template get_ptr<0>() == i);
        REQUIRE(tuple.template get_ptr<1>() == c);
        REQUIRE(tuple.get_ptr(0) == std::static_pointer_cast<void>(i));
        REQUIRE(tuple.get_ptr(1) == std::static_pointer_cast<void>(c));
    }
}

TEST_CASE("Test types", "[tefri::GeneralTuple]")
{
    using T = GeneralTuple<std::vector, int, double>;

    static_assert(std::is_same_v<std::tuple_element_t<0, T>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<1, T>, double>);
    static_assert(std::tuple_size_v<T> == 2);

    T tuple(1, 3.14);

    REQUIRE(std::get<0>(tuple) == 1);
    REQUIRE(std::get<1>(tuple) == Approx(3.14));
}

TEST_CASE("Test share", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, char> tuple(42, 'z');
    auto shared = tuple.share();

    REQUIRE(tuple.raw_objects() == shared.raw_objects());
}

TEST_CASE("Add element", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector> t0;

    auto t1 = t0.push_back(144);
    auto t2 = t1.push_back(3.14);
    auto t3 = t2.push_back('z');

    REQUIRE(t0.raw_objects() == t1.raw_objects());
    REQUIRE(t1.raw_objects() == t2.raw_objects());
    REQUIRE(t3.raw_objects() == t3.raw_objects());

    REQUIRE(t3.template get<0>() == 144);
    REQUIRE(t3.template get<1>() == Approx(3.14));
    REQUIRE(t3.template get<2>() == 'z');
}

TEST_CASE("Take range", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, double, char, std::string> tuple(42, 3.14, 'z', "Hello");

    auto middle = tuple.template take_range<1, 3>();

    static_assert(std::is_same_v<decltype(middle), GeneralTuple<std::vector, double, char>>);
    REQUIRE(middle.template get<0>() == Approx(3.14));
    REQUIRE(middle.template get<1>() == 'z');
    REQUIRE(middle.raw_objects() != tuple.raw_objects());
}

TEST_CASE("Take range shared", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, double, char, std::string> tuple(42, 3.14, 'z', "Hello");

    auto middle = tuple.template take_range_shared<1, 3>();

    static_assert(std::is_same_v<decltype(middle), GeneralTuple<std::vector, double, char>>);
    REQUIRE(middle.template get<0>() == Approx(3.14));
    REQUIRE(middle.template get<1>() == 'z');
    REQUIRE(middle.raw_objects() == tuple.raw_objects());
}

TEST_CASE("Take first", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, double, char, std::string> tuple(42, 3.14, 'z', "Hello");

    auto first = tuple.template take_first<2>();

    static_assert(std::is_same_v<decltype(first), GeneralTuple<std::vector, int, double>>);
    REQUIRE(first.template get<0>() == 42);
    REQUIRE(first.template get<1>() == Approx(3.14));
    REQUIRE(first.raw_objects() != tuple.raw_objects());
}

TEST_CASE("Take first shared", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, double, char, std::string> tuple(42, 3.14, 'z', "Hello");

    auto first = tuple.template take_first_shared<2>();

    static_assert(std::is_same_v<decltype(first), GeneralTuple<std::vector, int, double>>);
    REQUIRE(first.template get<0>() == 42);
    REQUIRE(first.template get<1>() == Approx(3.14));
    REQUIRE(first.raw_objects() == tuple.raw_objects());
}

TEST_CASE("Take last", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, double, char, std::string> tuple(42, 3.14, 'z', "Hello");

    auto last = tuple.template take_last<2>();

    static_assert(std::is_same_v<decltype(last), GeneralTuple<std::vector, char, std::string>>);
    REQUIRE(last.template get<0>() == 'z');
    REQUIRE(last.template get<1>() == "Hello");
    REQUIRE(last.raw_objects() != tuple.raw_objects());
}

TEST_CASE("Take last shared", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, double, char, std::string> tuple(42, 3.14, 'z', "Hello");

    auto last = tuple.template take_last_shared<2>();

    static_assert(std::is_same_v<decltype(last), GeneralTuple<std::vector, char, std::string>>);
    REQUIRE(last.template get<0>() == 'z');
    REQUIRE(last.template get<1>() == "Hello");
    REQUIRE(last.raw_objects() == tuple.raw_objects());
}

TEST_CASE("Skip first", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, double, char, std::string> tuple(42, 3.14, 'z', "Hello");

    auto last = tuple.template skip_first<2>();

    static_assert(std::is_same_v<decltype(last), GeneralTuple<std::vector, char, std::string>>);
    REQUIRE(last.template get<0>() == 'z');
    REQUIRE(last.template get<1>() == "Hello");
    REQUIRE(last.raw_objects() != tuple.raw_objects());
}

TEST_CASE("Skip first shared", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, double, char, std::string> tuple(42, 3.14, 'z', "Hello");

    auto last = tuple.template skip_first_shared<2>();

    static_assert(std::is_same_v<decltype(last), GeneralTuple<std::vector, char, std::string>>);
    REQUIRE(last.template get<0>() == 'z');
    REQUIRE(last.template get<1>() == "Hello");
    REQUIRE(last.raw_objects() == tuple.raw_objects());
}

TEST_CASE("Skip last", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, double, char, std::string> tuple(42, 3.14, 'z', "Hello");

    auto first = tuple.template skip_last<2>();

    static_assert(std::is_same_v<decltype(first), GeneralTuple<std::vector, int, double>>);
    REQUIRE(first.template get<0>() == 42);
    REQUIRE(first.template get<1>() == Approx(3.14));
    REQUIRE(first.raw_objects() != tuple.raw_objects());
}

TEST_CASE("Skip last shared", "[tefri::GeneralTuple]")
{
    GeneralTuple<std::vector, int, double, char, std::string> tuple(42, 3.14, 'z', "Hello");

    auto first = tuple.template skip_last_shared<2>();

    static_assert(std::is_same_v<decltype(first), GeneralTuple<std::vector, int, double>>);
    REQUIRE(first.template get<0>() == 42);
    REQUIRE(first.template get<1>() == Approx(3.14));
    REQUIRE(first.raw_objects() == tuple.raw_objects());
}