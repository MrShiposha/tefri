#ifndef TEFRI_TEST_OBJECTHOLDER_H
#define TEFRI_TEST_OBJECTHOLDER_H

#include "TestTefri.h"

class TestObjectHolder : public TestTefri
{
public:
    virtual bool test() override
    {
        bool result = true;
        result = result && test_get_object_ref();
        result = result && test_get_object_copy();

        return result;
    }

    bool test_get_object_copy()
    {
        using namespace tefri::detail;

        int value = 42;

        {
            ObjectHolder obj(value);

            static_assert(metaxxa::Type<decltype(obj.get_object_ref())>() == metaxxa::Type<int&>(), "invalid object ref type");
            TEST(obj.get_object_ref() == value, "invalid object ref value");
        }

        {
            ObjectHolder<const int> obj(value);

            static_assert(metaxxa::Type<decltype(obj.get_object_ref())>() == metaxxa::Type<const int&>(), "invalid object ref type");
            TEST(obj.get_object_ref() == value, "invalid object ref value");
        }


        {
            ObjectHolder obj(std::ref(value));

            static_assert(metaxxa::Type<decltype(obj.get_object_ref())>() == metaxxa::Type<int&>(), "invalid object ref type");
            TEST(obj.get_object_ref() == value, "invalid object ref value");
            TEST(&obj.get_object_ref() == &value, "invalid object ref address");
        }

        {
            ObjectHolder obj(std::cref(value));

            static_assert(metaxxa::Type<decltype(obj.get_object_ref())>() == metaxxa::Type<const int&>(), "invalid object ref type");
            TEST(obj.get_object_ref() == value, "invalid object ref value");
            TEST(&obj.get_object_ref() == &value, "invalid object ref address");
        }

        return true;   
    }

    bool test_get_object_ref()
    {
        using namespace tefri::detail;

       int value = 42;

        {
            ObjectHolder obj(value);

            static_assert(metaxxa::Type<decltype(obj.get_object_copy())>() == metaxxa::Type<int>(), "invalid object copy type");
            TEST(obj.get_object_copy() == value, "invalid object copy value");
        }

        {
            ObjectHolder<const int> obj(value);

            static_assert(metaxxa::Type<decltype(obj.get_object_copy())>() == metaxxa::Type<int>(), "invalid object copy type");
            TEST(obj.get_object_copy() == value, "invalid object copy value");
        }

        {
            ObjectHolder obj(std::ref(value));

            static_assert(metaxxa::Type<decltype(obj.get_object_copy())>() == metaxxa::Type<int>(), "invalid object copy type");
            TEST(obj.get_object_copy() == value, "invalid object copy value");
        }

        {
            ObjectHolder obj(std::cref(value));

            static_assert(metaxxa::Type<decltype(obj.get_object_copy())>() == metaxxa::Type<int>(), "invalid object copy type");
            TEST(obj.get_object_copy() == value, "invalid object copy value");
        }

        return true;   
    }
};

#endif // TEFRI_TEST_OBJECTHOLDER_H