#ifndef TEST_TEFRI_TAKELAST_H
#define TEST_TEFRI_TAKELAST_H

#include "TestTefri.h"

class TestTakeLast : public TestTefri
{
public:
    virtual bool test() override
    {
        bool result = true;
        result = result && basic_test();
        result = result && multiple_arguments_test();

        return result;
    }

    bool basic_test()
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
                TEST(take_last_2_first.value() == metaxxa::Tuple(first), "invalid value on complete take last 2");
            
            TEST(!take_last_2->try_complete(), "take last 2 doesn't complete when expected");

            TEST(take_last_2_second, "no value on complete take last 2");
            if(take_last_2_second)
                TEST(take_last_2_second.value() == metaxxa::Tuple(second), "invalid value on complete take last 2");
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
                    TEST(taken.value() == metaxxa::Tuple(values[i]), "invalid value on complete take last 3");
            }

            TEST(!take_last_3->try_complete(), "take last 3 doesn't complete when expected");
        }

        return true;
    }

    bool multiple_arguments_test()
    {
        using namespace tefri;

        auto take_last_2_template = take_last(2);
        auto take_last_2 = take_last_2_template->make_operator<std::string, int>();

        std::string first_message  = "First: ";
        std::string second_message = "Second: ";
        int first_int  = 42;
        int second_int = 41;
        (*take_last_2)(first_message,  first_int);
        (*take_last_2)(second_message, second_int);

        auto take_last_2_first  = take_last_2->try_complete();
        auto take_last_2_second = take_last_2->try_complete();

        TEST(take_last_2_first, "no value on complete take last 2");
        if(take_last_2_first) 
            TEST(take_last_2_first.value() == metaxxa::Tuple(first_message, first_int), "invalid value on complete take last 2");
        
        TEST(!take_last_2->try_complete(), "take last 2 doesn't complete when expected");

        TEST(take_last_2_second, "no value on complete take last 2");
        if(take_last_2_second)
            TEST(take_last_2_second.value() == metaxxa::Tuple(second_message, second_int), "invalid value on complete take last 2");

        TEST(!take_last_2->try_complete(), "take last 2 doesn't complete when expected");

        return true;
    }
};

#endif // TEST_TEFRI_TAKELAST_H