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
        result = result && test_build();

        return result;
    }

    bool test_default_constructor()
    {
        using namespace tefri;
        static_assert(metaxxa::Type<decltype(pipeline_builder())>() == metaxxa::Type<detail::PipelineBuilder<metaxxa::Tuple<>>>(), "invalid empty pipeline builder type"); 

        return true;
    }

    bool test_build()
    {
        using namespace tefri;

        {
            auto builder = pipeline_builder();
            static_assert(metaxxa::Type<decltype(builder.build())>() == metaxxa::Type<Pipeline<metaxxa::Tuple<>>>(), "invalid empty pipeline type"); 
        }

        return true;
    }
};

#endif // TEFRI_TEST_PIPELINEBUILDER_H