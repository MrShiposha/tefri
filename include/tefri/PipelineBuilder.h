#ifndef TEFRI_PIPELINEBUILDER_H
#define TEFRI_PIPELINEBUILDER_H

#include "external/metaxxa/metaxxa.hpp"

#include "Pipeline.h"
#include "operators/OperatorTemplate.h"

#include "detail/Any.h"
#include "detail/UnwrapObjectHolders.h"

namespace tefri
{
    namespace detail
    {
        template <typename OperatorPtr>
        struct IsNonTemplate 
        {
            constexpr bool operator()()
            {
                using Operator = typename OperatorPtr::element_type;
                return !is_operator_template<Operator>();
            }
        };

		template <typename OperatorPtr, bool IS_TEMPLATE = is_operator_template<typename OperatorPtr::element_type>()>
		struct OperatorInstantiator 
		{
			template <typename... T>
			static auto instantiate(OperatorPtr last_operator)
			{
				return metaxxa::Tuple(last_operator->template make_operator<T...>());
			}

			template <typename... T>
            using InstatiatedOperatorResult = typename decltype
            (
                OperatorInstantiator::template instantiate<T...>(std::declval<OperatorPtr>())
            )::template Parameter<0>::element_type::Result;
		};

		template <typename OperatorPtr>
		struct OperatorInstantiator<OperatorPtr, false>
		{
			template <typename... T>
			static auto instantiate(OperatorPtr last_operator)
			{
				return metaxxa::Tuple(last_operator);
			}

            template <typename... T>
            using InstatiatedOperatorResult = typename decltype
            (
                OperatorInstantiator::template instantiate<T...>(std::declval<OperatorPtr>())
            )::template Parameter<0>::element_type::Result;
		};

        template <typename OperatorPtr>
        struct BuilderContainsOnlyOperatorTemplates
        {
            constexpr bool operator()()
            {
                return is_operator_template<typename OperatorPtr::element_type>();
            }
        };

        template <bool BUILDER_CONTAINS_OPERATOR_TEMPLATES>
        struct PipelineBuilderImpl
        {
            template <typename PipelineBuilder, typename _OperatorPtrsTuple>
            static auto build(PipelineBuilder &builder)
            {
                if constexpr (_OperatorPtrsTuple::template every_types<BuilderContainsOnlyOperatorTemplates>())
                    return Pipeline(builder.template instantiate_operator_templates<TEFRI_ANY>());
                else
                    return Pipeline
                    (
                        builder.template instantiate_operator_templates_with_tuple
                        <
                            typename decltype(_OperatorPtrsTuple::template find_types<IsNonTemplate>())::Type::element_type::ArgumentsTuple
                        >()
                    );
            }
        };

        template <>
        struct PipelineBuilderImpl<false>
        {
            template <typename PipelineBuilder, typename _OperatorPtrsTuple>
            static auto build(PipelineBuilder &builder)
            {
                return Pipeline(builder.operator_ptrs_tuple);
            }
        };


        template <typename _OperatorPtrsTuple, bool CONTAINS_OPERATOR_TEMPLATES = false>
        class PipelineBuilder
        {
        public:
            PipelineBuilder(const _OperatorPtrsTuple &tuple)
            : operator_ptrs_tuple(tuple)
            {}

            auto build()
            {
                return PipelineBuilderImpl<CONTAINS_OPERATOR_TEMPLATES>::template build<PipelineBuilder, _OperatorPtrsTuple>(*this);
            }

            template <typename OperatorPtr>
            auto with_operator(OperatorPtr new_operator)
            {
                using Operator = typename OperatorPtr::element_type;

                auto new_operator_ptrs_tuple = operator_ptrs_tuple + metaxxa::tuple(new_operator);
                constexpr bool NEW_BUILDER_CONTAINS_OPERATOR_TEMPLATES = CONTAINS_OPERATOR_TEMPLATES || is_operator_template<Operator>();

                return PipelineBuilder<decltype(new_operator_ptrs_tuple), NEW_BUILDER_CONTAINS_OPERATOR_TEMPLATES>(new_operator_ptrs_tuple);
            }

        private:
            template <bool BUILDER_CONTAINS_OPERATOR_TEMPLATES>
            friend struct PipelineBuilderImpl;

            template <typename... Arguments>
            struct OperatorTemplatesInstatiation
            {
                template <size_t INDEX>
                static auto instantiate(_OperatorPtrsTuple &operator_ptrs_tuple)
                {
                    using OperatorPtr  = typename _OperatorPtrsTuple::template Parameter<INDEX>;
                    using Instantiator = OperatorInstantiator<OperatorPtr>;

                    auto current_operator = operator_ptrs_tuple.template get<INDEX>();

                    if constexpr (INDEX + 1 == _OperatorPtrsTuple::size())
                        return Instantiator::template instantiate<std::decay_t<Arguments>...>(current_operator);
                    else
                        return Instantiator::template instantiate<std::decay_t<Arguments>...>(current_operator)
                        + metaxxa::Type
                          <
                            typename Instantiator::template InstatiatedOperatorResult<std::decay_t<Arguments>...>
                          >::template MoveTemplateTypes
                             <
                                PipelineBuilder::template OperatorTemplatesInstatiation
                             >::template instantiate<INDEX + 1>(operator_ptrs_tuple);
                }
            };

            template <typename... T>
            auto instantiate_operator_templates()
            {
                return OperatorTemplatesInstatiation<T...>::template instantiate<0>(operator_ptrs_tuple);
            }

            template <typename Tuple>
            auto instantiate_operator_templates_with_tuple()
            {
                return metaxxa::Type
                <
                    Tuple
                >::template MoveTemplateTypes
                    <
                        PipelineBuilder::template OperatorTemplatesInstatiation
                    >::template instantiate<0>(operator_ptrs_tuple);
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