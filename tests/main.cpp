#include "TestTefri.h"
#include "TestPipeline.h"
#include "TestRingQueue.h"
#include "TestOperators.h"

#include <vector>


int main(int argc, char **argv)
{
    std::vector<std::shared_ptr<TestTefri>> tests = 
    {
        std::make_shared<TestPipeline>(),
        std::make_shared<TestRingQueue>(),
        std::make_shared<TestOperators>()
    };

    for(auto &&test : tests)
        if(!test->test())
            return -1;

    std::cout << "All test succeeded" << std::endl;

    return 0;
}