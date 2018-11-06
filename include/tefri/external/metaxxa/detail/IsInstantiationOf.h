#ifndef METAXXA_ISINSTANTIATIONOF_H
#define METAXXA_ISINSTANTIATIONOF_H

#include <type_traits>
#include <functional>

namespace metaxxa
{
    namespace detail
    {
        template <template <typename...> typename TemplateType>
        struct IsInstantiationOf
        {
            template <typename... Parameters>
            struct check : std::false_type {};

            template <typename... Parameters>
            struct check<TemplateType<Parameters...>> : std::true_type {};
        };

        template <template <typename...> typename TemplateType, typename... Parameters>
		constexpr bool IS_INSTANTIATION_OF = IsInstantiationOf<TemplateType>::template check<Parameters...>::value;
    }
}

#endif /* METAXXA_ISINSTANTIATIONOF_H */
