#ifndef TEFRI_TEST_PIPELINEBUILDER_H
#define TEFRI_TEST_PIPELINEBUILDER_H

#include "TestTefri.h"

class TestPipelineBuilder : public TestTefri
{
public:
    virtual bool test() override
    {
        bool result = true;
        result = result && test_default_constructor();
        result = result && test_empty_build();
        result = result && test_operator_template_build();
        result = result && test_carry_operator_build();

        return result;
    }

    bool test_default_constructor()
    {
        using namespace tefri;
        static_assert(metaxxa::Type<decltype(pipeline_builder())>() == metaxxa::Type<detail::PipelineBuilder<metaxxa::Tuple<>>>(), "invalid empty pipeline builder type"); 

        return true;
    }

    bool test_empty_build()
    {
        using namespace tefri;

        {
            auto builder = pipeline_builder();
            static_assert(metaxxa::Type<decltype(builder.build())>() == metaxxa::Type<Pipeline<metaxxa::Tuple<>>>(), "invalid empty pipeline type"); 
        }

        return true;
    }

    bool test_operator_template_build()
    {
        using namespace tefri;

        {
            auto pipeline = pipeline_builder()
                .with_operator(take_last(2)) 
                .build();

            static_assert
            (
                metaxxa::Type<decltype(pipeline)>() == metaxxa::Type<Pipeline<metaxxa::Tuple<OperatorPtr<detail::TakeLast<TEFRI_ANY>>>>>(), 
                "invalid pipeline with operator templates type"
            ); 
        }

        return true;
    }

    bool test_carry_operator_build()
    {
        using namespace tefri;

        {
            auto pipeline = pipeline_builder()
                .with_operator(filter([](int v) { return v > 10; }))
                .build();

            static_assert
            (
                metaxxa::Type<decltype(pipeline)>() == metaxxa::Type
                <
                    Pipeline
                    <
                        metaxxa::Tuple
                        <
                            OperatorPtr<detail::Filter<bool, int>>
                        >
                    >
                >(), 
                "invalid pipeline with carry operators"
            ); 
        }

        return true;
    }
};

#endif // TEFRI_TEST_PIPELINEBUILDER_H