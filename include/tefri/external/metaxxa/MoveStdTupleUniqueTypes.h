
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_MOVESTDTUPLEUNIQUETYPES_H
#define METAXXA_MOVESTDTUPLEUNIQUETYPES_H

#include "ContainsTypesOfStdTuple.h"

namespace metaxxa
{
	namespace implementation
	{
		template <typename UniqueTuple, typename Type>
		constexpr bool is_unique_type()
		{
			if constexpr (std::tuple_size_v<UniqueTuple> == 0)
				return true;

			else
				return !::metaxxa::contains_type
					<
						UniqueTuple,
						Type
					>();
		}

		template
		<
			template <typename...> typename TemplateType,
			typename StdTuple,
			bool IS_UNIQUE = false,
			size_t INDEX = std::tuple_size_v<StdTuple>,
			size_t... CORRECT_INDICES
		>
		struct MoveStdTupleUniqueTypesProxy
			: MoveStdTupleUniqueTypesProxy
			<
				TemplateType,
				StdTuple,
				is_unique_type
					<
						std::tuple<typename std::tuple_element_t<CORRECT_INDICES, StdTuple>...>,
						std::tuple_element_t<INDEX - 1, StdTuple>
					>(),
				INDEX - 1,
				CORRECT_INDICES...
			>
		{
		};

		template
		<
			template <typename...> typename TemplateType,
			typename StdTuple,
			size_t INDEX,
			size_t... CORRECT_INDICES
		>
		struct MoveStdTupleUniqueTypesProxy
			<
				TemplateType,
				StdTuple,
				true,
				INDEX,
				CORRECT_INDICES...
			>
			: MoveStdTupleUniqueTypesProxy
			<
				TemplateType,
				StdTuple,
				is_unique_type
					<
						std::tuple<typename std::tuple_element_t<CORRECT_INDICES, StdTuple>...>,
						std::tuple_element_t<INDEX - 1, StdTuple>
					>(),
				INDEX - 1,
				INDEX,
				CORRECT_INDICES...
			>
			
		{
		};

		template
		<
			template <typename...> typename TemplateType,
			typename StdTuple,
			size_t... CORRECT_INDICES
		>
		struct MoveStdTupleUniqueTypesProxy
			<
				TemplateType,
				StdTuple,
				false,
				0,
				CORRECT_INDICES...
			>
		{
			using Type = TemplateType<typename std::tuple_element_t<CORRECT_INDICES, StdTuple>...>;
		};

		template
		<
			template <typename...> typename TemplateType,
			typename StdTuple,
			size_t... CORRECT_INDICES
		>
		struct MoveStdTupleUniqueTypesProxy
			<
				TemplateType,
				StdTuple,
				true,
				0,
				CORRECT_INDICES...
			>
		{
			using Type = TemplateType<typename std::tuple_element_t<0, StdTuple>, typename std::tuple_element_t<CORRECT_INDICES, StdTuple>...>;
		};


		template 
		<
			template <typename...> typename TemplateType,
			typename StdTuple
		>
		struct MoveStdTupleUniqueTypes
		{
			using Type = typename MoveStdTupleUniqueTypesProxy<TemplateType, StdTuple>::Type;
		};
	}

	template 
	<
		template <typename...> typename TemplateType,
		typename StdTuple
	>
	using MoveStdTupleUniqueTypes = typename implementation::MoveStdTupleUniqueTypes<TemplateType, StdTuple>::Type;
}

#endif // METAXXA_MOVESTDTUPLEUNIQUETYPES_H