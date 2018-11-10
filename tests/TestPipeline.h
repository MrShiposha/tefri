#ifndef TEST_PIPELINE_H
#define TEST_PIPELINE_H

#include "TestTefri.h"

class TestPipeline : public TestTefri
{
public:
    template <typename Result, typename... Input>
    struct FakeOperator
    {
        Result operator()(Input...)
        {}
    };

    virtual bool test()
    {
        bool result = true;
        result = result && test_input_types();
        result = result && test_output_types();

        return result;
    }

    bool test_input_types()
    {
        using namespace tefri;
        using namespace metaxxa;

        static_assert(std::is_same_v<typename Pipeline<Tuple<FakeOperator<double, int>, FakeOperator<float, char>>>::InputTuple, Tuple<int>>,  "class Pipeline: input types test failed");
        static_assert(std::is_same_v<typename Pipeline<Tuple<FakeOperator<float, char>, FakeOperator<double, int>>>::InputTuple, Tuple<char>>, "class Pipeline: input types test failed");

        return true;
    }

    bool test_output_types()
    {
        using namespace tefri;
        using namespace metaxxa;

        static_assert(std::is_same_v<typename Pipeline<Tuple<FakeOperator<double, int>, FakeOperator<float, char>>>::OutputTuple, Tuple<float>>,  "class Pipeline: output types test failed");
        static_assert(std::is_same_v<typename Pipeline<Tuple<FakeOperator<float, char>, FakeOperator<double, int>>>::OutputTuple, Tuple<double>>, "class Pipeline: output types test failed");

        return true;
    }
};

#endif // TEST_PIPELINE_H