#ifndef TEST_OPERATORS_H
#define TEST_OPERATORS_H

#include "TestTefri.h"

class TestOperators : public TestTefri
{
public:
    virtual bool test() override
    {
        bool result = true;
        result = result && test_take_last();

        return result;
    }

    bool test_take_last()
    {
        using namespace tefri;

        {
            auto take_last_2_template = take_last(2);
            auto take_last_2 = take_last_2_template->make_operator<int>();
            
            int first  = 42;
            int second = 41;
            (*take_last_2)(first);
            (*take_last_2)(second);

            auto take_last_2_first  = take_last_2->try_complete();
            auto take_last_2_second = take_last_2->try_complete();

            TEST(take_last_2_first, "no value on complete take last 2");
            if(take_last_2_first) 
                TEST(take_last_2_first.value() == 42, "invalid value on complete take last 2");

            TEST(take_last_2_second, "no value on complete take last 2");
            if(take_last_2_second)
                TEST(take_last_2_second.value() == 41, "invalid value on complete take last 2");
        }

        {
            auto take_last_3_template = take_last(3);
            auto take_last_3 = take_last_3_template->make_operator<double>();

            double values[4] = 
            {
                1.,
                1.1,
                1.2,
                1.3
            };

            (*take_last_3)(values[0]);
            (*take_last_3)(values[1]);
            (*take_last_3)(values[2]);
            (*take_last_3)(values[3]);

            for(size_t i = 1; i < 4; ++i)
            {
                auto taken = take_last_3->try_complete();
                TEST(taken, "no value on complete take last 3");   
                if(taken)
                    TEST(taken.value() == values[i], "invalid value on complete take last 3");
            }
        }

        return true;
    }
};

#endif // TEST_OPERATORS_H