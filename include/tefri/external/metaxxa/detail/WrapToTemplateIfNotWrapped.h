#ifndef METAXXA_WRAPTOTEMPLATEIFNOTWRAPPED_H
#define METAXXA_WRAPTOTEMPLATEIFNOTWRAPPED_H

#include <optional>
#include "IsInstantiationOf.h"

namespace metaxxa
{
    namespace detail
    {
        template <template <typename...> typename Template, typename TypeToWrap, bool IS_WRAPPED = IS_INSTANTIATION_OF<Template, TypeToWrap>>
        struct WrapToTemplateIfNotWrapped
        {};

        template <template <typename...> typename Template, typename TypeToWrap>
        struct WrapToTemplateIfNotWrapped<Template, TypeToWrap, false> 
        {
            using Result = Template<TypeToWrap>;
        };

        template <template <typename...> typename Template, typename TypeToWrap>
        struct WrapToTemplateIfNotWrapped<Template, TypeToWrap, true> 
        {
            using Result = TypeToWrap;
        };
    }
}

#endif // METAXXA_WRAPTOTEMPLATEIFNOTWRAPPED_H