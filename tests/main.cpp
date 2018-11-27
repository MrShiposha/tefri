#include <vector>

#include "TestTefri.h"
#include "TestPipeline.h"
#include "TestRingQueue.h"
#include "TestTakeLast.h"
#include "TestObjectHolder.h"
#include "TestUnwrapObjectHolders.h"
#include "TestPipelineBuilder.h"
#include "TestCarryOperator.h"
#include "TestFilter.h"

int main(int argc, char **argv)
{
    std::vector<std::shared_ptr<TestTefri>> tests = 
    {
        std::make_shared<TestPipeline>(),
        std::make_shared<TestRingQueue>(),
        std::make_shared<TestTakeLast>(),
        std::make_shared<TestObjectHolder>(),
        std::make_shared<TestUnwrapObjectHolders>(),
        std::make_shared<TestPipelineBuilder>(),
        std::make_shared<TestCarryOperator>(),
        std::make_shared<TestFilter>()
    };

    for(int i = 0, size = static_cast<int>(tests.size()); i < size; ++i)
        if(!tests[i]->test())
            return -(i + 1);

    std::cout << "All test succeeded" << std::endl;

    return 0;
}