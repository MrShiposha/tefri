#include <utility>
#include <string>
#include <iostream>

#include "tests.h"

struct TestDestruct
{
    int a;
    double b;
    char c;

    template <std::size_t INDEX>
    auto &get()
    {
        if constexpr (INDEX == 0)
            return a;
        else if constexpr (INDEX == 1)
            return b;
        else if constexpr (INDEX == 2)
            return c;
    }

    template <std::size_t INDEX>
    const auto &get() const
    {
        return const_cast<TestDestruct*>(this)->template get<INDEX>();
    }

    bool operator==(const TestDestruct &rhs) const
    {
        return a == rhs.a 
            && b == rhs.b
            && c == rhs.c;
    }
};

struct TestEmptyStruct
{};

namespace std
{
    template <>
    class tuple_element<0, TestDestruct>
    {
    public: 
        using type = decltype(TestDestruct::a); 
    };

    template <>
    class tuple_element<1, TestDestruct>
    {
    public: 
        using type = decltype(TestDestruct::b); 
    };

    template <>
    class tuple_element<2, TestDestruct>
    {
    public: 
        using type = decltype(TestDestruct::c); 
    };

    template <>
    class tuple_size<TestDestruct> : public std::integral_constant<std::size_t, 3> {};

    template <>
    class tuple_size<TestEmptyStruct> : public std::integral_constant<std::size_t, 0> {};
}

TEST_CASE("[tefri::destruct]")
{
    bool _0_called = false;
    bool _2_called = false;
    bool _3_called = false;

    auto m = tefri::detail::monad
    <
        Seq<TestDestruct>, 
        Seq<TestEmptyStruct>,
        Seq<std::pair<std::size_t, std::string>>
    >() >> destruct()
        >> [&](auto &&next, auto&&... args)
        {
            if constexpr (sizeof...(args) == 0)
            {
                _0_called = true;
            }
            if constexpr (sizeof...(args) == 2)
            {
                REQUIRE
                (
                    std::pair<std::size_t, std::string> { args.get_ref()... } ==
                    std::pair<std::size_t, std::string> { 11, "Message" }
                );

                _2_called = true;
            }
            else if constexpr (sizeof...(args) == 3)
            {
                REQUIRE
                (
                    TestDestruct { args.get_ref()... } ==
                    TestDestruct { 42, 3.14, 'c' }
                );

                _3_called = true;
            }
            return next();
        };

    m(TestDestruct { 42, 3.14, 'c' });
    m(std::pair<std::size_t, std::string> { 11, "Message" });
    m(TestEmptyStruct {});

    REQUIRE(_0_called);
    REQUIRE(_2_called);
    REQUIRE(_3_called);
}