// MIT License
// 
// Copyright (c) 2018 Daniel Shiposha
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 

#ifndef TEFRI_HPP
#define TEFRI_HPP

#include <memory>
#include <future>
#include <chrono>
#include <type_traits>
#include <utility>
#include <functional>
#include <atomic>
#include <tuple>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <string>
#include <typeinfo>

#ifndef TEFRI_H
#define TEFRI_H


#ifndef TEFRI_DEF_H
#define TEFRI_DEF_H

#ifdef _MSC_VER
    #define tefri_inline __forceinline
#elif defined(__GNUC__)
    #define tefri_inline inline __attribute__((__always_inline__))
#elif defined(__CLANG__)
    #if __has_attribute(__always_inline__)
        #define tefri_inline inline __attribute__((__always_inline__))
    #else
        #define tefri_inline inline
    #endif
#else
    #define tefri_inline inline
#endif

#endif // TEFRI_DEF_H

#ifndef TEFRI_OBJECTHOLDER_INC
#define TEFRI_OBJECTHOLDER_INC


#ifndef TEFRI_DETAIL_OBJECTHOLDER_H
#define TEFRI_DETAIL_OBJECTHOLDER_H



// MIT License
// 
// Copyright (c) 2018 Daniel Shiposha
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 

#ifndef METAXXA_HPP
#define METAXXA_HPP


#ifndef METAXXA_DEF_H
#define METAXXA_DEF_H

#ifdef _MSC_VER
    #define metaxxa_inline __forceinline
#elif defined(__GNUC__)
    #define metaxxa_inline inline __attribute__((__always_inline__))
#elif defined(__CLANG__)
    #if __has_attribute(__always_inline__)
        #define metaxxa_inline inline __attribute__((__always_inline__))
    #else
        #define metaxxa_inline inline
    #endif
#else
    #define metaxxa_inline inline
#endif

#endif // METAXXA_DEF_H

#ifndef METAXXA_ISVALID_H
#define METAXXA_ISVALID_H



#ifndef METAXXA_TYPESAVER_H
#define METAXXA_TYPESAVER_H

namespace metaxxa
{
    template <typename T>
    struct TypeSaver
    {
        using Type = T;
    };
}

#endif // METAXXA_TYPESAVER_H

namespace metaxxa
{
    namespace detail
    {
        template <typename... Args>
        class IsValid
        {
            template <typename Callable>
            static constexpr auto check(...) 
                -> std::false_type;

            template <typename Callable>
            static constexpr auto check(TypeSaver<decltype(std::declval<Callable&&>()(std::declval<Args&&>()...))> *)
                -> std::true_type;

        public:
            template <typename Callable>
            constexpr bool operator()(Callable &&) const
            {
                return std::is_same_v<decltype(check<Callable>(0)), std::true_type>;
            }
        };
    }

    template <typename... Types>
    constexpr detail::IsValid<Types...> is_valid {}; 
}

#endif // METAXXA_ISVALID_H

#ifndef METAXXA_TYPELIST_H
#define METAXXA_TYPELIST_H


namespace metaxxa
{
    namespace detail
    {
        class ListTag {};
    }

    template <typename... Args>
    class TypeList;

    using Nil = TypeList<>;

    template <typename H = Nil, typename... Tail>
    struct CarT
    {
        using Head = H;
    };

    template <typename H = Nil, typename... Args>
    struct CdrT
    {
        using Tail = TypeList<Args...>;
    };

    template <typename T>
    struct CdrT<T>
    {
        using Tail = Nil;
    };

    template <typename... Args>
    using Car = typename CarT<Args...>::Head;

    template <typename... Args>
    using Cdr = typename CdrT<Args...>::Tail;

    template <typename... Args>
    class TypeList : public detail::ListTag
    {
    public:
        constexpr TypeList() = default;

        using Head = Car<Args...>;
        using Tail = Cdr<Args...>;
    };
}

namespace std
{
    template <typename... Args>
    class tuple_size<metaxxa::TypeList<Args...>>
        : public std::integral_constant<std::size_t, sizeof...(Args)>
    {};

    template <>
    class tuple_size<metaxxa::TypeList<>>
        : public std::integral_constant<std::size_t, 0>
    {};

    template <size_t INDEX, typename... Args>
	class tuple_element<INDEX, metaxxa::TypeList<Args...>>
        : public tuple_element<INDEX - 1, typename metaxxa::TypeList<Args...>::Tail>
	{};

    template <size_t INDEX>
	class tuple_element<INDEX, metaxxa::Nil>
    {

    };

    template <>
	class tuple_element<0, metaxxa::Nil>
    {};

    template <typename... Args>
	class tuple_element<0, metaxxa::TypeList<Args...>>
	{
	public:
		using type = typename metaxxa::TypeList<Args...>::Head;
	};
}

#endif // METAXXA_TYPELIST_H

#ifndef METAXXA_LIETRALLIST_H
#define METAXXA_LIETRALLIST_H



#ifndef METAXXA_LITERAL_H
#define METAXXA_LITERAL_H

namespace metaxxa
{
    template <typename T, T LITERAL>
    struct Literal
    {
        using Type = T;

        static constexpr Type value = LITERAL;
    };

}

#endif // METAXXA_LITERAL_H

namespace metaxxa
{
    namespace detail
    {
        struct LiteralListTag
        {};
    }

    struct LiteralNilT
    {};

    inline static constexpr LiteralNilT NIL {};

    template <typename T = LiteralNilT, T... LITERALS>
    struct LiteralList;

    using LiteralNil = LiteralList<>;

    template <typename LiteralH = LiteralNil, typename... LiteralTail>
    struct LiteralCarT
    {
        using Head = LiteralH;
    };

    template <typename LiteralH = LiteralNil, typename... LiteralTail>
    struct LiteralCdrT
    {
        using Tail = LiteralList<typename LiteralH::Type, LiteralTail::value...>;
    };

    template <typename LiteralT>
    struct LiteralCdrT<LiteralT>
    {
        using Tail = LiteralNil;
    };

    template <typename... Literals>
    using LiteralCar = typename LiteralCarT<Literals...>::Head;

    template <typename... Literals>
    using LiteralCdr = typename LiteralCdrT<Literals...>::Tail;

    template <typename T, T... LITERALS>
    struct LiteralList : public detail::LiteralListTag
    {
        using Head = LiteralCar<Literal<T, LITERALS>...>;
        using Tail = LiteralCdr<Literal<T, LITERALS>...>;
        using IntegerSequence = std::integer_sequence<T, LITERALS...>;

        using HeadType = typename Head::Type;

        static constexpr HeadType head()
        {
            return Head::value;
        }
    };

    template <typename T>
    struct LiteralList<T> : public detail::LiteralListTag
    {
        using Head = LiteralNil;
        using Tail = LiteralNil;
        using IntegerSequence = std::integer_sequence<T>;

        using HeadType = LiteralNilT;

        static constexpr HeadType head()
        {
            return NIL;
        }
    };

    namespace detail
    {
        template 
        <
            template <typename T, T...> typename SequenceT,
            typename LiteralT, 
            LiteralT... LITERALS
        >
        constexpr auto literal_list_from_sequence(SequenceT<LiteralT, LITERALS...>)
            -> LiteralList<LiteralT, LITERALS...>;
    }

    template <typename Sequence>
    using LiteralListFromSequence = decltype(detail::literal_list_from_sequence(std::declval<Sequence>()));
}

namespace std
{
    template <typename T, T... LITERALS>
    class tuple_size<metaxxa::LiteralList<T, LITERALS...>>
        : public std::integral_constant<std::size_t, sizeof...(LITERALS)>
    {};

    template <typename T>
    class tuple_size<metaxxa::LiteralList<T>>
        : public std::integral_constant<std::size_t, 0>
    {};

    template <size_t INDEX, typename T, T... LITERALS>
	class tuple_element<INDEX, metaxxa::LiteralList<T, LITERALS...>>
        : public tuple_element<INDEX - 1, typename metaxxa::LiteralList<T, LITERALS...>::Tail>
	{};

    template <size_t INDEX>
	class tuple_element<INDEX, metaxxa::LiteralNil>
    {};

    template <>
	class tuple_element<0, metaxxa::LiteralNil>
    {};

    template <typename T, T... LITERALS>
	class tuple_element<0, metaxxa::LiteralList<T, LITERALS...>>
	{
	public:
		using type = typename metaxxa::LiteralList<T, LITERALS...>::Head;
	};
}

template <typename T>
constexpr bool operator==(const T, const metaxxa::LiteralNilT)
{
    return false;
}

template <typename T>
constexpr bool operator==(const metaxxa::LiteralNilT, const T)
{
    return false;
}

constexpr bool operator==(const metaxxa::LiteralNilT, const metaxxa::LiteralNilT)
{
    return true;
}

#endif // METAXXA_LIETRALLIST_H


#ifndef METAXXA_TYPETUPLE_H
#define METAXXA_TYPETUPLE_H


#ifndef METAXXA_CONCAT_H
#define METAXXA_CONCAT_H


#ifndef METAXXA_MOVEPARAMETERS_H
#define METAXXA_MOVEPARAMETERS_H


namespace metaxxa
{
    namespace detail
    {
        template <typename... StartTypes>
        struct Mover
        {
            template
            <
                template <typename...> typename DestTemplate,
                template <typename...> typename SrcTemplate,
                typename... Args
            >
            static constexpr auto move_parameters(SrcTemplate<Args...> &&) -> TypeSaver<DestTemplate<StartTypes..., Args...>>;
        };
    }

    template 
    <
        template <typename...> typename DestTemplate,
        typename SrcTemplate,
        typename... StartTypes
    >
    using MoveParameters = typename decltype
    (
        detail::Mover<StartTypes...>::template move_parameters<DestTemplate>(std::declval<SrcTemplate>())
    )::Type;
}

#endif // METAXXA_MOVEPARAMETERS_H

#ifndef METAXXA_TEMPLATECONTAINER_H
#define METAXXA_TEMPLATECONTAINER_H

namespace metaxxa
{
    template <typename... Args>
    struct TemplateContainer 
    {};
}

#endif // METAXXA_TEMPLATECONTAINER_H

namespace metaxxa
{
    namespace detail
    {
        template
        <
            typename RHS,
            typename... LHSArgs
        >
        struct Concatenator
        {
            template <typename... RHSArgs>
            static constexpr auto evaltype(TemplateContainer<RHSArgs...> &&)
                -> TemplateContainer<LHSArgs..., RHSArgs...>;

            using RHSTypeList = MoveParameters<TemplateContainer, RHS>;
            using Type = decltype(evaltype(std::declval<RHSTypeList>()));
        };

        template
        <
            typename LHS,
            typename RHS,
            bool IS_RHS_LIST = std::is_base_of_v<ListTag, RHS>
        >
        struct ConcatenatorImpl
        {
            template <typename... LHSArgs>
            static constexpr auto evaltype(TemplateContainer<LHSArgs...> &&)
                -> Concatenator<RHS, LHSArgs...>;

            using LHSTypeList = MoveParameters<TemplateContainer, LHS>;
            using Type = typename decltype(evaltype(std::declval<LHSTypeList>()))::Type;
        };

        template <typename LHS, bool IS_LIST = std::is_base_of_v<ListTag, LHS>>
        struct ResolveEndType
        {
            using Type = MoveParameters<TemplateContainer, LHS>;
        };

        template <typename LHS>
        struct ResolveEndType<LHS, false>
        {
            using Type = LHS;
        };

        template
        <
            typename LHS
        >
        struct ConcatenatorImpl<LHS, TypeList<>, true>
        {
            using Type = typename ResolveEndType<LHS>::Type;
        };

        template
        <
            typename LHS,
            typename RHS
        >
        struct ConcatenatorImpl<LHS, RHS, true>
        {
            using Type = typename ConcatenatorImpl
            <
                LHS,
                typename ConcatenatorImpl
                <
                    typename RHS::Head,
                    typename RHS::Tail
                >::Type
            >::Type;
        };

        template
        <
            template <typename...> typename Template,
            typename... Templates
        >
        struct ConcatenatorFacade
        {
            using List = TypeList<Templates...>;

            using Type = MoveParameters
            <
                Template,
                typename ConcatenatorImpl
                <
                    typename List::Head,
                    typename List::Tail
                >::Type
            >;
        };
    }

    template
    <
        template <typename...> typename Template,
        typename... Templates
    >
    using Concat = typename detail::ConcatenatorFacade<Template, Templates...>::Type;
}

#endif // METAXXA_CONCAT_H

namespace metaxxa
{
    template <typename... Args>
    class TypeTuple
    {
    public:
        using List = metaxxa::TypeList<Args...>;

        template <std::size_t INDEX>
        using Get = typename std::tuple_element_t<INDEX, List>;

        template <typename... RHS>
        using Concat = Concat<metaxxa::TypeTuple, TypeTuple, RHS...>;

        constexpr TypeTuple() = default;

        constexpr TypeTuple(const TypeTuple &) = default;

        constexpr TypeTuple(TypeTuple &&) = default;

        virtual ~TypeTuple() = default;

        TypeTuple &operator=(const TypeTuple &) = default;

        TypeTuple &operator=(TypeTuple &&) = default;

        static constexpr bool is_empty();

        static constexpr std::size_t get_size();

        static constexpr std::size_t size();
    };
}

namespace std
{
    template <typename... Args>
    class tuple_size<metaxxa::TypeTuple<Args...>>
        : public std::integral_constant<std::size_t, sizeof...(Args)>
    {};

    template <>
    class tuple_size<metaxxa::TypeTuple<>>
        : public std::integral_constant<std::size_t, 0>
    {};

    template <size_t INDEX, typename... Args>
	class tuple_element<INDEX, metaxxa::TypeTuple<Args...>>
	{
	public:
		using type = tuple_element_t<INDEX, typename metaxxa::TypeTuple<Args...>::List>;
	};
}

#endif // METAXXA_TYPETUPLE_H


namespace metaxxa
{
    template <typename... Args>
    constexpr bool TypeTuple<Args...>::is_empty()
    {
        return std::is_same_v<Car<Args...>, Nil>;
    }

    template <typename... Args>
    constexpr std::size_t TypeTuple<Args...>::get_size()
    {
        return std::tuple_size_v<TypeTuple<Args...>>;
    }

    template <typename... Args>
    constexpr std::size_t TypeTuple<Args...>::size()
    {
        return get_size();
    }
}


#ifndef METAXXA_IF_H
#define METAXXA_IF_H


namespace metaxxa
{
    namespace detail
    {
        template <bool CONDITION, typename Then, typename Else>
        struct ElseResolver
        {
            using Type = Then;
        };

        template <typename Then, typename Else>
        struct ElseResolver<false, Then, Else>
        {
            using Type = Else;
        };

        template <bool CONDITION, typename T>
        struct ThenResolver
        {
            template <typename E>
            using Else = ElseResolver<CONDITION, T, E>;
        };

        template <typename T>
        struct ThenResolver<true, T>
        {
            using Type = T;

            template <typename E>
            using Else = ElseResolver<true, T, E>;
        };
    }

    template <bool CONDITION>
    struct If
    {
        template <typename T>
        using Then = detail::ThenResolver<CONDITION, T>;
    };
}

#endif // METAXXA_IF_H

#ifndef METAXXA_TYPEIF_H
#define METAXXA_TYPEIF_H



namespace metaxxa
{
    template <typename T>
    using TypeIf = If<!std::is_same_v<T, std::false_type>>;
}

#endif // METAXXA_TYPEIF_H

#ifndef METAXXA_TYPEORREF_H
#define METAXXA_TYPEORREF_H



namespace metaxxa
{
    template <typename T>
    using TypeOrRef = typename 
        If<sizeof(T) <= sizeof(T *)>
                ::template Then<T>
                ::template Else<T &>
                ::Type;

    template <typename T>
    using TypeOrRefWrapper = typename 
        If<sizeof(T) <= sizeof(T *)>
                ::template Then<T>
                ::template Else<std::reference_wrapper<T>>
                ::Type;

    template <typename T>
    using TypeOrRefConstWrapper = typename 
        If<sizeof(T) <= sizeof(T *)>
                ::template Then<T>
                ::template Else<std::reference_wrapper<const T>>
                ::Type;

    template <typename T>
    auto obj_or_ref(T &obj)
    {
        return TypeOrRefWrapper<T>(obj);
    }

    template <typename T>
    auto obj_or_cref(const T &obj)
    {
        return TypeOrRefConstWrapper<T>(obj);
    }
}

#endif // METAXXA_TYPEORREF_H


#ifndef METAXXA_TIMES_H
#define METAXXA_TIMES_H

namespace metaxxa
{
    namespace detail
    {
        template
        <
            template <typename...> typename Template,
            std::size_t N,
            typename T,
            typename... Ts
        >
        struct Times : Times<Template, N - 1, T, T, Ts...>
        {};

        template
        <
            template <typename...> typename Template,
            typename T,
            typename... Ts
        >
        struct Times<Template, 0, T, Ts...>
        {
            using Type = Template<Ts...>;
        };
    }

    template
    <
        template <typename...> typename Template,
        std::size_t N,
        typename T
    >
    using Times = typename detail::Times<Template, N, T>::Type;
}

#endif // METAXXA_TIMES_H

#ifndef METAXXA_PARAMETERSCOUNT_H
#define METAXXA_PARAMETERSCOUNT_H



#ifndef METAXXA_SIZECONSTANT_H
#define METAXXA_SIZECONSTANT_H


namespace metaxxa
{
    template <std::size_t INDEX>
    using SizeConstant = std::integral_constant<std::size_t, INDEX>;
}

#endif // METAXXA_SIZECONSTANT_H

namespace metaxxa
{
    namespace detail
    {
        template
        <
            template <typename...> typename Template,
            typename... Args
        >
        constexpr auto parameters_count(Template<Args...> &&)
            -> SizeConstant<sizeof...(Args)>;
    }

    // Holds number of template parameters of a class template
    template <typename T>
    using ParametersCount = decltype(detail::parameters_count(std::declval<T>()));

    template <typename T>
    constexpr std::size_t parameters_count() 
    {
        return ParametersCount<T>::value;
    }
}

#endif // METAXXA_PARAMETERSCOUNT_H


#ifndef METAXXA_INDEXRANGE_H
#define METAXXA_INDEXRANGE_H


namespace metaxxa
{
    namespace detail
    {
        template <std::size_t TO_ADD, std::size_t... SEQ>
        constexpr auto shift_seq_plus(std::index_sequence<SEQ...> &&)
            -> std::index_sequence<TO_ADD + SEQ ...>;

        template <std::size_t TO_ADD, std::size_t... SEQ>
        constexpr auto shift_seq_minus(std::index_sequence<SEQ...> &&)
            -> std::index_sequence<TO_ADD - SEQ ...>;
    }

    template <std::size_t MIN, std::size_t MAX>
    using MakeIndexRange = decltype(detail::shift_seq_plus<MIN>(std::declval<std::make_index_sequence<MAX-MIN>>()));

    template <std::size_t MAX, std::size_t MIN>
    using MakeReverseIndexRange = decltype(detail::shift_seq_minus<MAX - 1U>(std::declval<std::make_index_sequence<MAX-MIN>>()));
}

#endif // METAXXA_INDEXRANGE_H


#ifndef METAXXA_ALGORITHM_H
#define METAXXA_ALGORITHM_H


#ifndef METAXXA_ALGORITHM_ONLYINDICES_H
#define METAXXA_ALGORITHM_ONLYINDICES_H


namespace metaxxa
{
    template 
    <
        template <typename...> typename Template, 
        typename TupleT, 
        std::size_t... INDICES
    >
    using OnlyIndices = Template<std::tuple_element_t<INDICES, TupleT>...>;
}

#endif // METAXXA_ALGORITHM_ONLYINDICES_H

#ifndef METAXXA_ALGORITHM_SEQFILTER_H
#define METAXXA_ALGORITHM_SEQFILTER_H


namespace metaxxa
{
    namespace detail
    {
        template
        <
            template <typename...> typename Template,
            typename TupleT,
            std::size_t... INDICES
        >
        constexpr auto seq_filter(std::index_sequence<INDICES...> &&)
            -> OnlyIndices<Template, TupleT, INDICES...>;
    }

    template
    <
        template <typename...> typename Template,
        typename TupleT,
        typename Seq
    >
    using SeqFilter = decltype(detail::seq_filter<Template, TupleT>(std::declval<Seq>()));
}

#endif // METAXXA_ALGORITHM_SEQFILTER_H


#ifndef METAXXA_ALGORITHM_TAKERANGE_H
#define METAXXA_ALGORITHM_TAKERANGE_H



namespace metaxxa
{
    template
    <
        template <typename...> typename Template,
        typename TupleT,
        std::size_t FROM_I,
        std::size_t TO_I
    >
    using TakeRange = SeqFilter
    <
        Template,
        TupleT,
        MakeIndexRange<FROM_I, TO_I>
    >;
}

#endif // METAXXA_ALGORITHM_TAKERANGE_H

#ifndef METAXXA_ALGORITHM_TAKEFIRST_H
#define METAXXA_ALGORITHM_TAKEFIRST_H


namespace metaxxa
{
    template
    <
        template <typename...> typename Template,
        typename TupleT,
        std::size_t N
    >
    using TakeFirst = TakeRange
    <
        Template,
        TupleT,
        0, N
    >;
}

#endif // METAXXA_ALGORITHM_TAKEFIRST_H

#ifndef METAXXA_ALGORITHM_TAKELAST_H
#define METAXXA_ALGORITHM_TAKELAST_H


namespace metaxxa
{
    template
    <
        template <typename...> typename Template,
        typename TupleT,
        std::size_t N
    >
    using TakeLast = TakeRange
    <
        Template,
        TupleT,
        std::tuple_size_v<TupleT> - N, std::tuple_size_v<TupleT>
    >;
}

#endif // METAXXA_ALGORITHM_TAKELAST_H

#ifndef METAXXA_ALGORITHM_SKIPFIRST_H
#define METAXXA_ALGORITHM_SKIPFIRST_H


namespace metaxxa
{
    template 
    <
        template <typename...> typename Template,
        typename TupleT, 
        std::size_t N
    >
    using SkipFirst = TakeRange
    <
        Template,
        TupleT,
        N, std::tuple_size_v<TupleT>
    >;
}

#endif // METAXXA_ALGORITHM_SKIPFIRST_H

#ifndef METAXXA_ALGORITHM_SKIPLAST_H
#define METAXXA_ALGORITHM_SKIPLAST_H


namespace metaxxa
{
    template
    <
        template <typename...> typename Template,
        typename TupleT,
        std::size_t N
    >
    using SkipLast = TakeRange
    <
        Template,
        TupleT,
        0, std::tuple_size_v<TupleT> - N
    >;
}

#endif // METAXXA_ALGORITHM_SKIPLAST_H

#ifndef METAXXA_ALGORITHM_FIND_H
#define METAXXA_ALGORITHM_FIND_H


#define EXPAND(...) __VA_ARGS__

#define DEFINE_FIND_ALGORITHM(Variant, ...) \
    template                                                    \
    <                                                           \
        typename TupleT,                                        \
        EXPAND(FUNCTOR_TYPE),                                   \
        std::size_t N   = 0,                                    \
        bool PREV_FOUND = false,                                \
        bool ENOUGH     = (N >= std::tuple_size_v<TupleT>)      \
    >                                                           \
    struct Variant##Find : Variant##Find                        \
    <                                                           \
        TupleT,                                                 \
        Functor,                                                \
        N + 1,                                                  \
        Functor<__VA_ARGS__>::value,                            \
        N + 1 >= std::tuple_size_v<TupleT>                      \
    >                                                           \
    {};                                                         \
                                                                \
    template                                                    \
    <                                                           \
        typename TupleT,                                        \
        EXPAND(FUNCTOR_TYPE),                                   \
        std::size_t N,                                          \
        bool ENOUGH                                             \
    >                                                           \
    struct Variant##Find                                        \
    <                                                           \
        TupleT,                                                 \
        Functor,                                                \
        N,                                                      \
        true,                                                   \
        ENOUGH                                                  \
    >                                                           \
    {                                                           \
        static constexpr bool FOUND = true;                     \
        static constexpr std::size_t INDEX = N - 1;             \
                                                                \
        using Type = std::tuple_element_t<INDEX, TupleT>;       \
                                                                \
        template <typename T>                                   \
        using TypeOr = Type;                                    \
    };                                                          \
                                                                \
    template                                                    \
    <                                                           \
        typename TupleT,                                        \
        EXPAND(FUNCTOR_TYPE),                                   \
        std::size_t N                                           \
    >                                                           \
    struct Variant##Find                                        \
    <                                                           \
        TupleT,                                                 \
        Functor,                                                \
        N,                                                      \
        false,                                                  \
        true                                                    \
    >                                                           \
    {                                                           \
        static constexpr bool FOUND = false;                    \
                                                                \
        template <typename T>                                   \
        using TypeOr = T;                                       \
    }                                                          

namespace metaxxa
{
    namespace detail
    {
        template <typename E>
        struct IsEqualTo
        {
            template <typename T>
            struct Type
            {
                static constexpr bool value = std::is_same_v<T, E>;
            };
        };

        #define FUNCTOR_TYPE template <typename T> typename Functor

        DEFINE_FIND_ALGORITHM(, std::tuple_element_t<N, TupleT>);

        #undef FUNCTOR_TYPE
        #define FUNCTOR_TYPE template <typename T, std::size_t INDEX> typename Functor

        DEFINE_FIND_ALGORITHM(Index, std::tuple_element_t<N, TupleT>, N);

        #undef FUNCTOR_TYPE
        #define FUNCTOR_TYPE template <typename T, std::size_t INDEX, typename SrcTuple> typename Functor

        DEFINE_FIND_ALGORITHM(Overall, std::tuple_element_t<N, TupleT>, N, TupleT);
    }

    template 
    <
        typename TupleT,
        template <typename T> typename Functor
    >
    using Find = detail::Find<TupleT, Functor>;

    template
    <
        typename TupleT,
        typename T
    >
    using FindEqual = detail::Find<TupleT, detail::IsEqualTo<T>::template Type>;

    template 
    <
        typename TupleT,
        template <typename T, std::size_t INDEX> typename Functor
    >
    using IndexFind = detail::IndexFind<TupleT, Functor>;

    template 
    <
        typename TupleT,
        template <typename T, std::size_t INDEX, typename SrcTuple> typename Functor
    >
    using OverallFind = detail::OverallFind<TupleT, Functor>;
}

#undef FUNCTOR_TYPE
#undef DEFINE_FIND_ALGORITHM
#undef EXPAND

#endif // METAXXA_ALGORITHM_FIND_H

#ifndef METAXXA_ALGORITHM_SKIPRANGE_H
#define METAXXA_ALGORITHM_SKIPRANGE_H


namespace metaxxa
{
    template
    <
        template <typename...> typename Template,
        typename TupleT,
        std::size_t FROM_I,
        std::size_t TO_I
    >
    using SkipRange = Concat
    <
        Template,
        TakeFirst<TypeList, TupleT, FROM_I>,
        TakeLast<TypeList, TupleT, std::tuple_size_v<TupleT> - TO_I>
    >;
}

#endif // METAXXA_ALGORITHM_SKIPRANGE_H

#ifndef METAXXA_ALGORITHM_FILTER_H
#define METAXXA_ALGORITHM_FILTER_H



namespace metaxxa
{
    namespace detail
    {
        template 
        <
            typename TupleT,
            std::size_t INDEX,
            bool FunctorResult
        >
        struct ResolveFilterType
        {
            using Type = TemplateContainer<std::tuple_element_t<INDEX, TupleT>>;
        };

        template 
        <
            typename TupleT,
            std::size_t INDEX
        >
        struct ResolveFilterType<TupleT, INDEX, false>
        {
            using Type = TemplateContainer<>;
        };

        template 
        <
            template <typename...> typename Template,
            typename TupleT,
            template <typename T> typename Functor,
            std::size_t... INDICES
        >
        constexpr auto filter_types(std::index_sequence<INDICES...>)
            -> Concat
                <
                    Template, 
                    typename ResolveFilterType
                    <
                        TupleT, 
                        INDICES, 
                        Functor<std::tuple_element_t<INDICES, TupleT>>::value
                    >::Type...
                >;

        template 
        <
            template <typename...> typename Template,
            typename TupleT,
            template <typename T, std::size_t INDEX> typename Functor,
            std::size_t... INDICES
        >
        constexpr auto index_filter_types(std::index_sequence<INDICES...>)
            -> Concat
                <
                    Template, 
                    typename ResolveFilterType
                    <
                        TupleT, 
                        INDICES, 
                        Functor<std::tuple_element_t<INDICES, TupleT>, INDICES>::value
                    >::Type...
                >;

        template 
        <
            template <typename...> typename Template,
            typename TupleT,
            template <typename T, std::size_t INDEX, typename SrcTuple> typename Functor,
            std::size_t... INDICES
        >
        constexpr auto overall_filter_types(std::index_sequence<INDICES...>)
            -> Concat
                <
                    Template, 
                    typename ResolveFilterType
                    <
                        TupleT, 
                        INDICES, 
                        Functor<std::tuple_element_t<INDICES, TupleT>, INDICES, TupleT>::value
                    >::Type...
                >;
    }

    template 
    <
        template <typename...> typename Template,
        typename TupleT,
        template <typename T> typename Functor
    >
    using Filter = decltype
    (
        detail::filter_types
        <
            Template, 
            TupleT, 
            Functor
        >(std::make_index_sequence<std::tuple_size_v<TupleT>>())
    );

    template 
    <
        template <typename...> typename Template,
        typename TupleT,
        template <typename T, std::size_t INDEX> typename Functor
    >
    using IndexFilter = decltype
    (
        detail::index_filter_types
        <
            Template, 
            TupleT, 
            Functor
        >(std::make_index_sequence<std::tuple_size_v<TupleT>>())
    );

    template 
    <
        template <typename...> typename Template,
        typename TupleT,
        template <typename T, std::size_t INDEX, typename SrcTuple> typename Functor
    >
    using OverallFilter = decltype
    (
        detail::overall_filter_types
        <
            Template, 
            TupleT, 
            Functor
        >(std::make_index_sequence<std::tuple_size_v<TupleT>>())
    );
}

#endif // METAXXA_ALGORITHM_FILTER_H

#ifndef METAXXA_ALGORITHM_MAP_H
#define METAXXA_ALGORITHM_MAP_H



namespace metaxxa
{
    namespace detail
    {
        template 
        <
            template <typename...> typename Template,
            typename TupleT,
            template <typename T> typename Functor,
            std::size_t... INDICES
        >
        constexpr auto map_types(std::index_sequence<INDICES...>)
            -> Concat
                <
                    Template,  
                    TemplateContainer<typename Functor<std::tuple_element_t<INDICES, TupleT>>::type...>
                >;

        template 
        <
            template <typename...> typename Template,
            typename TupleT,
            template <typename T, std::size_t INDEX> typename Functor,
            std::size_t... INDICES
        >
        constexpr auto index_map_types(std::index_sequence<INDICES...>)
            -> Concat
                <
                    Template,  
                    TemplateContainer<typename Functor<std::tuple_element_t<INDICES, TupleT>, INDICES>::type...>
                >;

        template 
        <
            template <typename...> typename Template,
            typename TupleT,
            template <typename T, std::size_t INDEX, typename SrcTuple> typename Functor,
            std::size_t... INDICES
        >
        constexpr auto overall_map_types(std::index_sequence<INDICES...>)
            -> Concat
                <
                    Template,  
                    TemplateContainer<typename Functor<std::tuple_element_t<INDICES, TupleT>, INDICES, TupleT>::type...>
                >;
    }

    template 
    <
        template <typename...> typename Template,
        typename TupleT,
        template <typename T> typename Functor
    >
    using Map = decltype
    (
        detail::map_types
        <
            Template, 
            TupleT, 
            Functor
        >(std::make_index_sequence<std::tuple_size_v<TupleT>>())
    );

    template 
    <
        template <typename...> typename Template,
        typename TupleT,
        template <typename T, std::size_t INDEX> typename Functor
    >
    using IndexMap = decltype
    (
        detail::index_map_types
        <
            Template, 
            TupleT, 
            Functor
        >(std::make_index_sequence<std::tuple_size_v<TupleT>>())
    );

    template 
    <
        template <typename...> typename Template,
        typename TupleT,
        template <typename T, std::size_t INDEX, typename SrcTuple> typename Functor
    >
    using OverallMap = decltype
    (
        detail::overall_map_types
        <
            Template, 
            TupleT, 
            Functor
        >(std::make_index_sequence<std::tuple_size_v<TupleT>>())
    );
}

#endif // METAXXA_ALGORITHM_MAP_H

#ifndef METAXXA_ALGORITHM_EVERY_H
#define METAXXA_ALGORITHM_EVERY_H


namespace metaxxa
{
    namespace detail
    {
        template 
        <
            template <typename> typename Predicate,
            typename TupleT, 
            std::size_t... INDICES
        >
        constexpr bool every(std::index_sequence<INDICES...>)
        {
            return (true && ... && Predicate<std::tuple_element_t<INDICES, TupleT>>::value);
        }
    }

    template
    <
        template <typename> typename Predicate,
        typename TupleT
    >
    struct Every 
        : public std::bool_constant
        <
            detail::every<Predicate, TupleT>(std::make_index_sequence<std::tuple_size_v<TupleT>>())
        >
    {};

    template
    <
        template <typename> typename Predicate,
        typename TupleT
    >
    constexpr bool every = Every<Predicate, TupleT>::value;
}

#endif // METAXXA_ALGORITHM_EVERY_H

#ifndef METAXXA_CONTAINS_H
#define METAXXA_CONTAINS_H



namespace metaxxa
{
    template
    <
        typename TupleT,
        typename... Types
    >
    constexpr bool contains_all();

    namespace detail
    {
        template
        <
            typename TupleT,
            typename Type,
            std::size_t... INDICES
        >
        constexpr bool contains(std::index_sequence<INDICES...>)
        {
            return (false || ... || std::is_same_v<std::tuple_element_t<INDICES, TupleT>, Type>);
        }

        template
        <
            typename TupleT,
            typename RequestedTuple,
            std::size_t... INDICES
        >
        constexpr bool contains_packed(std::index_sequence<INDICES...>)
        {
            return contains_all<TupleT, std::tuple_element_t<INDICES, RequestedTuple>...>();
        }
    }

    template
    <
        typename TupleT,
        typename Type
    >
    constexpr bool contains()
    {
        using Tuple = MoveParameters<TypeTuple, TupleT>;        

        return detail::contains<Tuple, Type>(std::make_index_sequence<std::tuple_size_v<Tuple>>());
    }

    template
    <
        typename TupleT,
        typename... Types
    >
    constexpr bool contains_all()
    {
        using Tuple = MoveParameters<TypeTuple, TupleT>;

        return (true && ... && contains<Tuple, Types>());
    }

    template
    <
        typename TupleT,
        typename RequestedTuple
    >
    constexpr bool contains_packed()
    {
        using Tuple = MoveParameters<TypeTuple, TupleT>;
        using Requested = MoveParameters<TypeTuple, RequestedTuple>;

        return detail::contains_packed<Tuple, Requested>(std::make_index_sequence<std::tuple_size_v<Requested>>());
    }

    template <typename TupleT, typename T>
    using Contains = typename If<contains<TupleT, T>()>
        ::template Then<std::true_type>
        ::template Else<std::false_type>
        ::Type;

    template <typename TupleT, typename... Ts>
    using ContainsAll = typename If<contains_all<TupleT, Ts...>()>
        ::template Then<std::true_type>
        ::template Else<std::false_type>
        ::Type;

    template <typename TupleT, typename RequestedTupleT>
    using ContainsPacked = typename If<contains_packed<TupleT, RequestedTupleT>()>
        ::template Then<std::true_type>
        ::template Else<std::false_type>
        ::Type;
}

#endif // METAXXA_CONTAINS_H

#ifndef METAXXA_UNIQUE_H
#define METAXXA_UNIQUE_H


namespace metaxxa
{
    namespace detail
    {
        template 
        <
            template <typename...> typename ResultTemplate, 
            typename ResultTuple, 
            typename T, 
            typename... Rest
        >
        class Unique
        {
            static constexpr bool CONTAINS = metaxxa::contains<ResultTuple, T>();

            using NextUnique = typename If<CONTAINS>
                ::template Then
                <
                    Unique<ResultTemplate, ResultTuple, Rest...>
                >
                ::template Else
                <
                    Unique
                    <
                        ResultTemplate, 
                        typename ResultTuple::template Concat<TypeTuple<T>>, 
                        Rest...
                    >
                >
                ::Type;
        public:
            using Type = typename NextUnique::Type;

            using CheckType = typename If<CONTAINS>
                ::template Then<std::false_type>
                ::template Else<typename NextUnique::CheckType>
                ::Type;
        };

        template 
        <
            template <typename...> typename ResultTemplate,
            typename ResultTuple, 
            typename T
        >
        class Unique<ResultTemplate, ResultTuple, T>
        {
            static constexpr bool CONTAINS = metaxxa::contains<ResultTuple, T>();

            using FinalTuple = typename If<CONTAINS>
                ::template Then<ResultTuple>
                ::template Else<typename ResultTuple::template Concat<TypeTuple<T>>>
                ::Type;

        public:
            using Type = MoveParameters<ResultTemplate, FinalTuple>;

            using CheckType = typename If<CONTAINS>
                ::template Then<std::false_type>
                ::template Else<std::true_type>
                ::Type;
        };

        template 
        <
            template <typename...> typename ResultTemplate,
            typename... Types
        >
        struct AvoidEmpty
        {
            using Type = typename Unique<ResultTemplate, TypeTuple<>, Types...>::Type;

            using CheckType = typename Unique<ResultTemplate, TypeTuple<>, Types...>::CheckType;
        };

        template <template <typename...> typename ResultTemplate>
        struct AvoidEmpty<ResultTemplate>
        {
            using Type = ResultTemplate<>;

            using CheckType = std::true_type;
        };

        template <template <typename...> typename ResultTemplate>
        struct UniqueGate
        {
            template <typename... Types>
            struct Type
            {
                using Result = typename AvoidEmpty<ResultTemplate, Types...>::Type;
            };

            template <typename... Types>
            using CheckType = typename AvoidEmpty<ResultTemplate, Types...>::CheckType;
        };
    }

    template
    <
        template <typename...> typename ResultTemplate,
        typename TupleT
    >
    using Unique = typename MoveParameters
    <
        detail::UniqueGate<ResultTemplate>::template Type,
        TupleT
    >::Result;

    template <typename TupleT>
    using IsUnique = MoveParameters
    <
        detail::UniqueGate<TypeTuple>::template CheckType,
        TupleT
    >;

    template <typename TupleT>
    constexpr bool is_unique()
    {
        return IsUnique<TupleT>::value;
    }
}

#endif // METAXXA_UNIQUE_H

#ifndef METAXXA_ALGORITHM_INVOKEFUNCTIONS_INC
#define METAXXA_ALGORITHM_INVOKEFUNCTIONS_INC


#ifndef METAXXA_ALGORITHM_INVOKEFUNCTIONS_H
#define METAXXA_ALGORITHM_INVOKEFUNCTIONS_H



namespace metaxxa
{
    namespace detail
    {
        template
        <
            typename Function,
            bool IS_INVOCABLE,
            typename... Args
        >
        struct Invoker
        {
            metaxxa_inline static void invoke(Function &function, Args&&... args);

            metaxxa_inline static void invoke(const Function &function, Args&&... args);
        };

        template
        <
            typename Function,
            typename... Args
        >
        struct Invoker<Function, false, Args...>
        {
            metaxxa_inline static void invoke(const Function &, Args&&...);
        };

        template 
        <
            typename Tuple, 
            typename... Args
        >
        struct FunctionsInvoker
        {
            template <std::size_t... INDICES>
            metaxxa_inline static void invoke(std::index_sequence<INDICES...>, Tuple &, Args&&...);

            template <std::size_t... INDICES>
            metaxxa_inline static void invoke(std::index_sequence<INDICES...>, const Tuple &, Args&&...);
        };
    }

    template <typename Tuple, typename... Args>
    void invoke_functions(Tuple &, Args&&...);

    template <typename Tuple, typename... Args>
    void invoke_functions(const Tuple &, Args&&...);
}

#endif // METAXXA_ALGORITHM_INVOKEFUNCTIONS_H

namespace metaxxa
{
    namespace detail
    {
        template
        <
            typename Function,
            bool IS_INVOCABLE,
            typename... Args
        >
        metaxxa_inline void Invoker<Function, IS_INVOCABLE, Args...>::invoke(Function &function, Args&&... args)
        {
            std::invoke(function, std::forward<Args>(args)...);
        }

        template
        <
            typename Function,
            bool IS_INVOCABLE,
            typename... Args
        >
        metaxxa_inline void Invoker<Function, IS_INVOCABLE, Args...>::invoke(const Function &function, Args&&... args)
        {
            std::invoke(function, std::forward<Args>(args)...);
        }

        template
        <
            typename Function,
            typename... Args
        >
        metaxxa_inline void Invoker<Function, false, Args...>::invoke(const Function &, Args&&...)
        {}
        

        template 
        <
            typename Tuple, 
            typename... Args
        >
        template <std::size_t... INDICES>
        metaxxa_inline void FunctionsInvoker<Tuple, Args...>::invoke(std::index_sequence<INDICES...>, Tuple &tuple, Args&&... args)
        {
            (
                Invoker
                <
                    std::tuple_element_t<INDICES, Tuple>, 
                    std::is_invocable_v<std::tuple_element_t<INDICES, Tuple>, Args...>, 
                    Args...
                >::invoke
                (
                    std::get<INDICES>(tuple), 
                    std::forward<Args>(args)...
                ),
                ...
            );
        }

        template 
        <
            typename Tuple, 
            typename... Args
        >
        template <std::size_t... INDICES>
        metaxxa_inline void FunctionsInvoker<Tuple, Args...>::invoke(std::index_sequence<INDICES...>, const Tuple &tuple, Args&&... args)
        {
            (
                Invoker
                <
                    std::tuple_element_t<INDICES, Tuple>, 
                    std::is_invocable_v<std::tuple_element_t<INDICES, Tuple>, Args...>, 
                    Args...
                >::invoke
                (
                    std::get<INDICES>(tuple), 
                    std::forward<Args>(args)...
                ),
                ...
            );
        }
    }

    template <typename Tuple, typename... Args>
    void invoke_functions(Tuple &tuple, Args&&... args)
    {
        detail::FunctionsInvoker<Tuple, Args...>::invoke(std::make_index_sequence<std::tuple_size_v<Tuple>>(), tuple, std::forward<Args>(args)...);
    }

    template <typename Tuple, typename... Args>
    void invoke_functions(const Tuple &tuple, Args&&... args)
    {
        detail::FunctionsInvoker<Tuple, Args...>::invoke(std::make_index_sequence<std::tuple_size_v<Tuple>>(), tuple, std::forward<Args>(args)...);
    }
}

#endif // METAXXA_ALGORITHM_INVOKEFUNCTIONS_INC

#endif // METAXXA_ALGORITHM_H


#ifndef METAXXA_SWITCH_H
#define METAXXA_SWITCH_H


namespace metaxxa
{
    template <typename T, T CONSTANT, typename CaseType>
    struct SwitchCase
    {
        using ConstantType = T;
        static constexpr ConstantType value = CONSTANT;

        using type = CaseType;

        template <T VALUE>
        static constexpr bool is_equal_to() { return value == VALUE; }
    };

    template <typename T, typename CaseType>
    struct SwitchDefaultCase
    {
        using type = CaseType;

        template <T VALUE>
        static constexpr bool is_equal_to() { return true; }
    };

    namespace detail
    {
        template <typename T, T CONSTANT, typename Cases>
        struct SwitchType
        {
            template <typename SwitchCase>
            struct Functor
            {
                static constexpr bool value = SwitchCase::template is_equal_to<CONSTANT>();
            };

            using Type = typename Find<Cases, Functor>::Type::type;
        };

        template <typename T, typename DefaultType, typename CasesList>
        struct SwitchDefault
        {
            using Cases = Concat<TypeList, CasesList, TemplateContainer<SwitchDefaultCase<T, DefaultType>>>;

            template <T CONSTANT>
            using Type = typename SwitchType<T, CONSTANT, Cases>::Type;
        };

        template <typename T, T CASE_CONSTANT, typename CaseType, typename CasesList>
        struct CaseImpl
        {
            using Cases = Concat<TypeList, CasesList, TemplateContainer<SwitchCase<T, CASE_CONSTANT, CaseType>>>;

            template <T NEXT_CONSTANT, typename NextCaseType>
            using Case = CaseImpl<T, NEXT_CONSTANT, NextCaseType, Cases>;

            template <typename DefaultT>
            using Default = SwitchDefault<T, DefaultT, Cases>; 

            template <T CONSTANT>
            using Type = typename SwitchType<T, CONSTANT, Cases>::Type;
        };
    }

    template <typename T>
    struct Switch
    {
        template <T CASE_CONSTANT, typename CaseType>
        using Case = detail::CaseImpl
        <
            T,
            CASE_CONSTANT,
            CaseType,
            TypeList<>
        >;
    };
}

#endif // METAXXA_SWITCH_H

#ifndef METAXXA_TYPESWITCH_H
#define METAXXA_TYPESWITCH_H


namespace metaxxa
{
    template <typename CaseCondition, typename CaseType>
    struct TypeSwitchCase
    {
        using Condition = CaseCondition;
        using type      = CaseType;

        template <typename T>
        static constexpr bool is_equal_to() { return std::is_same_v<T, Condition>; }
    };

    template <typename CaseType>
    struct TypeSwitchDefaultCase
    {
        using type = CaseType;

        template <typename T>
        static constexpr bool is_equal_to() { return true; }
    };

    namespace detail
    {
        template <typename CaseCondition, typename CasesList>
        struct TypeSwitchType
        {
            template <typename TypeSwitchCase>
            struct Functor
            {
                static constexpr bool value = TypeSwitchCase::template is_equal_to<CaseCondition>();
            };

            using Type = typename Find<CasesList, Functor>::Type::type;
        };

        template <typename DefaultType, typename CasesList>
        struct TypeSwitchDefault
        {
            using Cases = Concat<TypeList, CasesList, TemplateContainer<TypeSwitchDefaultCase<DefaultType>>>;

            template <typename T>
            using Type = typename TypeSwitchType<T, Cases>::Type;
        };

        template <typename CaseCondition, typename CaseType, typename CasesList>
        struct TypeCaseImpl
        {
            using Cases = Concat<TypeList, CasesList, TemplateContainer<TypeSwitchCase<CaseCondition, CaseType>>>;

            template <typename NextCaseCondition, typename NextCaseType>
            using Case = TypeCaseImpl<NextCaseCondition, NextCaseType, Cases>;

            template <typename DefaultT>
            using Default = TypeSwitchDefault<DefaultT, Cases>; 

            template <typename T>
            using Type = typename TypeSwitchType<T, Cases>::Type;
        };
    }

    struct TypeSwitch
    {
        template <typename CaseCondition, typename CaseType>
        using Case = detail::TypeCaseImpl
        <
            CaseCondition,
            CaseType,
            TypeList<>
        >;
    };
}

#endif // METAXXA_TYPESWITCH_H


#ifndef METAXXA_MAKEFUNCTIONTYPE_H
#define METAXXA_MAKEFUNCTIONTYPE_H



namespace metaxxa
{
    namespace detail
    {
        template <typename ResultType>
        struct MakeFunctionType
        {
            template <typename... Args>
            using Type = ResultType(Args...);
        };

        template <typename Tuple, std::size_t RETURN_INDEX>
        struct MakeFunctionTypeImpl
        {
            using ResultType = std::tuple_element_t<RETURN_INDEX, Tuple>;
            using ArgsList  = SkipRange<TypeList, Tuple, RETURN_INDEX, RETURN_INDEX + 1>;

            using Type = MoveParameters
            <
                MakeFunctionType<ResultType>::template Type,
                ArgsList
            >;
        };
    }

    template <typename Tuple, std::size_t RETURN_INDEX>
    using MakeFunctionType = typename detail::MakeFunctionTypeImpl<Tuple, RETURN_INDEX>::Type;
}

#endif // METAXXA_MAKEFUNCTIONTYPE_H


#ifndef METAXXA_ENABLEFNIF_H
#define METAXXA_ENABLEFNIF_H


#define ENABLE_T_IF(CONDITION) typename = std::enable_if_t<CONDITION>

#define ENABLE_FN_IF_T(CONDITION) std::enable_if_t<CONDITION> *

#define ENABLE_FN_IF(CONDITION) ENABLE_FN_IF_T(CONDITION) = nullptr

#endif // METAXXA_ENABLEFNIF_H

#ifndef METAXXA_ISINSTANTIATIONOF_H
#define METAXXA_ISINSTANTIATIONOF_H


namespace metaxxa
{
    namespace detail
    {
        template 
        <
            template <typename...> typename TemplateType, 
            template <typename...> typename SrcType, 
            typename... Types
        >
        constexpr auto is_instatiation_of(SrcType<Types...> &&)
            -> std::bool_constant<std::is_same_v<TemplateType<Types...>, SrcType<Types...>>>;

        template 
        <
            template <typename...> typename TemplateType,
            typename SrcType,
            typename... Types
        >
        constexpr auto is_instatiation_of(SrcType &&)
            -> std::false_type;
    }

    template <typename T, template <typename...> typename TemplateType>
    constexpr bool is_instatiation_of()
    {
        return decltype(detail::is_instatiation_of<TemplateType>(std::declval<T>()))::value;
    }
}

#endif // METAXXA_ISINSTANTIATIONOF_H

#ifndef METAXXA_ISARRAYOF_H
#define METAXXA_ISARRAYOF_H


namespace metaxxa
{
    template <typename T, typename Of>
    constexpr bool is_array_of()
    {
        using TNCV = std::remove_cv_t<std::remove_reference_t<T>>;
        using OfNCV = std::remove_cv_t<std::remove_reference_t<Of>>;

        return std::is_array_v<TNCV> 
        && std::is_same_v
        <
            TNCV, 
            OfNCV[std::extent_v<TNCV>]
        >;
    }    
}

#endif // METAXXA_ISARRAYOF_H

#ifndef METAXXA_ISAPPLICABLE_H
#define METAXXA_ISAPPLICABLE_H


namespace metaxxa
{
    namespace detail
    {
        template <typename Callable, typename TupleT, std::size_t... INDICES>
        constexpr bool is_applicable(std::index_sequence<INDICES...>)
        {
            return std::is_invocable_v<Callable, std::tuple_element_t<INDICES, TupleT>...>;
        }
    }

    template <typename Callable, typename TupleT>
    constexpr bool is_applicable()
    {
        return detail::is_applicable<Callable, TupleT>(std::make_index_sequence<std::tuple_size_v<TupleT>>());
    }
}

#endif // METAXXA_ISAPPLICABLE_H

#ifndef METAXXA_APPLYRESULT_H
#define METAXXA_APPLYRESULT_H


namespace metaxxa
{
    namespace detail
    {
        template <typename Function, typename... Args>
        struct ApplyResult
        {
            using type = std::invoke_result_t<Function, Args...>;
        };

        template <typename Function, typename TupleT, std::size_t... INDICES>
        auto apply_result(std::index_sequence<INDICES...>) 
            -> ApplyResult<Function, std::tuple_element_t<INDICES, TupleT>...>;
    }
    
    template <typename Function, typename TupleT>
    using ApplyResult = typename decltype(detail::apply_result<Function, TupleT>(std::make_index_sequence<std::tuple_size_v<TupleT>>()))::type;
}

#endif // METAXXA_APPLYRESULT_H

#ifndef METAXXA_TYPE_H
#define METAXXA_TYPE_H


namespace metaxxa
{
    template <typename T>
    class Type
    {
    public:
        using Unwrap = T;

        template <typename... With>
        using Concat = Type<::metaxxa::Concat<TypeList, T, With...>>;

        template <template <typename...> typename Template>
        using MoveParameters = Type<::metaxxa::MoveParameters<Template, T>>;

        template <std::size_t N>
        using Times = Type<::metaxxa::Times<TypeList, N, T>>;

        template <template <typename> typename Functor>
        using Filter = Type<::metaxxa::Filter<TypeList, T, Functor>>;

        template <template <typename, std::size_t INDEX> typename Functor>
        using IndexFilter = Type<::metaxxa::IndexFilter<TypeList, T, Functor>>;

        template <template <typename, std::size_t INDEX, typename SrcTuple> typename Functor>
        using OverallFilter = Type<::metaxxa::OverallFilter<TypeList, T, Functor>>;

        template <typename Sequence>
        using IndexSeqFilter = Type<SeqFilter<TypeList, T, Sequence>>;

        template <template <typename> typename Functor>
        using Map = Type<::metaxxa::Map<TypeList, T, Functor>>;

        template <template <typename, std::size_t INDEX> typename Functor>
        using IndexMap = Type<::metaxxa::IndexMap<TypeList, T, Functor>>;

        template <template <typename, std::size_t INDEX, typename SrcTuple> typename Functor>
        using OverallMap = Type<::metaxxa::OverallMap<TypeList, T, Functor>>;

        template <std::size_t... INDICES>
        using OnlyIndices = Type<::metaxxa::OnlyIndices<TypeList, T, INDICES...>>;

        template <std::size_t N>
        using SkipFirst = Type<::metaxxa::SkipFirst<TypeList, T, N>>;

        template <std::size_t N>
        using SkipLast = Type<::metaxxa::SkipLast<TypeList, T, N>>;

        template <std::size_t FROM, std::size_t TO>
        using SkipRange = Type<::metaxxa::SkipRange<TypeList, T, FROM, TO>>;

        template <std::size_t N>
        using TakeFirst = Type<::metaxxa::TakeFirst<TypeList, T, N>>;

        template <std::size_t N>
        using TakeLast = Type<::metaxxa::TakeLast<TypeList, T, N>>;

        template <std::size_t FROM, std::size_t TO>
        using TakeRange = Type<::metaxxa::TakeRange<TypeList, T, FROM, TO>>;

        using Unique = Type<::metaxxa::Unique<TypeList, T>>;

        static constexpr std::size_t parameters_count() { return ::metaxxa::parameters_count<T>(); }

        template <template <typename...> typename Template>
        static constexpr bool is_instatiation_of() { return ::metaxxa::is_instatiation_of<T, Template>(); }

        template <typename Callable>
        static constexpr bool is_applicable() { return ::metaxxa::is_applicable<Callable, T>(); }

        template <typename U>
        static constexpr bool contains() { return ::metaxxa::contains<T, U>(); }

        template <typename... Ts>
        static constexpr bool contains_all() { return ::metaxxa::contains_all<T, Ts...>(); }

        template <typename Tuple>
        static constexpr bool contains_packed() { return ::metaxxa::contains_packed<T, Tuple>(); }

        template <template <typename> typename Predicate>
        static constexpr bool every() { return ::metaxxa::Every<Predicate, T>::value; }

        static constexpr bool is_parameters_unique() { return ::metaxxa::is_unique<T>(); }
    };
}

#endif // METAXXA_TYPE_H


#endif // METAXXA_HPP


#ifndef TEFRI_FLOWVALUE_H
#define TEFRI_FLOWVALUE_H



#ifndef TEFRI_PRIORITY_H
#define TEFRI_PRIORITY_H

namespace tefri
{
    enum class Priority : unsigned long long
    {
        LOWEST       = 0ull,
        LOW          = 10ull,
        BELOW_NORMAL = 30ull,
        NORMAL       = 50ull,
        ABOVE_NORMAL = 70ull,
        HIGH         = 90ull,
        HIGHEST      = 100ull
    };
}

#endif // TEFRI_PRIORITY_H

namespace tefri
{
    class FlowValue
    {
    public:
        using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

        tefri_inline FlowValue(unsigned long long priority = static_cast<unsigned long long>(Priority::NORMAL));

        tefri_inline FlowValue(Priority);

        tefri_inline FlowValue(const FlowValue &);

        tefri_inline FlowValue(FlowValue &&);

        tefri_inline virtual ~FlowValue();

        tefri_inline FlowValue &operator=(const FlowValue &) = delete;

        tefri_inline FlowValue &operator=(FlowValue &&) = delete;

        tefri_inline const TimePoint &creation_time() const;

        tefri_inline std::size_t operator_index() const;

        tefri_inline unsigned long long priority() const;

        tefri_inline void priority(unsigned long long);

        tefri_inline void priority(Priority);

        tefri_inline void next_operator_index();

    private:
        const TimePoint    creation;
        std::atomic_size_t index;
        std::atomic_ullong priority_;
    };
}

#endif // TEFRI_FLOWVALUE_H

namespace tefri
{
    namespace detail
    {
        template <typename T>
        constexpr bool is_reference_wrapper()
        {
            return metaxxa::is_instatiation_of<std::decay_t<T>, std::reference_wrapper>();
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

        ObjectHolder(const std::remove_reference_t<T> &object);

        ObjectHolder(std::remove_reference_t<T> &&object);

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
    ObjectHolder(T) -> ObjectHolder<T>;

    template <typename T>
    ObjectHolder(std::shared_future<WrapObject<T>>) -> ObjectHolder<T>;

    template <typename T>
    ObjectHolder(std::future<WrapObject<T>>) -> ObjectHolder<T>;
}

#endif // TEFRI_DETAIL_OBJECTHOLDER_H

#ifndef TEFRI_FLOWVALUE_INC
#define TEFRI_FLOWVALUE_INC


namespace tefri
{
    FlowValue::FlowValue(unsigned long long priority)
    : creation(std::chrono::high_resolution_clock::now()), index(0), priority_(priority)
    {}

    FlowValue::FlowValue(Priority priority)
    : FlowValue(static_cast<unsigned long long>(priority))
    {}

    FlowValue::FlowValue(const FlowValue &other)
    : creation(other.creation), index(other.index.load())
    {}

    FlowValue::FlowValue(FlowValue &&other)
    : creation(std::move(other.creation)), index(other.index.load())
    {}

    FlowValue::~FlowValue()
    {}

    const FlowValue::TimePoint &FlowValue::creation_time() const
    {
        return creation;
    }

    std::size_t FlowValue::operator_index() const
    {
        return index.load();
    }

    void FlowValue::next_operator_index()
    {
        ++index;
    }

    unsigned long long FlowValue::priority() const
    {
        return priority_.load();
    }

    void FlowValue::priority(unsigned long long priority)
    {
        priority_.store(priority);
    }

    void FlowValue::priority(Priority p)
    {
        priority(static_cast<unsigned long long>(p));
    }
}

#endif // TEFRI_FLOWVALUE_INC

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

#ifndef TEFRI_MONAD_INC
#define TEFRI_MONAD_INC


#ifndef TEFRI_MONAD_H
#define TEFRI_MONAD_H


#ifndef TEFRI_SEQ_H
#define TEFRI_SEQ_H



namespace tefri
{
    template <typename... Ts>
    struct Seq : public std::tuple<Ts...>
    {
        static_assert
        (
            sizeof...(Ts) > 0, 
            "Sequence can't be empty"
        );
        
        static_assert
        (
            !metaxxa::contains<std::tuple<Ts...>, void>(),
            "Sequence can't contain void"
        );

        using std::tuple<Ts...>::tuple;
    };

    template <typename... Args>
    Seq(Args...) -> Seq<Args...>;
}

namespace std
{
    template <std::size_t INDEX, typename... Ts>
    class tuple_element<INDEX, tefri::Seq<Ts...>>
        : public tuple_element<INDEX, std::tuple<Ts...>>
    {};

    template <typename... Ts>
    class tuple_size<tefri::Seq<Ts...>> 
        : public std::tuple_size<std::tuple<Ts...>>
    {};

    template <std::size_t INDEX, typename... Ts>
    auto &get(tefri::Seq<Ts...> &sequence)
    {
        return std::get<INDEX>(static_cast<std::tuple<Ts...> &>(sequence));
    }

    template <std::size_t INDEX, typename... Ts>
    const auto &get(const tefri::Seq<Ts...> &sequence)
    {
        return std::get<INDEX>(static_cast<const std::tuple<Ts...> &>(sequence));
    }
}

#endif // TEFRI_SEQ_H

namespace tefri
{
    template <typename... Seqs>
    struct Monad : public Monad<Seqs>...
    {
        using Monad<Seqs>::invoke...;
    };

    template <typename... Types>
    struct Monad<Seq<Types...>>
    {
        virtual ~Monad() = default;

        virtual void invoke(Types&&... args) = 0;
    };
}

#endif // TEFRI_MONAD_H

#ifndef TEFRI_DETAIL_MONADIMPL_INC
#define TEFRI_DETAIL_MONADIMPL_INC


#ifndef TEFRI_DETAIL_MONADIMPL_H
#define TEFRI_DETAIL_MONADIMPL_H



#ifndef TEFRI_DETAIL_MONADINVOKER_INC
#define TEFRI_DETAIL_MONADINVOKER_INC


#ifndef TEFRI_DEMANGLE_INC
#define TEFRI_DEMANGLE_INC



#ifndef TEFRI_DEMANGLE_H
#define TEFRI_DEMANGLE_H


#ifndef _MSC_VER
#   include <cxxabi.h>
#endif // _MSC_VER

namespace tefri::detail
{
    template <typename T>
    std::string demangle_name();
}

#endif // TEFRI_DEMANGLE_H


namespace tefri::detail
{
    template <typename T>
    std::string demangle_name()
    {
#   ifdef _MSC_VER
        return typeid(T).name();
#   else
        const char *mangled = typeid(metaxxa::TypeSaver<T>).name();
        int status = 0;

        const char *name = abi::__cxa_demangle(mangled, 0, 0, &status);
        switch(status)
        {
        case 0:
            break;
        case -1:
            throw std::bad_alloc();
        case -2:
            throw std::runtime_error(std::string(mangled) + ": not mangled name");
        default:
            throw std::runtime_error("Unknown error with demangle name");
        }

        std::string_view name_view(name);

        std::size_t prefix_l = strlen("metaxxa::TypeSaver<");
        std::size_t suffix_l = name_view.size() - name_view.find_last_of('>');
        name_view.remove_prefix(prefix_l);
        name_view.remove_suffix(suffix_l);
        
        std::string result(name_view.begin(), name_view.end());
        free(const_cast<void*>(static_cast<const void*>(name)));

        return result;
#   endif // _MSC_VER
    }
}

#endif // TEFRI_DEMANGLE_INC


#ifndef TEFRI_DETAIL_MONADINVOKER_H
#define TEFRI_DETAIL_MONADINVOKER_H


namespace tefri::detail
{
    template <typename Functions, std::size_t INDEX, typename... Seqs>
    struct MonadImpl;

    template <typename T>
    using HoldArg 
        = typename metaxxa::If<metaxxa::is_instatiation_of<std::decay_t<T>, ObjectHolder>()>
        ::template Then<std::decay_t<T>>
        ::template Else<ObjectHolder<std::decay_t<T>>>
        ::Type;

    template <typename T>
    struct ArgUnwrapper
    {
        using type = std::decay_t<T>;
    };

    template <typename T>
    struct ArgUnwrapper<ObjectHolder<T>>
    {
        using type = std::decay_t<T>;
    };

    template <typename T>
    using UnwrapArg = typename ArgUnwrapper<T>::type;

    template 
    <
        typename MonadImpl, 
        std::size_t LAST_INDEX = std::tuple_size_v<typename MonadImpl::Functions>
    >
    struct MonadInvoker
    {
    private:
        static constexpr std::size_t FN_INDEX = MonadImpl::INDEX;
        using Fns = typename MonadImpl::Functions;
        using Fn = std::tuple_element_t<FN_INDEX, Fns>;

        using Next = typename MonadImpl::template Next<1>;

    public:
        template <typename... Args>
        static inline auto invoke(MonadImpl &&, HoldArg<Args>&&...);
    };

    template
    <
        typename Functions, 
        std::size_t INDEX,
        typename... Seqs
    >
    struct MonadInvoker<MonadImpl<Functions, INDEX, Seqs...>, INDEX>
    {
    public:
        template <typename... Args>
        static inline auto invoke(MonadImpl<Functions, INDEX, Seqs...> &&, HoldArg<Args>&&...)
        { return; }
    };
}

#endif // TEFRI_DETAIL_MONADINVOKER_H

namespace tefri::detail
{
    template <typename MonadImpl, std::size_t LAST_INDEX>
    template <typename... Args>
    auto MonadInvoker<MonadImpl, LAST_INDEX>::invoke(MonadImpl &&monad, HoldArg<Args>&&... args)
    {
        static constexpr bool IS_NOT_LAST = (FN_INDEX != LAST_INDEX);
        static constexpr bool IS_INVOCABLE = std::is_invocable_v<Fn, Next, HoldArg<Args>...>;
        if constexpr (IS_INVOCABLE && IS_NOT_LAST)
        {
            Fn &f = std::get<FN_INDEX>(monad.functions);
            Next &next = monad.template next<1>();

            return f(next, std::forward<HoldArg<Args>>(args)...);
        }
        else
            return /*void*/;
    }
}

#endif // TEFRI_DETAIL_MONADINVOKER_INC

#ifndef TEFRI_DETAIL_SEQMAPPER_H
#define TEFRI_DETAIL_SEQMAPPER_H


#ifndef TEFRI_DETAIL_TYPEMAPPERMONAD_H
#define TEFRI_DETAIL_TYPEMAPPERMONAD_H


namespace tefri::detail
{
    template <typename, std::size_t, typename...>
    struct MonadImpl;

    template <typename... Types>
    struct UnwrapTypesStruct
    {
        using type = Seq<UnwrapArg<std::decay_t<Types>>...>;
    };

    template<>
    struct UnwrapTypesStruct<>
    {
        using type = void;
    };

    template <typename... Types>
    using UnwrapTypes = typename UnwrapTypesStruct<Types...>::type;

    inline auto unwrap_type = [](auto &&, auto&&... args)
        -> UnwrapTypes<decltype(args)...>
    { throw; };

    template <typename MonadImpl, std::size_t LAST_INDEX>
    struct TypeMapperMonadStruct;

    template 
    <
        std::size_t LAST_INDEX,
        typename Functions,
        typename... Seqs
    >
    struct TypeMapperMonadStruct<MonadImpl<Functions, 0, Seqs...>, LAST_INDEX>
    {
        using FunctionsLHS = metaxxa::TakeFirst
        <
            metaxxa::TypeList,
            Functions,
            LAST_INDEX
        >;

        using WithUnwrapperFunction = metaxxa::Concat
        <
            std::tuple,
            FunctionsLHS,
            metaxxa::TemplateContainer<decltype(unwrap_type)>
        >;

        using type = MonadImpl<WithUnwrapperFunction, 0, Seqs...>;
    };

    template <typename MonadImpl, std::size_t LAST_INDEX>
    using TypeMapperMonad = typename TypeMapperMonadStruct<MonadImpl, LAST_INDEX>::type;
}

#endif // TEFRI_DETAIL_TYPEMAPPERMONAD_H

namespace tefri::detail
{
    template <typename MonadImpl, std::size_t LAST_INDEX>
    struct SeqMapper;

    template
    <
        typename Functions,
        std::size_t LAST_INDEX,
        typename... Seqs
    >
    struct SeqMapper
    <
        MonadImpl<Functions, 0, Seqs...>,
        LAST_INDEX
    >
    {
        template <typename Seq>
        struct Functor;

        template <typename... Types>
        struct Functor<Seq<Types...>>
        {
            using type = decltype
            (
                MonadInvoker
                <
                    TypeMapperMonad<MonadImpl<Functions, 0, Seqs...>, LAST_INDEX>
                >
                ::template invoke<Types...>
                (
                    std::declval<TypeMapperMonad<MonadImpl<Functions, 0, Seqs...>, LAST_INDEX>>(),
                    std::declval<Types>()...
                )
            );
        };
    };
}

#endif // TEFRI_DETAIL_SEQMAPPER_H

namespace tefri::detail
{
    template <typename T>
    struct NotVoid : std::bool_constant<!std::is_same_v<T, void>> {};

    template <typename Monad, typename MonadImpl, typename... Seqs>
    struct MonadImplBase : MonadImplBase<Monad, MonadImpl, Seqs>...
    {
        using MonadImplBase<Monad, MonadImpl, Seqs>::invoke...;
    };

    template <typename Monad, typename MonadImpl, typename... Types>
    struct MonadImplBase<Monad, MonadImpl, Seq<Types...>> 
        : virtual public ::tefri::Monad<Seq<Types...>>
    {
        virtual void invoke(Types&&... args) override;
    };

    template <typename _Functions, std::size_t _INDEX, typename... Seqs>
    struct MonadImpl : public MonadImplBase
    <
        Monad<Seqs...>,
        MonadImpl<_Functions, _INDEX, Seqs...>, 
        Seqs...
    >
    {
        using InputSeqTuple = std::tuple<Seqs...>;

        static_assert
        (
            metaxxa::is_unique<InputSeqTuple>(),
            "Sequences must be unique"  
        );

        using Functions = _Functions;

        static constexpr std::size_t INDEX = _INDEX;

        template <std::size_t N>
        using Next = MonadImpl<Functions, INDEX + N, Seqs...>;

        template <typename Function>
        using WithFunction = MonadImpl
        <
            metaxxa::Concat<std::tuple, Functions, metaxxa::TemplateContainer<Function>>,
            INDEX,
            Seqs...
        >;

    public:
        template <long long N>
        using SeqTuple = typename
            metaxxa::Type<InputSeqTuple>
                ::template Map<SeqMapper<MonadImpl<Functions, 0, Seqs...>, INDEX + N>::template Functor>
                ::template Filter<NotVoid>
                ::Unique
                ::template MoveParameters<std::tuple>
                ::Unwrap;

        MonadImpl();

        MonadImpl(Functions &&);

        template <typename Function>
        WithFunction<Function> operator>>(Function &) &&;

        template <typename Function>
        WithFunction<Function> operator>>(const Function &) &&;

        template <typename Function>
        WithFunction<Function> operator>>(Function &&) &&;

        template <typename... Args>
        auto operator()(Args&&... args);

        template <typename... Args>
        auto operator()(Seq<Args...> &&args);

        template <std::size_t N>
        Next<N> &next();

    private:
        template <typename, std::size_t>
        friend struct MonadInvoker;

        template <typename Seq, std::size_t... INDICES>
        auto invoke_seq(Seq &&args, std::index_sequence<INDICES...>);

        Functions functions;
    };

    template <typename... Seqs>
    MonadImpl<std::tuple<>, 0, Seqs...> monad();
}

/*
    Seq<int, char>  |----\
    Seq<double>     |------> std::variant<int, char, double, A, C>
    Seq<A, int, C>  |----/
*/

#endif // TEFRI_DETAIL_MONADIMPL_H

namespace tefri::detail
{
    template <typename Tuple, std::size_t... INDICES>
    auto rvalue_tuple_impl(Tuple &tuple, std::index_sequence<INDICES...>)
    {
        return std::tuple
        <
            std::add_rvalue_reference_t
            <
                std::tuple_element_t<INDICES, Tuple>
            >...
        >(std::move(std::get<INDICES>(tuple))...);
    }

    template <typename... Args>
    auto rvalue_tuple(std::tuple<Args...> &tuple)
        -> std::tuple<std::add_rvalue_reference_t<Args>...>
    {
        return rvalue_tuple_impl(tuple, std::make_index_sequence<sizeof...(Args)>());
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    MonadImpl<Fns, I, Seqs...>::MonadImpl()
    {}

    template <typename Fns, std::size_t I, typename... Seqs>
    MonadImpl<Fns, I, Seqs...>::MonadImpl(Functions &&fns)
        : functions(std::forward<Functions>(fns))
    {}

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename Function>
    typename MonadImpl<Fns, I, Seqs...>::template WithFunction<Function> 
    MonadImpl<Fns, I, Seqs...>::operator>>(Function &fn) &&
    {
        return (*this >> std::forward<Function>(fn));
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename Function>
    typename MonadImpl<Fns, I, Seqs...>::template WithFunction<Function> 
    MonadImpl<Fns, I, Seqs...>::operator>>(const Function &fn) &&
    {
        return (*this >> std::forward<Function>(fn));
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename Function>
    typename MonadImpl<Fns, I, Seqs...>::template WithFunction<Function> 
    MonadImpl<Fns, I, Seqs...>::operator>>(Function &&fn) &&
    {
        return WithFunction<Function>
        (
            std::tuple_cat(rvalue_tuple(functions), std::tuple<Function&&>(std::forward<Function>(fn)))
        );
    }

    template <typename Monad, typename MonadImpl, typename... Types>
    void MonadImplBase<Monad, MonadImpl, Seq<Types...>>::invoke(Types&&... args)
    {
        static_cast<MonadImpl*>(this)->invoke(std::forward<Types>(args)...);
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename... Args>
    auto MonadImpl<Fns, I, Seqs...>::operator()(Args&&... args)
    {
        return MonadInvoker<MonadImpl<Fns, I, Seqs...>>::template invoke<Args...>
        (
            std::forward<MonadImpl<Fns, I, Seqs...>>(*this),
            std::forward<Args>(args)...
        );
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename... Args>
    auto MonadImpl<Fns, I, Seqs...>::operator()(Seq<Args...> &&args)
    {
        return invoke_seq(args, std::make_index_sequence<sizeof...(Args)>());
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <typename Seq, std::size_t... INDICES>
    auto MonadImpl<Fns, I, Seqs...>::invoke_seq(Seq &&args, std::index_sequence<INDICES...>)
    {
        return (*this)
        (
            std::forward
            <
                std::tuple_element_t<INDICES, std::decay_t<Seq>>
            >(std::get<INDICES>(args))...
        );
    }

    template <typename Fns, std::size_t I, typename... Seqs>
    template <std::size_t N>
    typename MonadImpl<Fns, I, Seqs...>::template Next<N> &
    MonadImpl<Fns, I, Seqs...>::next()
    {
        return *reinterpret_cast<Next<N>*>(this);
    }

    template <typename... Seqs>
    MonadImpl<std::tuple<>, 0, Seqs...> monad()
    {
        return MonadImpl<std::tuple<>, 0, Seqs...>();
    }
}

#endif // TEFRI_DETAIL_MONADIMPL_INC

#endif // TEFRI_MONAD_INC


#ifndef TEFRI_OPERATOR_H
#define TEFRI_OPERATOR_H


#ifndef TEFRI_MAPPING_OPERATOR_INC
#define TEFRI_MAPPING_OPERATOR_INC



#ifndef TEFRI_MAPPING_OPERATOR_H
#define TEFRI_MAPPING_OPERATOR_H



#ifndef TEFRI_OPERATOR_DETAIL_UNWRAP_INC
#define TEFRI_OPERATOR_DETAIL_UNWRAP_INC


#ifndef TEFRI_OPERATOR_DETAIL_UNWRAP_H
#define TEFRI_OPERATOR_DETAIL_UNWRAP_H


namespace tefri::detail
{
    template <typename T>
    auto unwrap(const T &);

    template <typename T>
    auto unwrap_ref(const T &);
}

#endif // TEFRI_OPERATOR_DETAIL_UNWRAP_H

#define DECLARE_UNWRAPER_CPY(specialization) struct Unwrapper<specialization> : DefaultUnwrapRef<Unwrapper<specialization>>
#define DECLARE_UNWRAPER_REF(specialization) struct Unwrapper<specialization>


namespace tefri::detail
{
    namespace
    {
        // TODO: specializations for Aggregation
        template <typename T>
        struct Unwrapper;

        template <typename Unwrapper>
        struct DefaultUnwrapRef
        {
            template <typename T>
            static auto unwrap_ref(const T &obj)
            {
                return Unwrapper::unwrap(obj);
            }
        };

        template <typename T>
        DECLARE_UNWRAPER_CPY(ObjectHolder<T>)
        {
            static T unwrap(const ObjectHolder<T> &obj)
            {
                return obj.get_copy();
            }
        };
    }

    template <typename T>
    auto unwrap(const T &obj)
    {
        return Unwrapper<T>::unwrap(obj);
    }

    template <typename T>
    auto unwrap_ref(const T &obj)
    {
        return Unwrapper<T>::unwrap_ref(obj);
    }
}

#undef DECLARE_UNWRAPER_REF
#undef DECLARE_UNWRAPER_CPY

#endif // TEFRI_OPERATOR_DETAIL_UNWRAP_INC

namespace tefri
{
    struct MappingOperatorTag {};

    template <typename Callable>
    class Mapping : public MappingOperatorTag
    {
    public:
        Mapping();

        Mapping(const Mapping &);

        Mapping(Mapping &&);

        Mapping(const Callable &);

        Mapping(Callable &);

        Mapping(Callable &&);

        Mapping &operator=(const Mapping &);

        Mapping &operator=(Mapping &&);

        virtual ~Mapping();

    protected:
        Callable callable;
    };
}

#endif // TEFRI_MAPPING_OPERATOR_H

namespace tefri
{
    template <typename Callable>
    Mapping<Callable>::Mapping() = default;

    template <typename Callable>
    Mapping<Callable>::Mapping(const Mapping &) = default;

    template <typename Callable>
    Mapping<Callable>::Mapping(Mapping &&) = default;

    template <typename Callable>
    Mapping<Callable>::Mapping(const Callable &callable)
    : callable(callable)
    {}

    template <typename Callable>
    Mapping<Callable>::Mapping(Callable &callable)
    : callable(callable)
    {}

    template <typename Callable>
    Mapping<Callable>::Mapping(Callable &&callable)
    : callable(std::forward<Callable>(callable))
    {}

    template <typename Callable>
    Mapping<Callable> &Mapping<Callable>::operator=(const Mapping &) = default;

    template <typename Callable>
    Mapping<Callable> &Mapping<Callable>::operator=(Mapping &&) = default;

    template <typename Callable>
    Mapping<Callable>::~Mapping() = default;
}

#endif // TEFRI_MAPPING_OPERATOR_INC

#ifndef TEFRI_MAP_OPERATOR_INC
#define TEFRI_MAP_OPERATOR_INC


#ifndef TEFRI_MAP_OPERTAOR_H
#define TEFRI_MAP_OPERTAOR_H


namespace tefri
{
    template <typename Callable>
    class Map : public Mapping<Callable>
    {
    public:
        using Mapping<Callable>::Mapping;

        template <typename Next, typename... Args>
        auto operator()(Next &&, const Args &...);
    };

    template <typename Callable>
    class MapSeq : public Mapping<Callable>
    {
    public:
        using Mapping<Callable>::Mapping;

        template <typename Next, typename... Args>
        auto operator()(Next &&, const Args &...);
    };

    template <typename Callable>
    auto map(const Callable &);

    template <typename Callable>
    auto map(Callable &);

    template <typename Callable>
    auto map(Callable &&);

    template <typename Callable>
    auto map_seq(const Callable &);

    template <typename Callable>
    auto map_seq(Callable &);

    template <typename Callable>
    auto map_seq(Callable &&);
}

#endif // TEFRI_MAP_OPERTAOR_H

namespace tefri
{
    template <typename Callable>
    template <typename Next, typename... Args>
    auto Map<Callable>::operator()(Next &&next, const Args &... args)
    {
        if constexpr(std::is_invocable_v<Callable, decltype(detail::unwrap(args))...>)
            return next(std::invoke(this->callable, detail::unwrap_ref(args)...));
    }

    template <typename Callable>
    template <typename Next, typename... Args>
    auto MapSeq<Callable>::operator()(Next &&next, const Args &... args)
    {
        if constexpr((std::is_invocable_v<Callable, decltype(detail::unwrap(args))> && ...))
            return next(std::invoke(this->callable, detail::unwrap_ref(args))...);
    }

    template <typename Callable>
    auto map(const Callable &callable)
    {
        return Map<Callable>(callable);
    }

    template <typename Callable>
    auto map(Callable &callable)
    {
        return Map<Callable>(callable);
    }

    template <typename Callable>
    auto map(Callable &&callable)
    {
        return Map<Callable>(std::forward<Callable>(callable));
    }

    template <typename Callable>
    auto map_seq(const Callable &callable)
    {
        return MapSeq<Callable>(callable);
    }

    template <typename Callable>
    auto map_seq(Callable &callable)
    {
        return MapSeq<Callable>(callable);
    }

    template <typename Callable>
    auto map_seq(Callable &&callable)
    {
        return MapSeq<Callable>(std::forward<Callable>(callable));
    }
}

#endif // TEFRI_MAP_OPERATOR_INC

#ifndef TEFRI_FILTER_OPERATOR_INC
#define TEFRI_FILTER_OPERATOR_INC


#ifndef TEFRI_FILTER_OPERATOR_H
#define TEFRI_FILTER_OPERATOR_H


namespace tefri
{
    template <typename Callable>
    class Filter : public Mapping<Callable>
    {
    public:
        using Mapping<Callable>::Mapping;

        template <typename Next, typename... Args>
        auto operator()(Next &&, Args&&...);
    };

    template <typename Callable>
    class FilterSeq : public Mapping<Callable>
    {
    public:
        using Mapping<Callable>::Mapping;

        template <typename Next, typename... Args>
        auto operator()(Next &&, Args&&...);
    };

    template <typename Callable>
    auto filter(const Callable &);

    template <typename Callable>
    auto filter(Callable &);

    template <typename Callable>
    auto filter(Callable &&);

    template <typename Callable>
    auto filter_seq(const Callable &);

    template <typename Callable>
    auto filter_seq(Callable &);

    template <typename Callable>
    auto filter_seq(Callable &&);
}

#endif // TEFRI_FILTER_OPERATOR_H

namespace tefri
{
    template <typename Callable>
    template <typename Next, typename... Args>
    auto Filter<Callable>::operator()(Next &&next, Args&&... args)
    {
        if constexpr(std::is_invocable_v<Callable, decltype(detail::unwrap(args))...>)
        {
            if(std::invoke(this->callable, detail::unwrap_ref(args)...))
                return next(std::forward<Args>(args)...);
        }
    }

    template <typename Callable>
    template <typename Next, typename... Args>
    auto FilterSeq<Callable>::operator()(Next &&next, Args&&... args)
    {
        if constexpr((std::is_invocable_v<Callable, decltype(detail::unwrap(args))> && ...))
        {
            if((std::invoke(this->callable, detail::unwrap_ref(args)) && ...))
                return next(std::forward<Args>(args)...);
        }
    }

    template <typename Callable>
    auto filter(const Callable &callable)
    {
        return Filter<Callable>(callable);
    }

    template <typename Callable>
    auto filter(Callable &callable)
    {
        return Filter<Callable>(callable);
    }

    template <typename Callable>
    auto filter(Callable &&callable)
    {
        return Filter<Callable>(std::forward<Callable>(callable));
    }

    template <typename Callable>
    auto filter_seq(const Callable &callable)
    {
        return FilterSeq<Callable>(callable);
    }

    template <typename Callable>
    auto filter_seq(Callable &callable)
    {
        return FilterSeq<Callable>(callable);
    }

    template <typename Callable>
    auto filter_seq(Callable &&callable)
    {
        return FilterSeq<Callable>(std::forward<Callable>(callable));
    }
}

#endif // TEFRI_FILTER_OPERATOR_INC

#endif // TEFRI_OPERATOR_H


#endif // TEFRI_H


#endif // TEFRI_HPP