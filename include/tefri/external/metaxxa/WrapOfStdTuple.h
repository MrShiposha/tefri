#ifndef METAXXA_WRAPALLOFSTDTUPLE_H
#define METAXXA_WRAPALLOFSTDTUPLE_H

#include <tuple>

namespace metaxxa
{
    namespace detail
    {
        template <typename Tuple, template <typename> typename TemplateClass, size_t INDEX>
        constexpr auto wrap_of_std_tuple(Tuple &tuple)
        {
            if constexpr (INDEX == std::tuple_size_v<Tuple>)
                return std::tuple<>();
            else
                return std::tuple_cat
                (
                    std::tuple<TemplateClass<std::tuple_element_t<INDEX, Tuple>>>(std::get<INDEX>(tuple)),
                    wrap_of_std_tuple<Tuple, TemplateClass, INDEX + 1>(tuple)
                );
        }

        template <typename Tuple, template <typename> typename TemplateClass, size_t INDEX>
        constexpr auto wrap_of_std_tuple_types()
        {
            if constexpr (INDEX == std::tuple_size_v<Tuple>)
                return std::tuple<>();
            else
                return std::tuple_cat
                (
                    std::tuple<TemplateClass<std::tuple_element_t<INDEX, Tuple>>>(std::declval<TemplateClass<std::tuple_element_t<INDEX, Tuple>>>()),
                    wrap_of_std_tuple_types<Tuple, TemplateClass, INDEX + 1>()
                );
        }
    }

    template <typename Tuple, template <typename> typename TemplateType>
    constexpr auto wrap_of_std_tuple(Tuple &tuple)
    {
        return detail::wrap_of_std_tuple<Tuple, TemplateType, 0>(tuple);
    }

    template <typename Tuple, template <typename> typename TemplateType>
    constexpr auto wrap_of_std_tuple_types()
    {
        return detail::wrap_of_std_tuple_types<Tuple, TemplateType, 0>();
    }
}

#endif // METAXXA_WRAPALLOFSTDTUPLE_H