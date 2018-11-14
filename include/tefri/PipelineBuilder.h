#ifndef TEFRI_PIPELINEBUILDER_H
#define TEFRI_PIPELINEBUILDER_H

#include "external/metaxxa/metaxxa.hpp"

#include "Pipeline.h"

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
                    return Pipeline(operator_ptrs_tuple);
                }
                else
                    return Pipeline(operator_ptrs_tuple);
            }

            template <typename OperatorPtr>
            auto with_operator(OperatorPtr new_operator)
            {
                using Operator = typename OperatorPtr::element_type;

                auto new_operator_ptrs_tuple = operator_ptrs_tuple + metaxxa::tuple(new_operator);
                constexpr bool CONTAINS_OPERATOR_TEMPLATES = metaxxa::Type<Operator>::template is_instantiation_of<OperatorTemplate>();

                return PipelineBuilder<decltype(new_operator_ptrs_tuple), CONTAINS_OPERATOR_TEMPLATES>(new_operator_ptrs_tuple);
            }

        private:
            template <typename T, size_t INDEX>
            auto instantiate_operator_template()
            {
                using OperatorPtr = typename _OperatorPtrsTuple::template Parameter<INDEX>;
                using Operator    = typename OperatorPtr::element_type;

                auto current_operator_template_ptr = operator_ptrs_tuple.template get<INDEX>();

                if (INDEX + 1 == _OperatorPtrsTuple::size())
                {
                    if constexpr (metaxxa::Type<Operator>::template is_instantiation_of<OperatorTemplate>())
                        return metaxxa::tuple(current_operator_template_ptr->make_operator<T>());
                    else
                        return metaxxa::Tuple<>();
                }
                else
                {
                    if constexpr (metaxxa::Type<Operator>::template is_instantiation_of<OperatorTemplate>())
                        return metaxxa::tuple(current_operator_template_ptr->make_operator<T>())
                        + instantiate_operator_template<typename decltype(current_operator_template_ptr->make_operator<T>())::element_type::Result, INDEX + 1>();
                    else
                        return instantiate_operator_template<typename Operator::Result, INDEX + 1>();                    
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