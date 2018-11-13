#ifndef TEST_TEFRI_DETAIL_RINGQUEUE_H
#define TEST_TEFRI_DETAIL_RINGQUEUE_H

#include "TestTefri.h"

class TestRingQueue : public TestTefri
{
public:
    virtual bool test() 
    {
        bool result = true;
        result = result && test_push();

        return result;
    }

    bool test_push()
    {
        using namespace tefri::detail;

        {
            RingQueue<int> queue(4);
            queue.push(1);
            queue.push(2);
            queue.push(3);
            queue.push(4);

            size_t i = 0;
            for(auto &&item : queue)
                TEST(item == ++i, "invalid value in full queue -- possible invalid work of iterators");

            queue.push(5);        

            i = 1;
            for(auto &&item : queue)
                TEST(item == ++i, "invalid value in queue that was overflowed");
        }

        {
            RingQueue<int> queue(4);
            queue.push(1);
            queue.push(2);
            size_t i = 0;
            for(auto &&item : queue)
                TEST(item == ++i, "invalid value in queue which not full");
            TEST(i == 2, "invalud end iterator position");
            
        }

        return true;
    }
};

#endif // TEST_TEFRI_DETAIL_RINGQUEUE_H