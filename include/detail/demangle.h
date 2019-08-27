#ifndef TEFRI_DEMANGLE_H
#define TEFRI_DEMANGLE_H

#include <string>
#include <typeinfo>

#ifndef _MSC_VER
#   include <cxxabi.h>
#endif // _MSC_VER

namespace tefri::detail
{
    template <typename T>
    std::string demangle_name();
}

#endif // TEFRI_DEMANGLE_H