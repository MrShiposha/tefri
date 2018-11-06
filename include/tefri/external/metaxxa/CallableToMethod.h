
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_CALLABLETOMETHOD_H
#define METAXXA_CALLABLETOMETHOD_H

#include "detail/Function.h"

namespace metaxxa
{
	namespace detail
	{
		template <typename SomeType, typename Callable, bool IS_ENOUGH, typename... Arguments>
		struct CallableToMethod 
			: CallableToMethod
			<
				SomeType, 
				Callable, 
				sizeof...(Arguments) + 1 >= Function<Callable>::ARGUMENT_COUNT,
				Arguments..., typename Function<Callable>::template Argument<sizeof...(Arguments)>
			>
			{
			};

		template <typename SomeType, typename Callable, typename... Arguments>
		struct CallableToMethod<SomeType, Callable, true, Arguments...>
		{
			using MethodResult = typename Function<Callable>::Result;

			using Method = MethodResult(SomeType::*)(Arguments...);
		};
	}

	template <typename SomeType, typename Callable>
	using CallableToMethod = typename detail::CallableToMethod<SomeType, Callable, detail::Function<Callable>::ARGUMENT_COUNT == 0>::Method;
}

#endif // METAXXA_CALLABLETOMETHOD_H