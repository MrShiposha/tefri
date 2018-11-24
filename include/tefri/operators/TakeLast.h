#ifndef TEFRI_TAKELAST_H
#define TEFRI_TAKELAST_H

#include "../detail/RingQueue.h"
#include "Operator.h"
#include "OperatorTemplate.h"

namespace tefri
{
    namespace detail
    {
        template <typename... HoldedTypes>
        class TakeLast : public Operator<metaxxa::Tuple<HoldedTypes...>, HoldedTypes...>
        {
        public:
            using typename Operator<metaxxa::Tuple<HoldedTypes...>, HoldedTypes...>::ArgumentsTuple;
            using typename Operator<metaxxa::Tuple<HoldedTypes...>, HoldedTypes...>::Result;
            using typename Operator<metaxxa::Tuple<HoldedTypes...>, HoldedTypes...>::OptionalResult;

            TakeLast(size_t num)
            : queue(num), iterator(nullptr)
            {}

            virtual OptionalResult operator()(const HoldedTypes&... args) override
            {
                queue.push(metaxxa::Tuple<HoldedTypes...>(args...));
                return OptionalResult();
            }

            virtual OptionalResult try_complete() override
            {
                if(iterator == nullptr)
                    iterator = std::make_shared<typename RingQueue<metaxxa::Tuple<HoldedTypes...>>::Iterator>(queue.begin());
                
                if(*iterator == queue.end())
                    return OptionalResult();
                else
                {
                    OptionalResult result(*(*iterator)++);
                    return result;
                }
            }

        private:
            RingQueue<metaxxa::Tuple<HoldedTypes...>> queue;
            std::shared_ptr<typename RingQueue<metaxxa::Tuple<HoldedTypes...>>::Iterator> iterator;
        };
    }

    auto take_last(size_t num)
    {
        return make_operator_template<detail::TakeLast>(num);
    }
};

#endif // TEFRI_TAKELAST_H