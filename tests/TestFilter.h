#ifndef TEST_TEFRI_FILTER_H
#define TEST_TEFRI_FILTER_H

#include "TestTefri.h"

class TestFilter : public TestTefri
{
public:
    virtual bool test() override
    {
        using namespace tefri;
        auto filter_test = filter([](int value, char c) { return value > 10 && c != 'a'; });

        int invalid_int = 2;
        int valid_int   = 42;
        char invalid_char = 'a';
        char valid_char   = '1';

        TEST(!(*filter_test)(invalid_int, valid_char), "filter did not reject invalid value");
        TEST(!(*filter_test)(valid_int, invalid_char), "filter did not reject invalid value");

        auto valid = (*filter_test)(valid_int, valid_char); 
        TEST(valid, "filter rejected invalid value");
        if(valid)
            TEST(valid.value() == metaxxa::Tuple(valid_int, valid_char), "filter returned invalid value");

        return true;
    }
};

#endif // TEST_TEFRI_FILTER_H