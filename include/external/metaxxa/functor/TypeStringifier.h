#ifndef METAXXA_TYPESTRINGIFIERFUCTOR_H
#define METAXXA_TYPESTRINGIFIERFUCTOR_H

#include "Type.h"

namespace metaxxa
{
	namespace functor
	{
		template <typename SomeType>
		struct TypeStringifier
		{
			TypeStringifier(std::string &result)
				: result(result)
			{
			}

			void operator()()
			{
				if (!result.empty())
					result += ", ";
				result += Type<SomeType>::name();
			}

		private:
			std::string &result;
		};
	}
}

#endif // METAXXA_TYPESTRINGIFIERFUCTOR_H