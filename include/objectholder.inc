#ifndef TEFRI_OBJECTHOLDER_INC
#define TEFRI_OBJECTHOLDER_INC

#include "objectholder.h"
#include "flowvalue.inc"

namespace tefri
{
    template <typename T>
    std::shared_future<WrapObject<T>> hold_future(const T &object)
    {
        std::promise<WrapObject<T>> promise;
        promise.set_value(std::make_shared<T>(object));

        return promise.get_future();
    }

    template <typename T>
    ObjectHolder<T>::ObjectHolder(const std::remove_reference_t<T> &object)
    : future(hold_future<T>(object))
    {}

    template <typename T>
    ObjectHolder<T>::ObjectHolder(std::remove_reference_t<T> &&object)
    : future(hold_future<T>(object))
    {}

    template <typename T>
    ObjectHolder<T>::ObjectHolder(Future future)
    : future(future)
    {}

    template <typename T>
    ObjectHolder<T>::~ObjectHolder()
    {}

    template <typename T>
    typename ObjectHolder<T>::Object ObjectHolder<T>::get_copy() const
    {
        return get_ref();
    }

    template <typename T>
    typename ObjectHolder<T>::ObjectRef ObjectHolder<T>::get_ref() const
    {
        if constexpr(detail::is_reference_wrapper<typename WrappedObject::element_type>())
            return future.get()->get();
        else
            return *future.get();
    }

    template <typename T>
    std::future_status ObjectHolder<T>::status() const
    {
        using namespace std::chrono_literals;
        return future.wait_for(0ms);
    }

}

#endif // TEFRI_OBJECTHOLDER_INC