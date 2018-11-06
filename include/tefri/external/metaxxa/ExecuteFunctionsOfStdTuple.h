
//
// Created by Daniel Shiposha on 12 August 2018
// 



#ifndef GGRESOURCEPACKER_TUPLEFUNCTIONSEXECUTOR_H
#define GGRESOURCEPACKER_TUPLEFUNCTIONSEXECUTOR_H


#include <tuple>

#include "detail/Function.h"
#include "detail/IsImplicitlyConstructible.h"

namespace metaxxa
{
    namespace detail
    {
        template<typename Value, typename FunctionParameter, bool IS_FUNDAMENTAL = std::is_fundamental<Value>::value>
        struct IsCorrectArgument
        {};

        template<typename Value, typename FunctionParameter>
        struct IsCorrectArgument<Value, FunctionParameter, true>
                : std::integral_constant
                            <
                                    bool,
                                    std::is_same
                                        <
                                                typename std::remove_const<typename std::remove_reference<Value>::type>::type,
                                                typename std::remove_const<typename std::remove_reference<FunctionParameter>::type>::type
                                        >::value
                            >
        {};

        template<typename Value, typename FunctionParameter>
        struct IsCorrectArgument<Value, FunctionParameter, false>
                : detail::IsImplicitlyConstructible<FunctionParameter, Value>
        {};

        template <typename Values, typename Callable, bool IS_CORRECT, bool IS_ENOUGH = false, size_t INDEX = 0, size_t... INDICES>
        struct IsCorrectArgumentsProxy
                : IsCorrectArgumentsProxy
                            <
                                    Values,
                                    Callable,
                                    IsCorrectArgument
                                    <
                                    typename std::tuple_element<INDEX, Values>::type, 
                                    typename detail::Function<Callable>::template Argument<INDEX>
                                    >::value,
                                    std::tuple_size<Values>::value == INDEX + 1,
                                    sizeof...(INDICES) + 1,
                                    INDEX
                            >
        {};

        template<typename Values, typename Callable, size_t INDEX, size_t... INDICES>
        struct IsCorrectArgumentsProxy<Values, Callable, true, true, INDEX, INDICES...>
                : std::true_type
        {};

        template<typename Values, typename Callable, bool IS_ENOUGH, size_t INDEX, size_t... INDICES>
        struct IsCorrectArgumentsProxy<Values, Callable, false, IS_ENOUGH, INDEX, INDICES...>
                : std::false_type
        {};


        template <typename Values, typename Callable, size_t VALUE_COUNT = std::tuple_size<Values>::value, size_t ARGUMENT_COUNT = detail::Function<Callable>::ARGUMENT_COUNT>
        struct IsCorrectArguments
                : IsCorrectArgumentsProxy<Values, Callable, VALUE_COUNT == ARGUMENT_COUNT>
        {};

        template <typename Values, typename Callable>
        struct IsCorrectArguments<Values, Callable, 1, 0>
                : std::integral_constant<bool, std::is_same<typename std::tuple_element<0, Values>::type, void>::value>
        {};

        template <typename Values, typename SubscribersTuple, int INDEX = -1, size_t... CORRECT_INDICES>
        struct CorrectExecutorProxy
        {
            template <typename... Arguments>
            static void execute(SubscribersTuple &tuple, Arguments&&... arguments)
            {
                /*if constexpr 
                (
                    std::is_callable_v
                    <
                        typename std::tuple_element
                        <
                            INDEX, SubscribersTuple
                        >::type,
                        Arguments...
                    >
                )*/
                {
                    std::get<INDEX>(tuple)(arguments...);
                    CorrectExecutorProxy<Values, SubscribersTuple, CORRECT_INDICES...>::execute(tuple, std::forward<Arguments>(arguments)...);
                }
            }
        };

        template <typename Values, typename SubscribersTuple, int INDEX>
        struct CorrectExecutorProxy<Values, SubscribersTuple, INDEX>
        {
            template <typename... Arguments>
            static void execute(SubscribersTuple &tuple, Arguments&&... arguments)
            {
                /*if constexpr 
                (
                    std::is_callable_v
                    <
                        typename std::tuple_element
                        <
                            INDEX, SubscribersTuple
                        >::type,
                        Arguments...
                    >
                )*/
                {
                    std::get<INDEX>(tuple)(std::forward<Arguments>(arguments)...);
                }
            }
        };

        template <typename Value, typename SubscribersTuple>
        struct CorrectExecutorProxy<Value, SubscribersTuple, -1>
        {
            template <typename... Arguments>
            static void execute(SubscribersTuple &tuple, Arguments&&... arguments)
            {
            }
        };

        template <typename Values, typename SubscribersTuple, bool IS_ENOUGH = false, size_t INDEX = 0, typename... Arguments>
        struct CorrectExecutor
        {
            static void execute(SubscribersTuple &tuple, Values &values, Arguments&&... arguments)
            {
                CorrectExecutor
                        <
                                Values,
                                SubscribersTuple,
                                INDEX == std::tuple_size<Values>::value,
                                INDEX + 1,
                                typename std::tuple_element<INDEX, Values>::type
                        >::execute
                        (
                                tuple,
                                std::forward<Arguments>(arguments)...,
                                std::get<INDEX>(values)
                        );
            }
        };

        template <typename Values, typename SubscribersTuple, size_t INDEX, typename... Arguments>
        struct CorrectExecutor<Values, SubscribersTuple, true, INDEX, Arguments...>
        {
            static void execute(SubscribersTuple &tuple, Values &values, Arguments&&... arguments)
            {
                CorrectExecutor<Values, SubscribersTuple>::execute(tuple, std::forward<Arguments>(arguments)...);
            }
        };

        ////////////////VOID//////////////////////

        template <typename SubscribersTuple, int INDEX, size_t... CORRECT_INDICES>
        struct CorrectExecutorProxy<void, SubscribersTuple, INDEX, CORRECT_INDICES...>
        {
            static void execute(SubscribersTuple &tuple)
            {
                std::get<INDEX>(tuple)();
                CorrectExecutorProxy<void, SubscribersTuple, CORRECT_INDICES...>::execute(tuple);
            }
        };

        template <typename SubscribersTuple, int INDEX>
        struct CorrectExecutorProxy<void, SubscribersTuple, INDEX>
        {
            static void execute(SubscribersTuple &tuple)
            {
                std::get<INDEX>(tuple)();
            }
        };

        template <typename SubscribersTuple>
        struct CorrectExecutorProxy<void, SubscribersTuple, -1>
        {
            static void execute(SubscribersTuple &tuple)
            {
            }
        };

        //////////////////////////



        template
                <
                        typename Values,
                        typename SubscribersTuple,
                        bool IS_CORRECT_TYPE = false,
                        size_t INDEX = std::tuple_size<SubscribersTuple>::value,
                        size_t... CORRECT_INDICES
                >
        class TupleFunctionsExecutorProxy
                : public TupleFunctionsExecutorProxy
                        <
                                Values,
                                SubscribersTuple,
                                IsCorrectArguments<Values, typename std::tuple_element<INDEX - 1, SubscribersTuple>::type>::value,
                                INDEX - 1,
                                CORRECT_INDICES...
                        >
        {
        public:

        };

        template <typename Values, typename SubscribersTuple, size_t INDEX, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<Values, SubscribersTuple, true, INDEX, CORRECT_INDICES...>
                : public TupleFunctionsExecutorProxy
                        <
                                Values,
                                SubscribersTuple,
                                IsCorrectArguments<Values, typename std::tuple_element<INDEX - 1, SubscribersTuple>::type>::value,
                                INDEX - 1,
                                INDEX,
                                CORRECT_INDICES...
                        >
        {
        public:

        };

        template <typename Values, typename SubscribersTuple, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<Values, SubscribersTuple, false, 0, CORRECT_INDICES...>
        {
        public:
            template <typename... Arguments>
            static void execute(SubscribersTuple &tuple, Arguments&&... arguments)
            {
                CorrectExecutorProxy<Values, SubscribersTuple, CORRECT_INDICES...>
                ::execute(tuple, std::forward<Arguments>(arguments)...);
            }
        };

        template <typename Values, typename SubscribersTuple, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<Values, SubscribersTuple, true, 0, CORRECT_INDICES...>
        {
        public:
            template <typename... Arguments>
            static void execute(SubscribersTuple &tuple, Arguments&&... arguments)
            {
                CorrectExecutorProxy<Values, SubscribersTuple, 0, CORRECT_INDICES...>
                ::execute(tuple, std::forward<Arguments>(arguments)...);
            }
        };


        ////////////////////////////VOID////////////////////////////////


        template
                <
                        typename SubscribersTuple,
                        bool IS_CORRECT_TYPE,
                        size_t INDEX,
                        size_t... CORRECT_INDICES
                >
        class TupleFunctionsExecutorProxy<void, SubscribersTuple, IS_CORRECT_TYPE, INDEX, CORRECT_INDICES...>
                : public TupleFunctionsExecutorProxy
                        <
                                std::tuple<void>,
                                SubscribersTuple,
                                IsCorrectArguments<std::tuple<void>, typename std::tuple_element<INDEX - 1, SubscribersTuple>::type>::value,
                                INDEX - 1,
                                CORRECT_INDICES...
                        >
        {
        public:

        };

        template <typename SubscribersTuple, size_t INDEX, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<void, SubscribersTuple, true, INDEX, CORRECT_INDICES...>
                : public TupleFunctionsExecutorProxy
                        <
                                std::tuple<void>,
                                SubscribersTuple,
                                IsCorrectArguments<std::tuple<void>, typename std::tuple_element<INDEX - 1, SubscribersTuple>::type>::value,
                                INDEX - 1,
                                INDEX,
                                CORRECT_INDICES...
                        >
        {
        public:

        };

        template <typename SubscribersTuple, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<void, SubscribersTuple, false, 0, CORRECT_INDICES...>
        {
        public:
            static void execute(SubscribersTuple &tuple)
            {
                CorrectExecutorProxy<void, SubscribersTuple, CORRECT_INDICES...>
                ::execute(tuple);
            }
        };

        template <typename SubscribersTuple, size_t... CORRECT_INDICES>
        class TupleFunctionsExecutorProxy<void, SubscribersTuple, true, 0, CORRECT_INDICES...>
        {
        public:
            static void execute(SubscribersTuple &tuple)
            {
                CorrectExecutorProxy<void, SubscribersTuple, 0, CORRECT_INDICES...>
                ::execute(tuple);
            }
        };

        template <typename SubscribersTuple, typename... Values>
        struct TupleFunctionsExecutor : public detail::TupleFunctionsExecutorProxy<std::tuple<Values...>, SubscribersTuple>
        {
        };

        template <typename SubscribersTuple>
        struct TupleFunctionsExecutor<SubscribersTuple> : public detail::TupleFunctionsExecutorProxy<std::tuple<void>, SubscribersTuple>
        {
        };
    }

    template<typename SubscribersTuple, typename... Values>
    void execute_functions(SubscribersTuple &tuple, Values... values)
    {
        detail::TupleFunctionsExecutor<SubscribersTuple, Values...>::execute(tuple, values...);
    };
}

#endif //GGRESOURCEPACKER_TUPLEFUNCTIONSEXECUTOR_H