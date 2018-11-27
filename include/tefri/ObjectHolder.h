#ifndef TEFRI_DETAIL_OBJECTHOLDER_H
#define TEFRI_DETAIL_OBJECTHOLDER_H

#include <type_traits>
#include <functional>

#include "external/metaxxa/metaxxa.hpp"
#include "detail/UnwrapReferenceWrapper.h"

namespace tefri
{
    template <typename T, bool IS_REFERENCE_WRAPPER = metaxxa::Type<T>::template is_instantiation_of<std::reference_wrapper>()>
    struct ObjectWrapperImpl 
    {
        using Type = T;
    };

    template <typename T>
    struct ObjectWrapperImpl<T, false>
    {
        using Type = std::shared_ptr<T>;
    };

    template <typename T>
    using ObjectWrapper = typename ObjectWrapperImpl<T>::Type;

    template <typename T>
    auto make_object_wrapper(std::add_const_t<std::add_lvalue_reference_t<T>> object)
    {
        if constexpr (metaxxa::Type<T>::template is_instantiation_of<std::reference_wrapper>())
            return object;
        else
            return std::make_shared<T>(object);
    }


    template <typename T>
    class ObjectHolder
    {
    public:
        using Object        = detail::UnwrapReferenceWrapper<T>;
        using ObjectRef     = std::add_lvalue_reference_t<Object>;
        using ObjectWrapper = ObjectWrapper<T>;

        ObjectHolder(T &object)
        : wrapped_object(make_object_wrapper<T>(object))
        {}

        ObjectHolder(T &&object)
        : wrapped_object(make_object_wrapper<T>(object))
        {}

        ObjectHolder(const ObjectHolder &) = default;

        ObjectHolder(ObjectHolder &&) = default;

        ~ObjectHolder() = default;

        ObjectHolder &operator=(const ObjectHolder &) = default;

        ObjectHolder &operator=(ObjectHolder &&) = default;

        Object get_object_copy()
        {
            if constexpr(metaxxa::Type<ObjectWrapper>::template is_instantiation_of<std::reference_wrapper>())
                return wrapped_object;
            else
                return *wrapped_object;
        }

        ObjectRef get_object_ref()
        {
            if constexpr(metaxxa::Type<ObjectWrapper>::template is_instantiation_of<std::reference_wrapper>())            
                return wrapped_object;
            else
                return *wrapped_object;
        }

    private:
        ObjectWrapper wrapped_object;
    };
}

#endif // TEFRI_DETAIL_OBJECTHOLDER_H