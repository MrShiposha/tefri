#ifndef TEFRI_OPERATOR_H
#define TEFRI_OPERATOR_H

#include <memory>

#include "../external/metaxxa/metaxxa.hpp"
#include "../detail/Optional.h"

namespace tefri
{
    template <typename _Result = void, typename... Arguments>
    class Operator
    {
    public:
        using ArgumentsTuple = metaxxa::Tuple<Arguments...>; 
        using Result         = _Result;
        using OptionalResult = TEFRI_OPTIONAL<Result>;

        virtual ~Operator() {}

        virtual OptionalResult operator()(const Arguments&... args) = 0;

        virtual OptionalResult operator()(Arguments&... args)
        {
            return this->operator()(std::forward<std::add_const_t<Arguments>>(args)...);
        }

        virtual OptionalResult try_complete() 
        {
            return OptionalResult();
        }
    };

    template <typename T>
    using OperatorPtr = std::shared_ptr<T>;

    template <typename SomeOperator, typename... Args>
    auto make_operator(Args&&... args)
    {
       return std::make_shared<SomeOperator>(std::forward<Args>(args)...);
    }
}

#endif // TEFRI_OPERATOR_H