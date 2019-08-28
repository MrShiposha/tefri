#ifndef TEFRI_DEMANGLE_INC
#define TEFRI_DEMANGLE_INC

#include <stdexcept>
#include <string_view>
#include <cstring>
#include <iostream>

#include "demangle.h"

#include "../../lib/metaxxa/metaxxa.hpp"

namespace tefri::detail
{
    template <typename T>
    std::string demangle_name()
    {
#   ifdef _MSC_VER
        return typeid(T).name();
#   else
        const char *mangled = typeid(metaxxa::TypeSaver<T>).name();
        int status = 0;

        const char *name = abi::__cxa_demangle(mangled, 0, 0, &status);
        switch(status)
        {
        case 0:
            break;
        case -1:
            throw std::bad_alloc();
        case -2:
            throw std::runtime_error(std::string(mangled) + ": not mangled name");
        default:
            throw std::runtime_error("Unknown error with demangle name");
        }

        std::string_view name_view(name);

        std::size_t prefix_l = strlen("metaxxa::TypeSaver<");
        std::size_t suffix_l = name_view.size() - name_view.find_last_of('>');
        name_view.remove_prefix(prefix_l);
        name_view.remove_suffix(suffix_l);
        
        std::string result(name_view.begin(), name_view.end());
        free(const_cast<void*>(static_cast<const void*>(name)));

        return result;
#   endif // _MSC_VER
    }
}

#endif // TEFRI_DEMANGLE_INC
