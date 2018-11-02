
//
// Created by Daniel Shiposha on 12 August 2018
// 



#ifndef METAXXA_MOVEFUNCTIONARGUMENTTYPES_H
#define METAXXA_MOVEFUNCTIONARGUMENTTYPES_H

#include "Function.h"

namespace metaxxa
{
	namespace implementation
	{
		template
		<
			template<typename...> typename TemplateType, 
			typename Callable, 
			size_t ARGUMENT_COUNT, 
			bool ENOUGH, 
			size_t... INDICES
		>
		struct MoveFunctionArgumentTypesProxy
			: MoveFunctionArgumentTypesProxy
			<
				TemplateType,
				Callable,
				ARGUMENT_COUNT,
				sizeof...(INDICES)+1 == ARGUMENT_COUNT,
				INDICES...,
				sizeof...(INDICES)
			>
		{
		};

		template
		<
			template<typename...> typename TemplateType, 
			typename Callable, size_t ARGUMENT_COUNT, 
			size_t... INDICES
		>
		struct MoveFunctionArgumentTypesProxy<TemplateType, Callable, ARGUMENT_COUNT, true, INDICES...>
		{
			typedef TemplateType<typename ::metaxxa::Function<Callable>::template Argument<INDICES>...> Type;
		};

		template<template<typename...> typename TemplateType, typename Callable>
		struct MoveFunctionArgumentTypes
		{
			using Type = typename MoveFunctionArgumentTypesProxy
			<
				TemplateType,
				Callable,
				::metaxxa::Function<Callable>::argument_count(),
				::metaxxa::Function<Callable>::argument_count() == 0
			>::Type;
		};

	}

	template<template<typename...> typename TemplateType, typename Callable>
	using MoveFunctionArgumentTypes = typename implementation::MoveFunctionArgumentTypes<TemplateType, Callable>::Type;
}

#endif //METAXXA_MOVEFUNCTIONARGUMENTTYPES_H