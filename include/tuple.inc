#ifndef TEFRI_TUPLE_INC
#define TEFRI_TUPLE_INC

#include "tuple.h"

namespace tefri
{
    namespace detail
    {
        template <typename Tuple, typename Callable, std::size_t... INDICES>
        auto apply(Tuple tuple, Callable callable, std::index_sequence<INDICES...>)
        {
            return callable(tuple.template get<INDICES>()...);
        }
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...>::GeneralTuple()
    : objects
    (
        std::make_shared<Container>
        (
            std::initializer_list<std::shared_ptr<void>> 
            {
                std::static_pointer_cast<void>(std::make_shared<Seq>())...
            }
        )
    ),
    offset(0)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...>::GeneralTuple(Seq&&... args)
    : objects
    (
        std::make_shared<Container>
        (
            std::initializer_list<std::shared_ptr<void>> 
            {
                std::static_pointer_cast<void>(std::make_shared<Seq>(std::forward<Seq>(args)))...
            }
        )
    ),
    offset(0)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...>::GeneralTuple(const Seq &... args)
    : objects
    (
        std::make_shared<Container>
        (
            std::initializer_list<std::shared_ptr<void>> 
            {
                std::static_pointer_cast<void>(std::make_shared<Seq>(args))...
            }
        )
    ),
    offset(0)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...>::GeneralTuple(Seq*... args)
    : GeneralTuple(std::shared_ptr<Seq>(args)...)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...>::GeneralTuple(std::shared_ptr<Seq>... args)
    : objects
    (
        std::make_shared<Container>
        (
            std::initializer_list<std::shared_ptr<void>> 
            { 
                std::static_pointer_cast<void>(args)... 
            }
        )
    ),
    offset(0)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...>::GeneralTuple(const GeneralTuple &o)
    : objects(std::make_shared<Container>(*o.objects)), offset(o.offset)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...>::GeneralTuple(GeneralTuple &&o)
    : objects(std::move(o.objects)), offset(o.offset)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...>::~GeneralTuple()
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...>::GeneralTuple(Objects objects, std::size_t offset)
    : objects(objects), offset(offset)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...> &GeneralTuple<PtrContainer, Seq...>::operator=(const GeneralTuple &rhs)
    {
        if(this != &rhs)
            *this = std::move(GeneralTuple<PtrContainer, Seq...>(rhs));

        return *this;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...> &GeneralTuple<PtrContainer, Seq...>::operator=(GeneralTuple &&rhs)
    {
        objects = std::move(rhs.objects);
        return *this;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t INDEX>
    auto GeneralTuple<PtrContainer, Seq...>::get()
        -> typename TypeTuple::template Get<INDEX> &
    {
        return *get_ptr<INDEX>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t INDEX>
    auto GeneralTuple<PtrContainer, Seq...>::get() const
        -> std::add_const_t<typename TypeTuple::template Get<INDEX>> &
    {
        return const_cast<GeneralTuple*>(this)->template get<INDEX>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t INDEX>
    auto GeneralTuple<PtrContainer, Seq...>::get_ptr()
        -> std::shared_ptr<typename TypeTuple::template Get<INDEX>>
    {
        return std::static_pointer_cast<typename TypeTuple::template Get<INDEX>>(get_ptr(INDEX));        
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t INDEX>
    auto GeneralTuple<PtrContainer, Seq...>::get_ptr() const
        -> std::shared_ptr<std::add_const_t<typename TypeTuple::template Get<INDEX>>>
    {
        return const_cast<GeneralTuple>(this)->template get_ptr<INDEX>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    std::shared_ptr<void> GeneralTuple<PtrContainer, Seq...>::get_ptr(std::size_t index)
    {
        assert(index < objects->size());

        return *(objects->begin() + index + offset);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    std::shared_ptr<const void> GeneralTuple<PtrContainer, Seq...>::get_ptr(std::size_t index) const
    {
        return const_cast<GeneralTuple>(this)->get_ptr(index);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t FROM, std::size_t TO>
    auto GeneralTuple<PtrContainer, Seq...>::take_range() const 
        -> metaxxa::TakeRange
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            FROM, TO
        >
    {
        using Result = metaxxa::TakeRange
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            FROM, TO
        >;

        Objects objects_copy = std::make_shared<Container>(*objects);

        return Result(objects_copy, FROM); 
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t FROM, std::size_t TO>
    auto GeneralTuple<PtrContainer, Seq...>::take_range_shared() const 
        -> metaxxa::TakeRange
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            FROM, TO
        >
    {
        using Result = metaxxa::TakeRange
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            FROM, TO
        >;

        return Result(objects, FROM); 
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Seq...>::take_first() const 
        -> metaxxa::TakeFirst
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            N
        >
    {
        return take_range<0, N>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Seq...>::take_first_shared() const 
        -> metaxxa::TakeFirst
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            N
        >
    {
        return take_range_shared<0, N>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Seq...>::take_last() const 
        -> metaxxa::TakeLast
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            N
        >
    {
        return take_range<size() - N, size()>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Seq...>::take_last_shared() const 
        -> metaxxa::TakeLast
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            N
        >
    {
        return take_range_shared<size() - N, size()>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Seq...>::skip_first() const 
        -> metaxxa::SkipFirst
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            N
        >
    {
        return take_range<N, size()>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Seq...>::skip_first_shared() const 
        -> metaxxa::SkipFirst
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            N
        >
    {
        return take_range_shared<N, size()>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Seq...>::skip_last() const 
        -> metaxxa::SkipLast
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            N
        >
    {
        return take_range<0, size() - N>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Seq...>::skip_last_shared() const 
        -> metaxxa::SkipLast
        <
            DraftTuple<PtrContainer>::template Tuple,
            TypeTuple,
            N
        >
    {
        return take_range_shared<0, size() - N>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <typename T>
    GeneralTuple<PtrContainer, Seq..., T> GeneralTuple<PtrContainer, Seq...>::push_back(T &&obj)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<T>(std::forward<T>(obj))));
        return reinterpret<Seq..., T>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <typename T>
    GeneralTuple<PtrContainer, Seq..., T> GeneralTuple<PtrContainer, Seq...>::push_back(const T &obj)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<T>(obj)));
        return reinterpret<Seq..., T>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <typename T, typename... ArgsT>
    GeneralTuple<PtrContainer, Seq..., T> GeneralTuple<PtrContainer, Seq...>::emplace_back(ArgsT&&... args)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<T>(std::forward<Seq>(args)...)));
        return reinterpret<Seq..., T>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <typename T, typename... ArgsT>
    GeneralTuple<PtrContainer, Seq..., T> GeneralTuple<PtrContainer, Seq...>::emplace_back(const ArgsT &... args)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<T>(args...)));
        return reinterpret<Seq..., T>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    typename GeneralTuple<PtrContainer, Seq...>::Objects GeneralTuple<PtrContainer, Seq...>::raw_objects()
    {
        return objects;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    const typename GeneralTuple<PtrContainer, Seq...>::Objects GeneralTuple<PtrContainer, Seq...>::raw_objects() const
    {
        return const_cast<GeneralTuple>(this)->objects();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    GeneralTuple<PtrContainer, Seq...> GeneralTuple<PtrContainer, Seq...>::share()
    {
        return GeneralTuple(objects);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    const GeneralTuple<PtrContainer, Seq...> GeneralTuple<PtrContainer, Seq...>::share() const
    {
        return GeneralTuple(objects);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <typename... NewTypes>
    GeneralTuple<PtrContainer, NewTypes...> GeneralTuple<PtrContainer, Seq...>::reinterpret()
    {
        return GeneralTuple<PtrContainer, NewTypes...>(objects);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <typename... NewTypes>
    const GeneralTuple<PtrContainer, NewTypes...> GeneralTuple<PtrContainer, Seq...>::reinterpret() const
    {
        return const_cast<GeneralTuple>(this)->template reinterpret<NewTypes...>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <typename Callable>
    auto GeneralTuple<PtrContainer, Seq...>::apply(const Callable &callable) const
    {
        return detail::apply<const GeneralTuple<PtrContainer, Seq...> &, const Callable &>(*this, callable, std::make_index_sequence<size()>());
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <typename Callable>
    auto GeneralTuple<PtrContainer, Seq...>::apply(Callable &callable) const
    {
        return detail::apply<const GeneralTuple<PtrContainer, Seq...> &, Callable &>(*this, callable, std::make_index_sequence<size()>());
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <typename Callable>
    auto GeneralTuple<PtrContainer, Seq...>::apply(const Callable &callable)
    {
        return detail::apply<GeneralTuple<PtrContainer, Seq...> &, const Callable &>(*this, callable, std::make_index_sequence<size()>());
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Seq
    >
    template <typename Callable>
    auto GeneralTuple<PtrContainer, Seq...>::apply(Callable &callable)
    {
        return detail::apply<GeneralTuple<PtrContainer, Seq...> &, Callable &>(*this, callable, std::make_index_sequence<size()>());
    }

    // empty spec //
    
    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    GeneralTuple<PtrContainer>::GeneralTuple()
    : objects(std::make_shared<Container>()), offset(0)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    GeneralTuple<PtrContainer>::GeneralTuple(const GeneralTuple &o)
    : objects(std::make_shared<Container>()), offset(o.offset)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    GeneralTuple<PtrContainer>::GeneralTuple(GeneralTuple &&o)
    : objects(std::move(o.objects)), offset(o.offset)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    GeneralTuple<PtrContainer>::GeneralTuple(Objects objects, std::size_t offset)
    : objects(objects), offset(offset)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    GeneralTuple<PtrContainer>::~GeneralTuple()
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    GeneralTuple<PtrContainer> &GeneralTuple<PtrContainer>::operator=(const GeneralTuple &rhs)
    {
        return *this;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    GeneralTuple<PtrContainer> &GeneralTuple<PtrContainer>::operator=(GeneralTuple &&rhs)
    {
        objects = std::move(rhs.objects);
        return *this;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    template <typename NewObject>
    GeneralTuple<PtrContainer, NewObject> GeneralTuple<PtrContainer>::push_back(NewObject &&obj)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<NewObject>(std::forward<NewObject>(obj))));
        return reinterpret<NewObject>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    template <typename NewObject>
    GeneralTuple<PtrContainer, NewObject> GeneralTuple<PtrContainer>::push_back(const NewObject &obj)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<NewObject>(obj)));
        return reinterpret<NewObject>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    template <typename NewObject, typename... Seq>
    GeneralTuple<PtrContainer, NewObject> GeneralTuple<PtrContainer>::emplace_back(Seq&&... args)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<NewObject>(std::forward<Seq>(args)...)));
        return reinterpret<NewObject>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    template <typename NewObject, typename... Seq>
    GeneralTuple<PtrContainer, NewObject> GeneralTuple<PtrContainer>::emplace_back(const Seq &... args)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<NewObject>(args...)));
        return reinterpret<NewObject>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    typename GeneralTuple<PtrContainer>::Objects GeneralTuple<PtrContainer>::raw_objects()
    {
        return objects;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    const typename GeneralTuple<PtrContainer>::Objects GeneralTuple<PtrContainer>::raw_objects() const
    {
        return const_cast<GeneralTuple>(this)->objects();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    GeneralTuple<PtrContainer> GeneralTuple<PtrContainer>::share()
    {
        return GeneralTuple(objects);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    const GeneralTuple<PtrContainer> GeneralTuple<PtrContainer>::share() const
    {
        return const_cast<GeneralTuple>(this)->share();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    template <typename... NewTypes>
    GeneralTuple<PtrContainer, NewTypes...> GeneralTuple<PtrContainer>::reinterpret()
    {
        return GeneralTuple<PtrContainer, NewTypes...>(objects);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    template <typename... NewTypes>
    const GeneralTuple<PtrContainer, NewTypes...> GeneralTuple<PtrContainer>::reinterpret() const
    {
        return const_cast<GeneralTuple>(this)->template reinterpret<NewTypes...>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    template <typename Callable>
    auto GeneralTuple<PtrContainer>::apply(const Callable &callable) const
    {
        return callable();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    template <typename Callable>
    auto GeneralTuple<PtrContainer>::apply(Callable &callable) const
    {
        return callable();
    }

    // empty spec //
}

namespace std
{
    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Seq>
    auto &get(tefri::GeneralTuple<PtrContainer, Seq...> &tuple)
    {
        return tuple.template get<INDEX>();
    }

    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Seq>
    const auto &get(const tefri::GeneralTuple<PtrContainer, Seq...> &tuple)
    {
        return tuple.template get<INDEX>();
    }
}

#endif // TEFRI_TUPLE_INC