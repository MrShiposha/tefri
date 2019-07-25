#ifndef TEFRI_OPERATOR_DETAIL_UNWRAP_H
#define TEFRI_OPERATOR_DETAIL_UNWRAP_H

#include "../../objectholder.h"

namespace tefri::detail
{
    template <typename T>
    auto unwrap(const T &);

    template <typename T>
    auto unwrap_ref(const T &);
}

#endif // TEFRI_OPERATOR_DETAIL_UNWRAP_H