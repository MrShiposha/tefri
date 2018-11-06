
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_MOVESTDTUPLETYPES_H
#define METAXXA_MOVESTDTUPLETYPES_H

#include <tuple>

namespace metaxxa
{
	namespace detail
	{
		template 
		<
			template <typename...> typename TemplateType,
			typename Tuple, 
			bool ENOUGH,
			size_t INDEX,
			size_t TYPES_COUNT,
			typename... Types
		>
		struct MoveStdTupleTypesProxy : MoveStdTupleTypesProxy
		<
			TemplateType, 
			Tuple, 
			TYPES_COUNT == INDEX + 1,
			INDEX + 1,
			TYPES_COUNT,
			Types...,
			typename std::tuple_element<INDEX, Tuple>::type
		>
		{
		};

		template
		<
			template <typename...> typename TemplateType,
			typename Tuple,
			size_t INDEX,
			size_t TYPES_COUNT,
			typename... Types
		>
		struct MoveStdTupleTypesProxy<TemplateType, Tuple, true, INDEX, TYPES_COUNT, Types...>
		{
			using ResultType = TemplateType<Types...>;
		};

		template <template <typename...> typename TemplateType, typename Tuple>
		struct MoveStdTupleTypes
		{
			using ResultType = typename MoveStdTupleTypesProxy
				<
					TemplateType, 
					Tuple, 
					std::tuple_size<Tuple>::value == 1,
					1,
					std::tuple_size<Tuple>::value,
					typename std::tuple_element<0, Tuple>::type
				>::ResultType;
		};

		template <template <typename...> typename TemplateType>
		struct MoveStdTupleTypes<TemplateType, std::tuple<>>
		{
			using ResultType = TemplateType<>;
		};
	}

	template <template <typename...> typename TemplateType, typename Tuple>
	using MoveStdTupleTypes = typename detail::MoveStdTupleTypes<TemplateType, Tuple>::ResultType;
}

#endif // METAXXA_MOVESTDTUPLETYPES_H