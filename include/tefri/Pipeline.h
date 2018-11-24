#ifndef TEFRI_PIPELINE_H
#define TEFRI_PIPELINE_H

#include "external/metaxxa/metaxxa.hpp"

#include "ObjectHolder.h"

namespace tefri
{
    namespace detail
    {
        template <typename OperatorPtr>
        struct UnwrapOperatorPtr
        {
        public:
            constexpr auto operator()() -> typename OperatorPtr::element_type
            { throw; }
        };
    }

    template <typename _OperatorsPtrsTuple>
    class Pipeline
    {
    public:
        using OperatorsTuple = decltype(_OperatorsPtrsTuple::template map_types<detail::UnwrapOperatorPtr>());
        static constexpr size_t LENGTH = _OperatorsPtrsTuple::size();

        using InputTuple     = typename metaxxa::Function<typename _OperatorsPtrsTuple::template Parameter<0>::element_type>::Arguments;
        using OutputTuple    = typename metaxxa::Type
                               <
                                   typename metaxxa::Function<typename _OperatorsPtrsTuple::template Parameter<LENGTH - 1>::element_type>::Result
                               >::template WrapToTemplateIfNotWrapped<metaxxa::Tuple>;

        explicit Pipeline(const _OperatorsPtrsTuple &operators_ptrs_tuple)
        : operators_ptrs_tuple(operators_ptrs_tuple)
        {}

    private:
        _OperatorsPtrsTuple operators_ptrs_tuple;
    };

    template <>
    class Pipeline<metaxxa::Tuple<>>
    {
    public:
        static constexpr size_t LENGTH = 0;

        explicit Pipeline(const metaxxa::Tuple<> &operators_ptrs_tuple) 
        {}
    };
}

#endif // TEFRI_PIPELINE_H