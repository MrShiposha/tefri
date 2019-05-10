#ifndef TEFRI_TUPLE_H
#define TEFRI_TUPLE_H

#include <memory>

#include "../lib/metaxxa/metaxxa.hpp"

namespace tefri
{
    namespace detail
    {
        struct ShareTuple {};
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Types
    >
    class Tuple final : public metaxxa::TypeTuple<Types...>
    {
    public:
        using TypeTuple         = metaxxa::TypeTuple<Types...>;
        
        template <typename T>
        using ContainerTemplate = PtrContainer<T>;
        using Container         = PtrContainer<std::shared_ptr<void>>;
        using Objects           = std::shared_ptr<Container>;

        using TypeTuple::is_empty;
        using TypeTuple::get_size;
        using TypeTuple::size;

        Tuple();

        Tuple(Types&&...);

        Tuple(const Types &...);

        Tuple(Types*...);

        Tuple(std::shared_ptr<Types>...);

        Tuple(const Tuple &);

        Tuple(Tuple &&);

        ~Tuple();

        Tuple &operator=(const Tuple &);

        Tuple &operator=(Tuple &&);

        template <std::size_t INDEX>
        auto get()
            -> typename TypeTuple::template Get<INDEX> &;

        template <std::size_t INDEX>
        auto get() const
            -> std::add_const_t<typename TypeTuple::template Get<INDEX>> &;

        template <std::size_t INDEX>
        auto get_ptr()
            -> std::shared_ptr<typename TypeTuple::template Get<INDEX>>;

        template <std::size_t INDEX>
        auto get_ptr() const
            -> std::shared_ptr<std::add_const_t<typename TypeTuple::template Get<INDEX>>>;

        std::shared_ptr<void> get_ptr(std::size_t index);

        std::shared_ptr<const void> get_ptr(std::size_t index) const;

        template <typename NewObject>
        Tuple<PtrContainer, Types..., NewObject> push_back(NewObject &&);

        template <typename NewObject>
        Tuple<PtrContainer, Types..., NewObject> push_back(const NewObject &);

        template <typename NewObject, typename... Args>
        Tuple<PtrContainer, Types..., NewObject> emplace_back(Args&&...);

        template <typename NewObject, typename... Args>
        Tuple<PtrContainer, Types..., NewObject> emplace_back(const Args &...);

        Objects raw_objects();

        const Objects raw_objects() const;

        Tuple share();

        const Tuple share() const;

        template <typename... NewTypes>
        Tuple<PtrContainer, NewTypes...> reinterpret();

        template <typename... NewTypes>
        const Tuple<PtrContainer, NewTypes...> reinterpret() const;

    private:
        template 
        <
            template <typename, typename...> typename, 
            typename...
        >
        friend class Tuple;

        Tuple(Objects);

        Objects objects;
    };

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    class Tuple<PtrContainer> final : public metaxxa::TypeTuple<>
    {
    public:
        using TypeTuple         = metaxxa::TypeTuple<>;
        
        template <typename T>
        using ContainerTemplate = PtrContainer<T>;
        using Container         = PtrContainer<std::shared_ptr<void>>;
        using Objects           = std::shared_ptr<Container>;

        Tuple();

        Tuple(const Tuple &);

        Tuple(Tuple &&);

        ~Tuple();

        Tuple &operator=(const Tuple &);

        Tuple &operator=(Tuple &&);

        template <typename NewObject>
        Tuple<PtrContainer, NewObject> push_back(NewObject &&);

        template <typename NewObject>
        Tuple<PtrContainer, NewObject> push_back(const NewObject &);

        template <typename NewObject, typename... Args>
        Tuple<PtrContainer, NewObject> emplace_back(Args&&...);

        template <typename NewObject, typename... Args>
        Tuple<PtrContainer, NewObject> emplace_back(const Args &...);

        Objects raw_objects();

        const Objects raw_objects() const;

        Tuple share();

        const Tuple share() const;

        template <typename... NewTypes>
        Tuple<PtrContainer, NewTypes...> reinterpret();

        template <typename... NewTypes>
        const Tuple<PtrContainer, NewTypes...> reinterpret() const;

    private:
        Tuple(Objects);

        Objects objects;
    };

    template <template <typename, typename...> typename PtrContainer>
    class DraftTuple
    {
    public:
        template <typename... Types>
        using Tuple = ::tefri::Tuple<PtrContainer, Types...>;
    };
}

namespace std
{
    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Args>
    class tuple_element<INDEX, tefri::Tuple<PtrContainer, Args...>>
    {
    public:
        using type = std::tuple_element_t<INDEX, metaxxa::TypeTuple<Args...>>;
    };

    template <template <typename, typename...> typename PtrContainer, typename... Args>
    class tuple_size<tefri::Tuple<PtrContainer, Args...>> 
        : public std::integral_constant<std::size_t, sizeof...(Args)>
    {};

    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Args>
    auto &get(tefri::Tuple<PtrContainer, Args...> &);

    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Args>
    const auto &get(const tefri::Tuple<PtrContainer, Args...> &);
}

#endif // TEFRI_TUPLE_H