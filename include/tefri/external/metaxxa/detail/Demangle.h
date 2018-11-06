#ifndef METAXXA_detail_DEMANGLE_H
#define METAXXA_detail_DEMANGLE_H

#include <string>

#if __has_include(<cxxabi.h>)
#define ___METAXXA___HAS_CXX_ABI
#include <cxxabi.h>
#include <memory>
#endif // __has_include(<cxxabi.h>)

namespace metaxxa::detail
{
    template <typename T>
    class CVRSaver
    {};

    template <typename T>
    std::string demangle()
    {
        static const char cvr_saver_name[] = "metaxxa::detail::CVRSaver<";
        static constexpr std::string::size_type cvr_saver_name_length = sizeof(cvr_saver_name) - 1;

    const char *begin = nullptr;
    #ifdef _MSC_VER
        // Nothing to do, already demangled
        begin = typeid(CVRSaver<T>).name();
    #elif defined(___METAXXA___HAS_CXX_ABI)
        int status;

        auto deleter = [](char *p) { free(p); };
        std::unique_ptr<char, decltype(deleter)> real_name
        (
            abi::__cxa_demangle(typeid(CVRSaver<T>).name(), 0, 0, &status),
            deleter
        );

        begin = real_name.get();

    #else
        begin = typeid(T).name().c_str();
    #endif

        const std::string::size_type length = std::strlen(begin);
        const char *end = begin + length;

        if(length > cvr_saver_name_length)
        {
            const char *b = std::strstr(begin, cvr_saver_name);
            if(b)
            {
                b += cvr_saver_name_length;

                while(*b == ' ')
                    ++b;

                const char *e = end - 1;
                while(e > b && *e != '>')
                    --e;

                while(e > b && *(e - 1) == ' ')
                    --e;

                if(b < e)
                {
                    begin = b;
                    end = e;
                }
            }
        }

        return std::string(begin, end);
    }
}

#endif // METAXXA_detail_DEMANGLE_H