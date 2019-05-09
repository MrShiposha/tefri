#ifndef TEFRI_TUPLE_INC
#define TEFRI_TUPLE_INC

#include "tuple.h"

namespace tefri
{
    using namespace metaxxa;

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...>::Tuple(Args&&... args)
    : objects
    (
        std::make_shared<Container>
        (
            std::initializer_list<std::shared_ptr<void>> 
            {
                std::static_pointer_cast<void>(std::make_shared<Args>(std::forward<Args>(args)))...
            }
        )
    )
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...>::Tuple(const Args &... args)
    : objects
    (
        std::make_shared<Container>
        (
            std::initializer_list<std::shared_ptr<void>> 
            {
                std::static_pointer_cast<void>(std::make_shared<Args>(args))...
            }
        )
    )
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...>::Tuple(Args*... args)
    : Tuple(std::shared_ptr<Args>(args)...)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...>::Tuple(std::shared_ptr<Args>... args)
    : objects
    (
        std::make_shared<Container>
        (
            std::initializer_list<std::shared_ptr<void>> 
            { 
                std::static_pointer_cast<void>(args)... 
            }
        )
    )
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...>::Tuple(const Tuple &o)
    : objects(std::make_shared<Container>(*o.objects))
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...>::Tuple(Tuple &&o)
    : objects(std::move(o.objects))
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...>::~Tuple()
    {
        deallocate(MakeReverseIndexRange<size(), 0>());
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...>::Tuple(detail::ShareTuple, Objects objects)
    : objects(objects)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...> &Tuple<PtrContainer, Args...>::operator=(const Tuple &rhs)
    {
        if(this != &rhs)
            *this = std::move(Tuple<PtrContainer, Args...>(rhs));

        return *this;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...> &Tuple<PtrContainer, Args...>::operator=(Tuple &&rhs)
    {
        objects = std::move(rhs.objects);
        return *this;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <std::size_t... INDICES>
    void Tuple<PtrContainer, Args...>::deallocate(std::index_sequence<INDICES...>)
    {
        ((*objects)[INDICES].reset(), ...);
        objects.reset();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <std::size_t INDEX>
    auto Tuple<PtrContainer, Args...>::get()
        -> typename TypeTuple::template Get<INDEX> &
    {
        return *get_ptr<INDEX>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <std::size_t INDEX>
    auto Tuple<PtrContainer, Args...>::get() const
        -> std::add_const_t<typename TypeTuple::template Get<INDEX>> &
    {
        return const_cast<Tuple>(this)->template get<INDEX>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <std::size_t INDEX>
    auto Tuple<PtrContainer, Args...>::get_ptr()
        -> std::shared_ptr<typename TypeTuple::template Get<INDEX>>
    {
        return std::static_pointer_cast<typename TypeTuple::template Get<INDEX>>(get_ptr(INDEX));        
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <std::size_t INDEX>
    auto Tuple<PtrContainer, Args...>::get_ptr() const
        -> std::shared_ptr<std::add_const_t<typename TypeTuple::template Get<INDEX>>>
    {
        return const_cast<Tuple>(this)->template get_ptr<INDEX>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    std::shared_ptr<void> Tuple<PtrContainer, Args...>::get_ptr(std::size_t index)
    {
        assert(index < objects->size());

        return *(objects->begin() + index);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    std::shared_ptr<const void> Tuple<PtrContainer, Args...>::get_ptr(std::size_t index) const
    {
        return const_cast<Tuple>(this)->get_ptr(index);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    typename Tuple<PtrContainer, Args...>::Objects Tuple<PtrContainer, Args...>::raw_objects()
    {
        return objects;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    const typename Tuple<PtrContainer, Args...>::Objects Tuple<PtrContainer, Args...>::raw_objects() const
    {
        return const_cast<Tuple>(this)->objects();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    Tuple<PtrContainer, Args...> Tuple<PtrContainer, Args...>::share()
    {
        return Tuple(detail::ShareTuple{}, objects);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    const Tuple<PtrContainer, Args...> Tuple<PtrContainer, Args...>::share() const
    {
        return Tuple(detail::ShareTuple{}, objects);
    }
}

namespace std
{
    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Args>
    auto &get(tefri::Tuple<PtrContainer, Args...> &tuple)
    {
        return tuple.template get<INDEX>();
    }

    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Args>
    const auto &get(const tefri::Tuple<PtrContainer, Args...> &tuple)
    {
        return tuple.template get<INDEX>();
    }
}

#endif // TEFRI_TUPLE_INC