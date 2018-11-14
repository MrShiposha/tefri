#ifndef TEFRI_FILTER_H
#define TEFRI_FILTER_H

#include "CarryOperator.h"

namespace tefri
{
    namespace detail
    {
        template <typename _Result, typename... Args>
        class Filter : public CarryOperator
        <
            _Result(Args...), 
            metaxxa::Tuple<Args...>, 
            Args...
        >
        {
        public:
            using Base = CarryOperator<_Result(Args...), metaxxa::Tuple<Args...>, Args...>;

            using ArgumentsTuple = typename Base::ArgumentsTuple;
            using Result         = typename Base::Result;
            using OptionalResult = typename Base::OptionalResult;

            template <typename Callable>
            Filter(Callable callable)
            : CarryOperator<_Result(Args...), metaxxa::Tuple<Args...>, Args...>(callable)
            {
                using namespace metaxxa;
                static_assert
                (
                    Type<typename Function<Callable>::Result>() == Type<bool>(), 
                    "Tefri: only callable with bool return value can be carried by filer operator"
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