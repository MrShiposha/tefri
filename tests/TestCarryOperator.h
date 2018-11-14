#ifndef TEFRI_TEST_CARRYOPERATOR_H
#define TEFRI_TEST_CARRYOPERATOR_H

#include "TestTefri.h"

bool my_test_function(int, double, char)
{
    return true;
}

template <typename Result, typename... Args>
class MyTestCarryOperator : public tefri::CarryOperator<Result(Args...), Result, Args...>
{
public:
    using Base = tefri::CarryOperator<Result(Args...), Result, Args...>;

    using typename Base::ArgumentsTuple;
    using typename Base::Result;
    using typename Base::OptionalResult;

    MyTestCarryOperator(std::function<Result(Args...)> callable)
    : Base(callable)
    {}

    virtual OptionalResult operator()(const Args&... args) override
    {
        return {};
    }
};

class TestCarryOperator : public TestTefri
{
public:
    virtual bool test() override
    {
        using namespace metaxxa;
        using namespace tefri;

        using OperatorInstance = typename decltype(make_carry_operator<MyTestCarryOperator>(my_test_function))::element_type;

        static_assert
        (
            Type<OperatorInstance>::template is_derived_from<Operator<bool, int, double, char>>(), 
            "CarryOperator construct failed"
        );

        static_assert
        (
            !Type<OperatorInstance>::template is_derived_from<Operator<bool, int, int, int>>(), 
            "CarryOperator construct failed"
        );

        return true;
    }
};

#endif // TEFRI_TEST_CARRYOPERATOR_H