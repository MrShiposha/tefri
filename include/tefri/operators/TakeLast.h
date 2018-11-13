#ifndef TEFRI_TAKELAST_H
#define TEFRI_TAKELAST_H

#include "../detail/RingQueue.h"
#include "Operator.h"
#include "OperatorTemplate.h"

namespace tefri
{
    namespace detail
    {
        template <typename HoldedType>
        class TakeLast : public Operator<HoldedType, HoldedType>
        {
        public:
            using typename Operator<HoldedType, HoldedType>::ArgumentsTuple;
            using typename Operator<HoldedType, HoldedType>::Result;
            using typename Operator<HoldedType, HoldedType>::OptionalResult;

            TakeLast(size_t num)
            : queue(num), iterator(nullptr)
            {}

            virtual OptionalResult operator()(const HoldedType &arg) override
            {
                queue.push(arg);
                return OptionalResult();
            }

            virtual OptionalResult try_complete() override
            {
                if(iterator == nullptr)
                    iterator = std::make_shared<typename RingQueue<HoldedType>::Iterator>(queue.begin());
                
                if(*iterator == queue.end())
                    return OptionalResult();
                else
                {
                    OptionalResult result(*(*iterator)++);
                    return result;
                }
            }

        private:
            RingQueue<HoldedType> queue;
            std::shared_ptr<typename RingQueue<HoldedType>::Iterator> iterator;
        };
    }

    auto take_last(size_t num)
    {
        return make_operator_template<detail::TakeLast>(num);
    }
};

#endif // TEFRI_TAKELAST_H