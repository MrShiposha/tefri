#ifndef TEFRI_PIPELINE_H
#define TEFRI_PIPELINE_H

#include "external/metaxxa/metaxxa.hpp"

namespace tefri
{
    template <typename _OperatorsTuple>
    class Pipeline
    {
    public:
        using OperatorsTuple = _OperatorsTuple;
        static constexpr size_t LENGTH = OperatorsTuple::size();

        using InputTuple     = typename metaxxa::Function<typename OperatorsTuple::template Parameter<0>>::Arguments;
        using OutputTuple    = typename metaxxa::Type
                               <
                                   typename metaxxa::Function<typename OperatorsTuple::template Parameter<LENGTH - 1>>::Result
                               >::template WrapToTemplateIfNotWrapped<metaxxa::Tuple>;

    private:
    };
}

#endif // TEFRI_PIPELINE_H