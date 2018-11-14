#ifndef TEFRI_OPERATORTEMPLATE_H
#define TEFRI_OPERATORTEMPLATE_H

#include <memory>
#include <utility>

#include "../external/metaxxa/metaxxa.hpp"
#include "Operator.h"

namespace tefri
{
    namespace detail
    {
        class OperatorTemplateBase
        {};

        template <template <typename...> typename Operator, typename... ConstructorArguments>
        class OperatorTemplate : public OperatorTemplateBase
        {
            using ConstructorArgumentsTuple = metaxxa::Tuple<std::decay_t<ConstructorArguments>...>;
        public:
            OperatorTemplate(ConstructorArguments&&... args)
            : constructor_arguments(std::forward<ConstructorArguments>(args)...)
            {}

            template <typename... OperatorArguments>
            auto make_operator() const
            {
                return make_operator_from_tuple<Operator<OperatorArguments...>>(std::make_index_sequence<ConstructorArgumentsTuple::size()>());
            }

        private:
            template <typename OperatorInstance, size_t... INDICES>
            auto make_operator_from_tuple(std::index_sequence<INDICES...>) const
            {
                return ::tefri::make_operator<OperatorInstance>(constructor_arguments.METAXXA_TEMPLATE get<INDICES>()...);
            }

            ConstructorArgumentsTuple constructor_arguments;
        };
    }

    template <template <typename...> typename Operator, typename... ConstructorArguments>
    auto make_operator_template(ConstructorArguments&&... args)
    {
        return std::make_shared<detail::OperatorTemplate<Operator, ConstructorArguments...>>(args...);
    }

    template <typename Operator>
    constexpr bool is_operator_template()
    {
        return metaxxa::Type<Operator>::template is_derived_from<detail::OperatorTemplateBase>();
    }
}

#endif // TEFRI_OPERATORTEMPLATE_H