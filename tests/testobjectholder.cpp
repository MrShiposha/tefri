#ifndef TEFRI_TEST_OBJECTHOLDER_H
#define TEFRI_TEST_OBJECTHOLDER_H

#include <thread>
#include <chrono>

#include "tests.h"

struct Some
{
    Some() = default;

    Some(const Some &)
    {
        ++copies;
    }

    static size_t copies;
};

size_t Some::copies = 1;


TEST_CASE("test_get_object_ref", "[tefri::ObjectHolder]")
{
    using namespace tefri;

    int value = 42;

    {
        ObjectHolder obj(value);

        static_assert(std::is_same_v<decltype(obj.get_ref()), int&>, "invalid object ref type");
        REQUIRE(obj.get_ref() == value);
        REQUIRE(&obj.get_ref() != &value);
    }

    {
        ObjectHolder<const int> obj(value);

        static_assert(std::is_same_v<decltype(obj.get_ref()), const int&>, "invalid object ref type");
        REQUIRE(obj.get_ref() == value);
        REQUIRE(&obj.get_ref() != &value);
    }


    {
        ObjectHolder obj(std::ref(value));

        static_assert(std::is_same_v<decltype(obj.get_ref()), int&>, "invalid object ref type");
        REQUIRE(obj.get_ref() == value);
        REQUIRE(&obj.get_ref() == &value);
    }

    {
        ObjectHolder obj(std::cref(value));

        static_assert(std::is_same_v<decltype(obj.get_ref()), const int&>, "invalid object ref type");
        REQUIRE(obj.get_ref() == value);
        REQUIRE(&obj.get_ref() == &value);
    }
}

TEST_CASE("test_get_object_copy", "[tefri::ObjectHolder]")
{
    using namespace tefri;

    int value = 42;

    {
        ObjectHolder obj(value);

        static_assert(std::is_same_v<decltype(obj.get_copy()), int>, "invalid object copy type");
        REQUIRE(obj.get_copy() == value);
    }

    {
        ObjectHolder<const int> obj(value);

        static_assert(std::is_same_v<decltype(obj.get_copy()), int>, "invalid object copy type");
        REQUIRE(obj.get_copy() == value);
    }

    {
        ObjectHolder obj(std::ref(value));

        static_assert(std::is_same_v<decltype(obj.get_copy()), int>, "invalid object copy type");
        REQUIRE(obj.get_copy() == value);
    }

    {
        ObjectHolder obj(std::cref(value));

        static_assert(std::is_same_v<decltype(obj.get_copy()), int>, "invalid object copy type");
        REQUIRE(obj.get_copy() == value);
    }
}

TEST_CASE("test_copy_constructor", "[tefri::ObjectHolder]")
{
    using namespace tefri;

    {
        ObjectHolder obj((Some()));
        ObjectHolder obj_copy(obj);
        
        REQUIRE(Some::copies == 2);
    }

    {
        Some s;
        ObjectHolder obj(std::ref(s));
        [[maybe_unused]] ObjectHolder obj_copy(obj);
        
        REQUIRE(Some::copies == 2);
    }
}

TEST_CASE("test_future_constructor", "[tefri::ObjectHolder]")
{
    using namespace tefri;
    using namespace std::chrono_literals;

    ObjectHolder obj(std::async(std::launch::async, [] 
    { 
        std::this_thread::sleep_for(300ms);
        return std::make_shared<int>(42);
    }));

    REQUIRE(obj.status() == std::future_status::timeout);

    std::this_thread::sleep_for(400ms);
    REQUIRE(obj.status() == std::future_status::ready);
    REQUIRE(obj.get_ref() == 42);

}

#endif // TEFRI_TEST_OBJECTHOLDER_H