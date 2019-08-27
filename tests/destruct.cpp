#include <utility>
#include <string>

struct TestStruct
{
    int a;
    double b;
    char c;

    bool operator==(const TestStruct &rhs) const
    {
        return a == rhs.a 
            && b == rhs.b
            && c == rhs.c;
    }
};

namespace std
{
    template <>
    class tuple_element<0, TestStruct>
    {
    public: 
        using type = decltype(TestStruct::a); 
    };

    template <>
    class tuple_element<1, TestStruct>
    {
    public: 
        using type = decltype(TestStruct::b); 
    };

    template <>
    class tuple_element<2, TestStruct>
    {
    public: 
        using type = decltype(TestStruct::c); 
    };

    template <>
    class tuple_size<TestStruct> : public std::integral_constant<std::size_t, 3> {};

    template <std::size_t INDEX>
    auto &get(TestStruct &&obj)
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
    auto m = tefri::detail::monad<Seq<TestStruct>, Seq<std::pair<std::size_t, std::string>>>()
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
                    TestStruct { args.get_ref()... } ==
                    TestStruct { 42, 3.14, 'c' }
                );
            return next();
        };

    m(TestStruct { 42, 3.14, 'c' });
    m(std::pair<std::size_t, std::string> { 11, "Message" });
}