#ifndef TEFRI_PIPELINEBUILDER_H
#define TEFRI_PIPELINEBUILDER_H

#include "external/metaxxa/metaxxa.hpp"

#include "Pipeline.h"
#include "operators/OperatorTemplate.h"

#include "detail/Any.h"

namespace tefri
{
    namespace detail
    {
        template <typename _OperatorPtrsTuple, bool CONTAINS_OPERATOR_TEMPLATES = false>
        class PipelineBuilder
        {
        public:
            PipelineBuilder(const _OperatorPtrsTuple &tuple)
            : operator_ptrs_tuple(tuple)
            {}

            auto build()
            {
                if constexpr (CONTAINS_OPERATOR_TEMPLATES)
                {
                    if constexpr (contains_only_operator_templates())
                    {
                        std::cout << "only templates" << std::endl;
                        return Pipeline(instantiate_operator_templates<std::any, 0>());
                    }
                    else
                        return Pipeline(std::metaxxa::Tuple<>());
                }
                else
                    return Pipeline(operator_ptrs_tuple);
            }

            template <typename OperatorPtr>
            auto with_operator(OperatorPtr new_operator)
            {
                using Operator = typename OperatorPtr::element_type;

                auto new_operator_ptrs_tuple = operator_ptrs_tuple + metaxxa::tuple(new_operator);
                constexpr bool NEW_BUILDER_CONTAINS_OPERATOR_TEMPLATES = is_operator_template<Operator>();

                return PipelineBuilder<decltype(new_operator_ptrs_tuple), NEW_BUILDER_CONTAINS_OPERATOR_TEMPLATES>(new_operator_ptrs_tuple);
            }

        private:
            template <typename OperatorPtr>
            struct ContainsOnlyOperatorTemplates
            {
                constexpr bool operator()()
                {
                    return is_operator_template<typename OperatorPtr::element_type>();
                }
            };

            static constexpr bool contains_only_operator_templates()
            {
                return _OperatorPtrsTuple::template every_types<ContainsOnlyOperatorTemplates>();
            }

            template <typename T, size_t INDEX>
            auto instantiate_operator_templates()
            {
                using OperatorPtr = typename _OperatorPtrsTuple::template Parameter<INDEX>;
                using Operator    = typename OperatorPtr::element_type;

                auto current_operator_template_ptr = operator_ptrs_tuple.template get<INDEX>();

                if constexpr (INDEX + 1 == _OperatorPtrsTuple::size())
                {
                    if constexpr (is_operator_template<Operator>())
                        return metaxxa::tuple(current_operator_template_ptr->template make_operator<T>());
                    else
                        return metaxxa::Tuple<>();
                }
                else
                {
                    if constexpr (is_operator_template<Operator>())
                        return metaxxa::tuple(current_operator_template_ptr->template make_operator<T>())
                        + instantiate_operator_templates<typename decltype(current_operator_template_ptr->template make_operator<T>())::element_type::Result, INDEX + 1>();
                    else
                        return instantiate_operator_templates<typename Operator::Result, INDEX + 1>();                    
                }
            }

            _OperatorPtrsTuple operator_ptrs_tuple;
        };
    }

    auto pipeline_builder()
    {
        return detail::PipelineBuilder(metaxxa::Tuple<>());
    }
}

#endif // TEFRI_PIPELINEBUILDER_H