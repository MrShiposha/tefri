#ifndef METAXXA_STDTUPLEOFOPTIONALS_H
#define METAXXA_STDTUPLEOFOPTIONALS_H

#include <tuple>
#include <optional>

namespace metaxxa
{
    template <typename... Arguments>
    using StdTupleOfOptionals = std::tuple<std::optional<Arguments>...>;
}

#endif // METAXXA_STDTUPLEOFOPTIONALS_H