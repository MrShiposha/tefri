#ifndef TEFRI_DETAIL_UNWRAPREFERENCEWRAPPER_H
#define TEFRI_DETAIL_UNWRAPREFERENCEWRAPPER_H

#include "../external/metaxxa/metaxxa.hpp"

namespace tefri::detail
{
    template <typename T>
    constexpr bool is_reference_wrapper()
    {
        return metaxxa::Type<T>::template is_instantiation_of<std::reference_wrapper>();
    }

    template <typename T, bool IS_REFERENCE_WRAPPER = is_reference_wrapper<T>()>
    struct _UnwrapReferenceWrapper 
    {
        using Type = T;
    };

    template <typename T>
    struct _UnwrapReferenceWrapper<T, true>
    {
        using Type = typename T::type;
    };

    template <typename T>
    using UnwrapReferenceWrapper = typename _UnwrapReferenceWrapper<T>::Type;
}

#endif // TEFRI_DETAIL_UNWRAPREFERENCEWRAPPER_H
