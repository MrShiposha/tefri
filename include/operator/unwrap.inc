#ifndef TEFRI_OPERATOR_DETAIL_UNWRAP_INC
#define TEFRI_OPERATOR_DETAIL_UNWRAP_INC

#include "unwrap.h"

#define DECLARE_UNWRAPER_CPY(specialization) struct Unwrapper<specialization> : DefaultUnwrapRef<Unwrapper<specialization>>
#define DECLARE_UNWRAPER_REF(specialization) struct Unwrapper<specialization>


namespace tefri
{
    namespace
    {
        template <typename T>
        struct Unwrapper;

        template <typename Unwrapper>
        struct DefaultUnwrapRef
        {
            template <typename T>
            static auto unwrap_ref(const T &obj)
            {
                return Unwrapper::unwrap(obj);
            }
        };

        template <typename T>
        DECLARE_UNWRAPER_CPY(ObjectHolder<T>)
        {
            static T unwrap(const ObjectHolder<T> &obj)
            {
                return obj.get_copy();
            }
        };
    }

    template <typename T>
    auto unwrap(const T &obj)
    {
        return Unwrapper<T>::unwrap(obj);
    }

    template <typename T>
    auto unwrap_ref(const T &obj)
    {
        return Unwrapper<T>::unwrap_ref(obj);
    }
}

#undef DECLARE_UNWRAPER_REF
#undef DECLARE_UNWRAPER_CPY

#endif // TEFRI_OPERATOR_DETAIL_UNWRAP_INC