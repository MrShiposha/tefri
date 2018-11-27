#ifndef TEFRI_DETAIL_UNWRAPOBJECTHOLDERS_H
#define TEFRI_DETAIL_UNWRAPOBJECTHOLDERS_H

#include "../external/metaxxa/metaxxa.hpp"
#include "../ObjectHolder.h"

#include "UnwrapReferenceWrapper.h"

namespace tefri::detail
{
    template <typename T>
    constexpr bool is_tuple()
    {
        return metaxxa::Type<T>::template is_instantiation_of<metaxxa::Tuple>();
    }

    template <typename T>
    constexpr bool is_object_holder()
    {
        return metaxxa::Type<T>::template is_instantiation_of<ObjectHolder>();
    }

    template <typename T, bool IS_OBJECT_HOLDER = is_object_holder<T>()>
    struct _UnwrapObjectHolder
    {
        using Type = typename T::Object;
    };

    template <typename T>
    struct _UnwrapObjectHolder<T, false>
    {
        using Type = UnwrapReferenceWrapper<T>;
    };

    template <typename T>
    using UnwrapObjectHolder = typename _UnwrapObjectHolder<T>::Type;



    template <typename T>
    struct Unwrapper
    {
        constexpr auto operator()() -> UnwrapObjectHolder<T>;
    };

    template<typename T, bool IS_TUPLE = is_tuple<T>()>
    struct _UnwrapObjectHolders
    {
        using Type = decltype(T::template map_types<Unwrapper>());
    };

    template<typename T>
    struct _UnwrapObjectHolders<T, false>
    {
        using Type = UnwrapObjectHolder<T>;
    };

    template<typename T>
    using UnwrapObjectHolders = typename _UnwrapObjectHolders<T>::Type;
}

#endif // TEFRI_DETAIL_UNWRAPOBJECTHOLDERS_H