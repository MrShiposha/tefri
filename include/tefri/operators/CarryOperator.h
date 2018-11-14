#ifndef TEFRI_CARRYOPERATOR_H
#define TEFRI_CARRYOPERATOR_H

#include "../external/metaxxa/metaxxa.hpp"

#include "Operator.h"

namespace tefri
{
    namespace detail
    {
        template <typename Result>
        struct CarryOperatorBaseImpl
        {
            template <typename... Args>
            using Type = Operator<Result, Args...>;
        };

        template <typename Callable>
        using CarryOperatorBase = typename metaxxa::Type<typename metaxxa::Function<Callable>::Arguments>
        ::template MoveTemplateTypes<typename CarryOperatorBaseImpl<typename metaxxa::Function<Callable>::Result>::Type>;
    }

    template <typename Callable>
    class CarryOperator : public detail::CarryOperatorBase<Callable>
    {
    public:
        CarryOperator(Callable callable)
        : CarryOperator(), callable(callable)
        {}

        template <typename... Args>
        auto invoke(Args&&... args)
        {
            return callable(std::forward<Args>(args)...);
        }

    private:
        std::function<Callable> callable;
    };
}

#endif // TEFRI_CARRYOPERATOR_H