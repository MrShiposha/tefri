#ifndef TEFRI_OPERATOR_H
#define TEFRI_OPERATOR_H

#include <memory>

#include "../external/metaxxa/metaxxa.hpp"

namespace tefri
{
    template <typename _Result = void, typename... Arguments>
    class Operator
    {
    public:
        using ArgumentTuple = metaxxa::Tuple<Arguments...>; 
        using Result        = _Result;

        virtual ~Operator() {}

        virtual Result operator()(Arguments&&... args) = 0;
    };

    template <typename SomeOperator, typename... Args>
    auto make_operator(Args&&... args)
    {
       return std::make_shared<SomeOperator>(std::forward<Args>(args)...);
    }
}

#endif // TEFRI_OPERATOR_H