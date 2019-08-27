#include <utility>
#include <string>
#include <iostream>

struct TestDestruct
{
    int a;
    double b;
    char c;

    bool operator==(const TestDestruct &rhs) const
    {
        return a == rhs.a 
            && b == rhs.b
            && c == rhs.c;
    }
};

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

    template <std::size_t INDEX>
    auto &get(TestDestruct &&obj)
    {
        if constexpr (INDEX == 0)
            return obj.a;
        else if constexpr (INDEX == 1)
            return obj.b;
        else if constexpr (INDEX == 2)
            return obj.c;
    }
}

#include "tests.h"

TEST_CASE("[tefri::destruct]")
{
    auto m = tefri::detail::monad<Seq<TestDestruct>, Seq<std::pair<std::size_t, std::string>>>()
        >> destruct()
        >> [](auto &&next, auto&&... args)
        {
            if constexpr (sizeof...(args) == 2)
                REQUIRE
                (
                    std::pair<std::size_t, std::string> { args.get_ref()... } ==
                    std::pair<std::size_t, std::string> { 11, "Message" }
                );
            else if constexpr (sizeof...(args) == 3)
                REQUIRE
                (
                    TestDestruct { args.get_ref()... } ==
                    TestDestruct { 42, 3.14, 'c' }
                );
            return next();
        };

    m(TestDestruct { 42, 3.14, 'c' });
    m(std::pair<std::size_t, std::string> { 11, "Message" });
}