
//
// Created by Daniel Shiposha on 12 August 2018
// 


#ifndef METAXXA_CONVERTTOTUPLEIFISNOTTUPLE_H
#define METAXXA_CONVERTTOTUPLEIFISNOTTUPLE_H

#include "OptionalType.h"
#include "Tuple.h"

namespace metaxxa
{
	template <typename SomeType>
	using ConvertToTupleIfIsNotTuple = typename util::OptionalType
		<
			SomeType,
			util::Type<SomeType>::is_tuple()
		>::template TypeOr<util::Tuple<SomeType>>;
}

#endif // METAXXA_CONVERTTOTUPLEIFISNOTTUPLE_H