#ifndef TEFRI_DETAIL_OBJECTHOLDER_H
#define TEFRI_DETAIL_OBJECTHOLDER_H

#include <memory>
#include <future>
#include <chrono>

#include "../lib/metaxxa.hpp"

#include "flowvalue.h"

namespace tefri
{
    using namespace metaxxa;

    namespace detail
    {
        template <typename T>
        constexpr bool is_reference_wrapper()
        {
            return is_instatiation_of<std::decay_t<T>, std::reference_wrapper>();
        }

        template <typename T, bool IS_REFERENCE_WRAPPER = is_reference_wrapper<T>()>
        struct UnwrapReference 
        {
            using Type = T;
        };

        template <typename T>
        struct UnwrapReference<T, true>
        {
            using Type = typename T::type;
        };
    }

    template <typename T>
    using WrapObject = std::shared_ptr<T>;

    template <typename T>
    using UnwrapReference = typename detail::UnwrapReference<T>::Type;

    template <typename T>
    using UnwrapObject = UnwrapReference<typename T::element_type>;

    template <typename T>
    std::shared_future<WrapObject<T>> hold_future(const T &object);

    template <typename T>
    class ObjectHolder : public FlowValue
    {
    public:
        using Object        = UnwrapReference<T>;
        using ObjectRef     = std::add_lvalue_reference_t<Object>;
        using WrappedObject = WrapObject<T>;
        using Future        = std::shared_future<WrappedObject>;

        ObjectHolder() = delete;

        ObjectHolder(T &object);

        ObjectHolder(T &&object);

        ObjectHolder(Future future);

        ObjectHolder(const ObjectHolder &) = default;

        ObjectHolder(ObjectHolder &&) = default;

        ~ObjectHolder();

        ObjectHolder &operator=(const ObjectHolder &) = default;

        ObjectHolder &operator=(ObjectHolder &&) = default;

        Object get_copy() const;

        ObjectRef get_ref() const;

        std::future_status status() const;

    private:
        Future future;
    };

    template <typename T>
    ObjectHolder(std::shared_future<WrapObject<T>>) -> ObjectHolder<T>;

    template <typename T>
    ObjectHolder(std::future<WrapObject<T>>) -> ObjectHolder<T>;
}

#endif // TEFRI_DETAIL_OBJECTHOLDER_H