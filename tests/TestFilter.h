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

        TEST(!(*filter_test)(3, 'b'), "filter did not reject invalid value");
        TEST(!(*filter_test)(11, 'a'), "filter did not reject invalid value");

        auto valid = (*filter_test)(42, '1'); 
        TEST(valid, "filter rejected invalid value");
        if(valid)
            TEST(valid.value() == metaxxa::Tuple(42, '1'), "filter returned invalid value");

        return true;
    }
};

#endif // TEST_TEFRI_FILTER_H