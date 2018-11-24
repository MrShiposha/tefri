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
        template <typename OperatorPtr>
        struct NonTemplate 
        {
            constexpr bool operator()()
            {
                using Operator = typename OperatorPtr::element_type;
                return is_operator_template<Operator>();
            }
        };

		template <typename OperatorPtr, bool IS_TEMPLATE = is_operator_template<typename OperatorPtr::element_type>()>
		struct Instantiation 
		{
			template <typename... T>
			static auto instantiate(OperatorPtr last_operator)
			{
				return metaxxa::Tuple(last_operator->template make_operator<T...>());
			}

			template <typename... T>
			using Result = typename decltype(std::declval<OperatorPtr>()->template make_operator<T...>())::element_type::Result;
		};

		template <typename OperatorPtr>
		struct Instantiation<OperatorPtr, false>
		{
			using Result = typename OperatorPtr::element_type::Result;

			template <typename... T>
			static auto instantiate(OperatorPtr last_operator)
			{
				return metaxxa::Tuple(last_operator);
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
                if constexpr (CONTAINS_OPERATOR_TEMPLATES)
                {
                    if constexpr (contains_only_operator_templates())
                        return Pipeline(instantiate_operator_templates<TEFRI_ANY>());
                    else
                        return Pipeline
                        (
                            instantiate_operator_templates
                            <
                                typename decltype(_OperatorPtrsTuple::template find_types<NonTemplate>())::Type
                            >()
                        );
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

            template <typename... Arguments>
            struct OperatorTemplatesInstatiation
            {
                template <size_t INDEX>
                static auto instantiate(_OperatorPtrsTuple &operator_ptrs_tuple)
                {
                    using OperatorPtr           = typename _OperatorPtrsTuple::template Parameter<INDEX>;
                    using OperatorInstantiation = Instantiation<OperatorPtr>;

                    auto current_operator = operator_ptrs_tuple.template get<INDEX>();

                    if constexpr (INDEX + 1 == _OperatorPtrsTuple::size())
                        return OperatorInstantiation::template instantiate<std::decay_t<Arguments>...>(current_operator);
                    else
                        return OperatorInstantiation::template instantiate<std::decay_t<Arguments>...>(current_operator)
                        + typename metaxxa::Type
                          <
                            typename OperatorInstantiation::Result
                          >::template MoveTemplateTypes
                             <
                                OperatorTemplatesInstatiation
                             >::template instantiate<INDEX + 1>(operator_ptrs_tuple);
                }
            };

            template <typename... T>
            auto instantiate_operator_templates()
            {
                return OperatorTemplatesInstatiation<T...>::template instantiate<0>(operator_ptrs_tuple);
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