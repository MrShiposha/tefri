#ifndef TEST_PIPELINE_H
#define TEST_PIPELINE_H

#include "TestTefri.h"

class TestPipeline : public TestTefri
{
public:
    template <typename Result, typename... Input>
    struct FakeOperator : public tefri::Operator<Result, Input...>
    {
        using typename tefri::Operator<Result, Input...>::OptionalResult;

        virtual OptionalResult operator()(const Input&...) override
        {
            return {};
        }
    };

    virtual bool test()
    {
        bool result = true;
        result = result && test_operators_tuple();
        result = result && test_input_types();
        result = result && test_output_types();

        return result;
    }

    bool test_operators_tuple()
    {
        using namespace tefri;
        using namespace metaxxa;
        using PipelineInstance = decltype(Pipeline(metaxxa::tuple(make_operator<FakeOperator<double, int>>(), make_operator<FakeOperator<float, char>>()))); 

        static_assert(Type<typename PipelineInstance::OperatorsTuple>() == Type<Tuple<FakeOperator<double, int>, FakeOperator<float, char>>>(), "invalid operators tuple");
        return true;
    }

    bool test_input_types()
    {
        using namespace tefri;
        using namespace metaxxa;

        auto pipeline_1 = Pipeline(metaxxa::tuple(make_operator<FakeOperator<double, int>>(), make_operator<FakeOperator<float, char>>()));
        auto pipeline_2 = Pipeline(metaxxa::tuple(make_operator<FakeOperator<float, char>>(), make_operator<FakeOperator<double, int>>()));


        static_assert(std::is_same_v<decltype(pipeline_1)::InputTuple, Tuple<const int &>>,  "class Pipeline: input types test failed");
        static_assert(std::is_same_v<decltype(pipeline_2)::InputTuple, Tuple<const char &>>, "class Pipeline: input types test failed");

        return true;
    }

    bool test_output_types()
    {
        using namespace tefri;
        using namespace metaxxa;

        auto pipeline_1 = Pipeline(metaxxa::tuple(make_operator<FakeOperator<double, int>>(), make_operator<FakeOperator<float, char>>()));
        auto pipeline_2 = Pipeline(metaxxa::tuple(make_operator<FakeOperator<float, char>>(), make_operator<FakeOperator<double, int>>()));

        static_assert(std::is_same_v<decltype(pipeline_1)::OutputTuple, Tuple<TEFRI_OPTIONAL<float>>>,  "class Pipeline: output types test failed");
        static_assert(std::is_same_v<decltype(pipeline_2)::OutputTuple, Tuple<TEFRI_OPTIONAL<double>>>, "class Pipeline: output types test failed");

        return true;
    }
};

#endif // TEST_PIPELINE_H