#ifndef TEFRI_DETAIL_OBJECTHOLDER_H
#define TEFRI_DETAIL_OBJECTHOLDER_H

#include <type_traits>
#include <functional>

#include "../external/metaxxa/metaxxa.hpp"

namespace tefri::detail
{
    template <typename T, bool IS_REFERENCE_WRAPPER = metaxxa::Type<T>::is_instantiation_of<std::reference_wrapper>()>
    struct ObjectTypeImpl 
    {
        using Type = T;
    };

    template <typename T>
    struct ObjectTypeImpl<T, true>
    {
        using Type = typename T::type;
    };

    template <typename T>
    using ObjectType = typename ObjectTypeImpl<T>::Type;


    template <typename T>
    class ObjectHolder
    {
    public:
        using Object = ObjectType<T>;

        using ObjectRef = std::add_lvalue_reference_t<Object>;

        ObjectHolder(T object)
        : object(object)
        {}

        ObjectHolder(const ObjectHolder &) = default;

        ObjectHolder(ObjectHolder &&) = default;

        ~ObjectHolder() = default;

        ObjectHolder &operator=(const ObjectHolder &) = default;

        ObjectHolder &operator=(ObjectHolder &&) = default;

        Object get_object_copy()
        {
            return object;
        }

        ObjectRef get_object_ref()
        {
            return object;
        }

    private:
        T object;
    };
}

#endif // TEFRI_DETAIL_OBJECTHOLDER_H