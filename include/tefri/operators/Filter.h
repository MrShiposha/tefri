#ifndef TEFRI_FILTER_H
#define TEFRI_FILTER_H

#include "CarryOperator.h"

namespace tefri
{
    namespace detail
    {
        template <typename CallableResult, typename... Args>
        class Filter : public CarryOperator
        <
            CallableResult(Args...), 
            metaxxa::Tuple<Args...>, 
            Args...
        >
        {
        public:
            using Result = metaxxa::Tuple<Args...>;

            using Base = CarryOperator
            <
                CallableResult(Args...), 
                Result,
                Args...
            >;

            using ArgumentsTuple = typename Base::ArgumentsTuple;
            using OptionalResult = typename Base::OptionalResult;

            template <typename Callable>
            Filter(Callable callable)
            : Base(callable)
            {
                using namespace metaxxa;
                static_assert
                (
                    Type<typename Function<Callable>::Result>() == Type<bool>(), 
                    "Tefri: only callable with bool return value can be carried by filter operator"
                );
            }

            virtual OptionalResult operator()(const Args&... args) override
            {
                if(this->invoke(args...))
                    return metaxxa::Tuple(args...);
                else
                    return {};
            }
        };
    }

    template <typename Callable>
    auto filter(Callable callable)
    {
        return make_carry_operator<detail::Filter>(callable);
    }
}

#endif // TEFRI_FILTER_H