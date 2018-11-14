#ifndef TEFRI_DETAIL_ANY_H
#define TEFRI_DETAIL_ANY_H

#if __has_include(<any>)
    #include <any>

    #define TEFRI_ANY std::any
    

#elif __has_include(<experimental/any>)
    #include <experimental/any>

    #define TEFRI_ANY std::experimental::any
#else
    #error METAXXA: Sorry, your compiler does not support neither std::any or std::experimental::any
#endif // Check any

#endif // TEFRI_DETAIL_ANY_H