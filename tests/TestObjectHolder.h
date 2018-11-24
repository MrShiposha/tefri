#ifndef TEFRI_TEST_OBJECTHOLDER_H
#define TEFRI_TEST_OBJECTHOLDER_H

#include "TestTefri.h"

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

class TestObjectHolder : public TestTefri
{
public:
    virtual bool test() override
    {
        bool result = true;
        result = result && test_get_object_ref();
        result = result && test_get_object_copy();
        result = result && test_copy_constructor();

        return result;
    }

    bool test_get_object_copy()
    {
        using namespace tefri;

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
        using namespace tefri;

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

    bool test_copy_constructor()
    {
        using namespace tefri;

        {
            ObjectHolder obj((Some()));
            ObjectHolder obj_copy(obj);
            
            TEST(Some::copies == 2, "called copy constructor of holded object");
        }

        {
            Some s;
            ObjectHolder obj(std::ref(s));
            [[maybe_unused]] ObjectHolder obj_copy(obj);
            
            TEST(Some::copies == 2, "called copy constructor of holded object");
        }

        return true;
    }
};

#endif // TEFRI_TEST_OBJECTHOLDER_H