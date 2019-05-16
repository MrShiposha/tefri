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
    class GeneralTuple final : public metaxxa::TypeTuple<Types...>
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

        GeneralTuple();

        GeneralTuple(Types&&...);

        GeneralTuple(const Types &...);

        GeneralTuple(Types*...);

        GeneralTuple(std::shared_ptr<Types>...);

        GeneralTuple(const GeneralTuple &);

        GeneralTuple(GeneralTuple &&);

        ~GeneralTuple();

        GeneralTuple &operator=(const GeneralTuple &);

        GeneralTuple &operator=(GeneralTuple &&);

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
        GeneralTuple<PtrContainer, Types..., NewObject> push_back(NewObject &&);

        template <typename NewObject>
        GeneralTuple<PtrContainer, Types..., NewObject> push_back(const NewObject &);

        template <typename NewObject, typename... Args>
        GeneralTuple<PtrContainer, Types..., NewObject> emplace_back(Args&&...);

        template <typename NewObject, typename... Args>
        GeneralTuple<PtrContainer, Types..., NewObject> emplace_back(const Args &...);

        Objects raw_objects();

        const Objects raw_objects() const;

        GeneralTuple share();

        const GeneralTuple share() const;

        template <typename... NewTypes>
        GeneralTuple<PtrContainer, NewTypes...> reinterpret();

        template <typename... NewTypes>
        const GeneralTuple<PtrContainer, NewTypes...> reinterpret() const;

    private:
        template 
        <
            template <typename, typename...> typename, 
            typename...
        >
        friend class GeneralTuple;

        GeneralTuple(Objects, std::size_t offset = 0);

        Objects objects;
        std::size_t offset;
    };

    template 
    <
        template <typename, typename...> typename PtrContainer
    >
    class GeneralTuple<PtrContainer> final : public metaxxa::TypeTuple<>
    {
    public:
        using TypeTuple         = metaxxa::TypeTuple<>;
        
        template <typename T>
        using ContainerTemplate = PtrContainer<T>;
        using Container         = PtrContainer<std::shared_ptr<void>>;
        using Objects           = std::shared_ptr<Container>;

        GeneralTuple();

        GeneralTuple(const GeneralTuple &);

        GeneralTuple(GeneralTuple &&);

        ~GeneralTuple();

        GeneralTuple &operator=(const GeneralTuple &);

        GeneralTuple &operator=(GeneralTuple &&);

        template <typename NewObject>
        GeneralTuple<PtrContainer, NewObject> push_back(NewObject &&);

        template <typename NewObject>
        GeneralTuple<PtrContainer, NewObject> push_back(const NewObject &);

        template <typename NewObject, typename... Args>
        GeneralTuple<PtrContainer, NewObject> emplace_back(Args&&...);

        template <typename NewObject, typename... Args>
        GeneralTuple<PtrContainer, NewObject> emplace_back(const Args &...);

        Objects raw_objects();

        const Objects raw_objects() const;

        GeneralTuple share();

        const GeneralTuple share() const;

        template <typename... NewTypes>
        GeneralTuple<PtrContainer, NewTypes...> reinterpret();

        template <typename... NewTypes>
        const GeneralTuple<PtrContainer, NewTypes...> reinterpret() const;

    private:
        GeneralTuple(Objects, std::size_t offset = 0);

        Objects objects;
        std::size_t offset;
    };

    template <template <typename, typename...> typename PtrContainer>
    class DraftTuple
    {
    public:
        template <typename... Types>
        using Tuple = ::tefri::GeneralTuple<PtrContainer, Types...>;
    };

    template <typename... Types>
    using Tuple = typename DraftTuple<std::vector>::template Tuple<Types...>;
}

namespace std
{
    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Args>
    class tuple_element<INDEX, tefri::GeneralTuple<PtrContainer, Args...>>
    {
    public:
        using type = std::tuple_element_t<INDEX, metaxxa::TypeTuple<Args...>>;
    };

    template <template <typename, typename...> typename PtrContainer, typename... Args>
    class tuple_size<tefri::GeneralTuple<PtrContainer, Args...>> 
        : public std::integral_constant<std::size_t, sizeof...(Args)>
    {};

    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Args>
    auto &get(tefri::GeneralTuple<PtrContainer, Args...> &);

    template <std::size_t INDEX, template <typename, typename...> typename PtrContainer, typename... Args>
    const auto &get(const tefri::GeneralTuple<PtrContainer, Args...> &);
}

#endif // TEFRI_TUPLE_H