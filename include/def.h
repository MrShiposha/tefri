#ifndef TEFRI_DEF_H
#define TEFRI_DEF_H

#ifdef _MSC_VER
    #define tefri_inline __forceinline
#elif defined(__GNUC__)
    #define tefri_inline inline __attribute__((__always_inline__))
#elif defined(__CLANG__)
    #if __has_attribute(__always_inline__)
        #define tefri_inline inline __attribute__((__always_inline__))
    #else
        #define tefri_inline inline
    #endif
#else
    #define tefri_inline inline
#endif

#endif // TEFRI_DEF_H