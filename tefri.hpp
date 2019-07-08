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

#ifndef TEFRI_TUPLE_INC
#define TEFRI_TUPLE_INC


#ifndef TEFRI_TUPLE_H
#define TEFRI_TUPLE_H



namespace tefri
{
    namespace detail
    {
        struct ShareTuple {};
    }

    template 
    <
        template <typename, typename...> typename,
        typename...
    >
    class GeneralTuple;

    template <template <typename, typename...> typename PtrContainer>
    class DraftTuple
    {
    public:
        template <typename... Types>
        using Tuple = ::tefri::GeneralTuple<PtrContainer, Types...>;
    };

    template <typename... Types>
    using Tuple = typename DraftTuple<std::vector>::template Tuple<Types...>;

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

        template <std::size_t FROM, std::size_t TO>
        auto take_range() const 
            -> metaxxa::TakeRange
            <
                DraftTuple<PtrContainer>::template Tuple,
                TypeTuple,
                FROM, TO
            >;

        template <std::size_t FROM, std::size_t TO>
        auto take_range_shared() const 
            -> metaxxa::TakeRange
            <
                DraftTuple<PtrContainer>::template Tuple,
                TypeTuple,
                FROM, TO
            >;

        template <std::size_t N>
        auto take_first() const 
            -> metaxxa::TakeFirst
            <
                DraftTuple<PtrContainer>::template Tuple,
                TypeTuple,
                N
            >;

        template <std::size_t N>
        auto take_first_shared() const 
            -> metaxxa::TakeFirst
            <
                DraftTuple<PtrContainer>::template Tuple,
                TypeTuple,
                N
            >;

        template <std::size_t N>
        auto take_last() const 
            -> metaxxa::TakeLast
            <
                DraftTuple<PtrContainer>::template Tuple,
                TypeTuple,
                N
            >;

        template <std::size_t N>
        auto take_last_shared() const 
            -> metaxxa::TakeLast
            <
                DraftTuple<PtrContainer>::template Tuple,
                TypeTuple,
                N
            >;

        template <std::size_t N>
        auto skip_first() const 
            -> metaxxa::SkipFirst
            <
                DraftTuple<PtrContainer>::template Tuple,
                TypeTuple,
                N
            >;

        template <std::size_t N>
        auto skip_first_shared() const 
            -> metaxxa::SkipFirst
            <
                DraftTuple<PtrContainer>::template Tuple,
                TypeTuple,
                N
            >;

        template <std::size_t N>
        auto skip_last() const 
            -> metaxxa::SkipLast
            <
                DraftTuple<PtrContainer>::template Tuple,
                TypeTuple,
                N
            >;

        template <std::size_t N>
        auto skip_last_shared() const 
            -> metaxxa::SkipLast
            <
                DraftTuple<PtrContainer>::template Tuple,
                TypeTuple,
                N
            >;

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

        template <typename Callable>
        auto apply(const Callable &) const;

        template <typename Callable>
        auto apply(Callable &) const;

        template <typename Callable>
        auto apply(const Callable &);

        template <typename Callable>
        auto apply(Callable &);

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

        template <typename Callable>
        auto apply(const Callable &) const;

        template <typename Callable>
        auto apply(Callable &) const;

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
        return const_cast<GeneralTuple*>(this)->template get<INDEX>();
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
    template <std::size_t FROM, std::size_t TO>
    auto GeneralTuple<PtrContainer, Args...>::take_range() const 
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
        typename... Args
    >
    template <std::size_t FROM, std::size_t TO>
    auto GeneralTuple<PtrContainer, Args...>::take_range_shared() const 
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
        typename... Args
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Args...>::take_first() const 
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
        typename... Args
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Args...>::take_first_shared() const 
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
        typename... Args
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Args...>::take_last() const 
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
        typename... Args
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Args...>::take_last_shared() const 
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
        typename... Args
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Args...>::skip_first() const 
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
        typename... Args
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Args...>::skip_first_shared() const 
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
        typename... Args
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Args...>::skip_last() const 
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
        typename... Args
    >
    template <std::size_t N>
    auto GeneralTuple<PtrContainer, Args...>::skip_last_shared() const 
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

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <typename Callable>
    auto GeneralTuple<PtrContainer, Args...>::apply(const Callable &callable) const
    {
        return detail::apply<const GeneralTuple<PtrContainer, Args...> &, const Callable &>(*this, callable, std::make_index_sequence<size()>());
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <typename Callable>
    auto GeneralTuple<PtrContainer, Args...>::apply(Callable &callable) const
    {
        return detail::apply<const GeneralTuple<PtrContainer, Args...> &, Callable &>(*this, callable, std::make_index_sequence<size()>());
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <typename Callable>
    auto GeneralTuple<PtrContainer, Args...>::apply(const Callable &callable)
    {
        return detail::apply<GeneralTuple<PtrContainer, Args...> &, const Callable &>(*this, callable, std::make_index_sequence<size()>());
    }

    template 
    <
        template <typename, typename...> typename PtrContainer,
        typename... Args
    >
    template <typename Callable>
    auto GeneralTuple<PtrContainer, Args...>::apply(Callable &callable)
    {
        return detail::apply<GeneralTuple<PtrContainer, Args...> &, Callable &>(*this, callable, std::make_index_sequence<size()>());
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

#ifndef TEFRI_MONAD_INC
#define TEFRI_MONAD_INC


#ifndef TEFRI_MONAD_H
#define TEFRI_MONAD_H



#ifndef TEFRI_MONADBASE_H
#define TEFRI_MONADBASE_H


namespace tefri
{
    template <typename Monad, typename InputTupleVariants>
    class MonadBase // : public AbstractMonad<InputTupleVariants>
    {
    public:
        // TODO
    };

    template <typename Monad>
    class MonadBase<Monad, Tuple<>>
    {
    public:
        template <typename... Args>
        void operator()(Args&&... args)
        {
            *(static_cast<Monad*>(this))(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void operator()(const Args &... args)
        {
            *(static_cast<Monad*>(this))(args...);
        }
    };
}

#endif // TEFRI_MONADBASE_H

namespace tefri
{
    template <typename InputTupleVariants, typename... Functions>
    class Monad;

    namespace detail
    {
        template <typename InputTupleVariants>
        struct DraftMonad
        {
            template <typename... Functions>
            using Monad = ::tefri::Monad<InputTupleVariants, Functions...>;
        };

        template <typename Monad, typename... Args>
        struct Invoker;

        struct Unspecified {};
    }

    template <typename InputTupleVariants, typename... Functions>
    class Monad final : public MonadBase<Monad<InputTupleVariants, Functions...>, InputTupleVariants>
    {
        using FunctionsTuple    = Tuple<Functions...>;
        using FunctionsTuplePtr = std::shared_ptr<FunctionsTuple>;
    public:
        using Base = MonadBase<Monad<InputTupleVariants, Functions...>, InputTupleVariants>;

        template <std::size_t N>
        using NextMonad = metaxxa::TakeRange
        <
            detail::DraftMonad<InputTupleVariants>::template Monad,
            metaxxa::TypeTuple<Functions...>,
            N, sizeof...(Functions)
        >;

        Monad();

        Monad(const Monad &);

        Monad(Monad &&);

        ~Monad();

        Monad &operator=(const Monad &);

        Monad &operator=(Monad &&);

        Monad(FunctionsTuplePtr functions);

        Monad(FunctionsTuple &&functions);

        template <typename Function>
        auto operator>>(Function &&) && 
            -> Monad<InputTupleVariants, Functions..., Function>;

        template <typename Function>
        auto operator>>(Function &) && 
            -> Monad<InputTupleVariants, Functions..., Function>;

        template <typename... Args>
        void operator()(const Args &... args);

        template <std::size_t N>
        auto next() -> NextMonad<N>;

    private:
        template <typename Variants>
        friend auto monad() -> Monad<Variants>;

        template <typename Monad, typename... Args>
        friend struct detail::Invoker;

        FunctionsTuplePtr functions;
    };

    template <typename InputTupleVariants = metaxxa::TypeTuple<>>
    auto monad() -> Monad<InputTupleVariants>;
}

#endif // TEFRI_MONAD_H

namespace tefri
{
    namespace detail
    {
        template <typename T>
        using HoldType = ObjectHolder
        <
            typename metaxxa::If<metaxxa::is_array_of<T, char>()>
                ::template Then<std::string>
                ::template Else<T>
                ::Type
        >;

        template <typename Monad, typename... Args>
        struct Invoker
        {
        public:
            static void invoke(Monad &monad, const Args &... args)
            {
                auto hold = [](const auto &arg)
                { 
                    using Arg = std::remove_cv_t<std::remove_reference_t<decltype(arg)>>;

                    if constexpr (metaxxa::is_instatiation_of<Arg, ObjectHolder>())
                        return arg;
                    else
                        return HoldType<Arg> {arg}; 
                };

                using FunctionsType = typename Monad::FunctionsTuple;

                if constexpr 
                (
                    std::is_invocable_v
                    <
                        std::tuple_element_t<0, FunctionsType>,
                        decltype(monad.template next<1>()),
                        decltype(hold(args))...
                    >
                ) monad.functions->template get<0>()(monad.template next<1>(), hold(args)...);
            }
        };

        template <typename InputTupleVariants, typename... Args>
        struct Invoker<Monad<InputTupleVariants>, Args...>
        {
        public:
            static void invoke(Monad<InputTupleVariants> &, const Args &...)
            {}
        };
    }

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad() = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(const Monad &) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(Monad &&) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::~Monad() = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...> &Monad<Types, Functions...>::operator=(const Monad &) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...> &Monad<Types, Functions...>::operator=(Monad &&) = default;

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(FunctionsTuplePtr functions)
        : functions(functions)
    {}

    template <typename Types, typename... Functions>
    Monad<Types, Functions...>::Monad(FunctionsTuple &&functions)
        : functions(std::make_shared<FunctionsTuple>(std::forward<FunctionsTuple>(functions)))
    {}

    template <typename Types, typename... Functions>
    template <typename Function>
    auto Monad<Types, Functions...>::operator>>(Function &&function) &&
        -> Monad<Types, Functions..., Function>
    {
        return Monad<Types, Functions..., Function>(functions->template push_back<Function>(function));
    }

    template <typename Types, typename... Functions>
    template <typename Function>
    auto Monad<Types, Functions...>::operator>>(Function &function) &&
        -> Monad<Types, Functions..., Function>
    {
        return Monad<Types, Functions..., Function>(functions->template push_back<Function>(function));
    }

    template <typename Types, typename... Functions>
    template <typename... Args>
    void Monad<Types, Functions...>::operator()(const Args &... args)
    {
        detail::Invoker<Monad<Types, Functions...>, Args...>
            ::invoke(*this, args...);
    }

    template <typename Types, typename... Functions>
    template <std::size_t N>
    auto Monad<Types, Functions...>::next() 
        -> NextMonad<N>
    {
        return NextMonad<N>(functions->template take_range_shared<N, decltype(functions)::element_type::size()>());
    }

    template <typename InputTupleVariants>
    auto monad() -> Monad<InputTupleVariants>
    {
        return Monad<InputTupleVariants>
        (
            std::make_shared<typename Monad<InputTupleVariants>::FunctionsTuple>()
        );
    }
}

#endif // TEFRI_MONAD_INC


#ifndef TEFRI_OPERATOR_H
#define TEFRI_OPERATOR_H


#ifndef TEFRI_MAPPING_OPERATOR_INC
#define TEFRI_MAPPING_OPERATOR_INC



#ifndef TEFRI_MAPPING_OPERATOR_H
#define TEFRI_MAPPING_OPERATOR_H


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
        void operator()(Next &&, const Args &...);
    };

    template <typename Callable>
    class MapSeq : public Mapping<Callable>
    {
    public:
        using Mapping<Callable>::Mapping;

        template <typename Next, typename... Args>
        void operator()(Next &&, const Args &...);
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
    void Map<Callable>::operator()(Next &&next, const Args &... args)
    {
        if constexpr(std::is_invocable_v<Callable, decltype(args.get_copy())...>)
            next(std::invoke(this->callable, args.get_ref()...));
    }

    template <typename Callable>
    template <typename Next, typename... Args>
    void MapSeq<Callable>::operator()(Next &&next, const Args &... args)
    {
        if constexpr((true && ... && std::is_invocable_v<Callable, decltype(args.get_copy())>))
            next(std::invoke(this->callable, args.get_ref())...);
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
        void operator()(Next &&, const Args &...);
    };

    template <typename Callable>
    class FilterSeq : public Mapping<Callable>
    {
    public:
        using Mapping<Callable>::Mapping;

        template <typename Next, typename... Args>
        void operator()(Next &&, const Args &...);
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
    void Filter<Callable>::operator()(Next &&next, const Args &... args)
    {
        if constexpr(std::is_invocable_v<Callable, decltype(args.get_copy())...>)
        {
            if(std::invoke(this->callable, args.get_ref()...))
                next(args...);
        }
    }

    template <typename Callable>
    template <typename Next, typename... Args>
    void FilterSeq<Callable>::operator()(Next &&next, const Args &... args)
    {
        if constexpr((true && ... && std::is_invocable_v<Callable, decltype(args.get_copy())>))
        {
            if((true && ... && std::invoke(this->callable, args.get_ref())))
                next(args...);
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