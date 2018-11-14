#ifndef TEFRI_CARRYOPERATOR_H
#define TEFRI_CARRYOPERATOR_H

#include <functional>

#include "../external/metaxxa/metaxxa.hpp"

#include "Operator.h"

namespace tefri
{
    namespace detail
    {
        template <template <typename Result, typename... Args> typename CarryOperator, typename Result>
        struct CarryOperatorInstanceImpl
        {
            template <typename... Args>
            using Type = CarryOperator<Result, Args...>;
        };

        template <template <typename Result, typename... Args> typename CarryOperator, typename Callable>
        using CarryOperatorInstance = typename metaxxa::Type<typename metaxxa::Function<Callable>::Arguments>::template MoveTemplateTypes
        <
            CarryOperatorInstanceImpl
            <
                CarryOperator, 
                typename metaxxa::Function<Callable>::Result
            >::template Type
        >;
    }

    template <typename Callable, typename Result, typename... Args>
    class CarryOperator : public Operator<Result, Args...>
    {
    public:
        using typename Operator<Result, Args...>::ArgumentsTuple;
        using typename Operator<Result, Args...>::Result;
        using typename Operator<Result, Args...>::OptionalResult;

        CarryOperator(std::function<Callable> callable)
        : callable(callable)
        {}

        template <typename... CallableArgs>
        auto invoke(CallableArgs&&... args)
        {
            return callable(std::forward<CallableArgs>(args)...);
        }

    private:
        std::function<Callable> callable;
    };

    template <template <typename Result, typename... Args> typename CarryOperator, typename Callable>
    auto make_carry_operator(Callable callable)
    {
        return std::make_shared<detail::CarryOperatorInstance<CarryOperator, Callable>>(callable);
    }
}

#endif // TEFRI_CARRYOPERATOR_H