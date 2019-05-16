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
    GeneralTuple<PtrContainer, Args...>::GeneralTuple()
    : objects
    (
        std::make_shared<Container>
        (
            std::initializer_list<std::shared_ptr<void>> 
            {
                std::static_pointer_cast<void>(std::make_shared<Args>())...
            }
        )
    ),
    offset(0)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...>::GeneralTuple(Args&&... args)
    : objects
    (
        std::make_shared<Container>
        (
            std::initializer_list<std::shared_ptr<void>> 
            {
                std::static_pointer_cast<void>(std::make_shared<Args>(std::forward<Args>(args)))...
            }
        )
    ),
    offset(0)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...>::GeneralTuple(const Args &... args)
    : objects
    (
        std::make_shared<Container>
        (
            std::initializer_list<std::shared_ptr<void>> 
            {
                std::static_pointer_cast<void>(std::make_shared<Args>(args))...
            }
        )
    ),
    offset(0)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...>::GeneralTuple(Args*... args)
    : GeneralTuple(std::shared_ptr<Args>(args)...)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...>::GeneralTuple(std::shared_ptr<Args>... args)
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
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...>::GeneralTuple(const GeneralTuple &o)
    : objects(std::make_shared<Container>(*o.objects)), offset(o.offset)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...>::GeneralTuple(GeneralTuple &&o)
    : objects(std::move(o.objects)), offset(o.offset)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...>::~GeneralTuple()
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...>::GeneralTuple(Objects objects, std::size_t offset)
    : objects(objects), offset(offset)
    {}

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...> &GeneralTuple<PtrContainer, Args...>::operator=(const GeneralTuple &rhs)
    {
        if(this != &rhs)
            *this = std::move(GeneralTuple<PtrContainer, Args...>(rhs));

        return *this;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...> &GeneralTuple<PtrContainer, Args...>::operator=(GeneralTuple &&rhs)
    {
        objects = std::move(rhs.objects);
        return *this;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <std::size_t INDEX>
    auto GeneralTuple<PtrContainer, Args...>::get()
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
    auto GeneralTuple<PtrContainer, Args...>::get() const
        -> std::add_const_t<typename TypeTuple::template Get<INDEX>> &
    {
        return const_cast<GeneralTuple>(this)->template get<INDEX>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <std::size_t INDEX>
    auto GeneralTuple<PtrContainer, Args...>::get_ptr()
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
    auto GeneralTuple<PtrContainer, Args...>::get_ptr() const
        -> std::shared_ptr<std::add_const_t<typename TypeTuple::template Get<INDEX>>>
    {
        return const_cast<GeneralTuple>(this)->template get_ptr<INDEX>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    std::shared_ptr<void> GeneralTuple<PtrContainer, Args...>::get_ptr(std::size_t index)
    {
        assert(index < objects->size());

        return *(objects->begin() + index + offset);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    std::shared_ptr<const void> GeneralTuple<PtrContainer, Args...>::get_ptr(std::size_t index) const
    {
        return const_cast<GeneralTuple>(this)->get_ptr(index);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <typename T>
    GeneralTuple<PtrContainer, Args..., T> GeneralTuple<PtrContainer, Args...>::push_back(T &&obj)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<T>(std::forward<T>(obj))));
        return reinterpret<Args..., T>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <typename T>
    GeneralTuple<PtrContainer, Args..., T> GeneralTuple<PtrContainer, Args...>::push_back(const T &obj)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<T>(obj)));
        return reinterpret<Args..., T>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <typename T, typename... ArgsT>
    GeneralTuple<PtrContainer, Args..., T> GeneralTuple<PtrContainer, Args...>::emplace_back(ArgsT&&... args)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<T>(std::forward<Args>(args)...)));
        return reinterpret<Args..., T>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <typename T, typename... ArgsT>
    GeneralTuple<PtrContainer, Args..., T> GeneralTuple<PtrContainer, Args...>::emplace_back(const ArgsT &... args)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<T>(args...)));
        return reinterpret<Args..., T>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    typename GeneralTuple<PtrContainer, Args...>::Objects GeneralTuple<PtrContainer, Args...>::raw_objects()
    {
        return objects;
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    const typename GeneralTuple<PtrContainer, Args...>::Objects GeneralTuple<PtrContainer, Args...>::raw_objects() const
    {
        return const_cast<GeneralTuple>(this)->objects();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    GeneralTuple<PtrContainer, Args...> GeneralTuple<PtrContainer, Args...>::share()
    {
        return GeneralTuple(objects);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    const GeneralTuple<PtrContainer, Args...> GeneralTuple<PtrContainer, Args...>::share() const
    {
        return GeneralTuple(objects);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <typename... NewTypes>
    GeneralTuple<PtrContainer, NewTypes...> GeneralTuple<PtrContainer, Args...>::reinterpret()
    {
        return GeneralTuple<PtrContainer, NewTypes...>(objects);
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <typename... NewTypes>
    const GeneralTuple<PtrContainer, NewTypes...> GeneralTuple<PtrContainer, Args...>::reinterpret() const
    {
        return const_cast<GeneralTuple>(this)->template reinterpret<NewTypes...>();
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
    template <typename NewObject, typename... Args>
    GeneralTuple<PtrContainer, NewObject> GeneralTuple<PtrContainer>::emplace_back(Args&&... args)
    {
        objects->push_back(std::static_pointer_cast<void>(std::make_shared<NewObject>(std::forward<Args>(args)...)));
        return reinterpret<NewObject>();
    }

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    template <typename NewObject, typename... Args>
    GeneralTuple<PtrContainer, NewObject> GeneralTuple<PtrContainer>::emplace_back(const Args &... args)
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

    // empty spec //
}

namespace std
{
    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Args>
    auto &get(tefri::GeneralTuple<PtrContainer, Args...> &tuple)
    {
        return tuple.template get<INDEX>();
    }

    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Args>
    const auto &get(const tefri::GeneralTuple<PtrContainer, Args...> &tuple)
    {
        return tuple.template get<INDEX>();
    }
}

#endif // TEFRI_TUPLE_INC