#ifndef TEFRI_OPERATORTEMPLATE_H
#define TEFRI_OPERATORTEMPLATE_H

#include <memory>

#include "../external/metaxxa/metaxxa.hpp"
#include "Operator.h"

namespace tefri
{
    namespace detail
    {
        template <template <typename OperatorArgumentTuple> typename Operator, typename... ConstructorArguments>
        class OperatorTemplate    
        {
        public:
            OperatorTemplate(ConstructorArguments&&... args)
            : constructor_arguments(std::forward<ConstructorArguments>(args)...)
            {}

            template <typename OperatorArgumentTuple>
            auto make_operator() const
            {
                return constructor_arguments.call_function(make_operator<Operator<OperatorArgumentTuple>>);
            }

        private:
            metaxxa::Tuple<ConstructorArguments...> constructor_arguments;
        };
    }

    template <template <typename OperatorArgumentTuple> typename Operator, typename... ConstructorArguments>
    auto operator_template(ConstructorArguments&&... args)
    {
        return std::make_shared<detail::OperatorTemplate<Operator, ConstructorArguments...>>(args...);
    }
}

#endif // TEFRI_OPERATORTEMPLATE_H