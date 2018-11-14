#ifndef TEFRI_TEST_CARRYOPERATOR_H
#define TEFRI_TEST_CARRYOPERATOR_H

#include "TestTefri.h"

bool my_test_function(int, double, char)
{
    return true;
}

class TestCarryOperator : public TestTefri
{
public:
    virtual bool test() override
    {
        using namespace metaxxa;
        using namespace tefri;

        static_assert
        (
            Type<CarryOperator<decltype(my_test_function)>>::template is_derived_from<Operator<bool, int, double, char>>(), 
            "CarryOperator construct failed"
        );

        static_assert
        (
            !Type<CarryOperator<decltype(my_test_function)>>::template is_derived_from<Operator<bool, int, int, int>>(), 
            "CarryOperator construct failed"
        );

        return true;
    }
};

#endif // TEFRI_TEST_CARRYOPERATOR_H