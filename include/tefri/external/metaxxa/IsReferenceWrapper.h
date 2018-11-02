//
//  IsReferenceWrapper.h
//  metaxxaProject
//
//  Created by Daniel Shiposha on 14/07/2018.
//

#ifndef METAXXA_IsReferenceWrapper_h
#define METAXXA_IsReferenceWrapper_h

#include <type_traits>
#include <functional>

namespace metaxxa
{
    namespace implementation
    {
        template <typename T>
        struct IsReferenceWrapper : std::false_type {};
        
        template <typename T>
        struct IsReferenceWrapper<std::reference_wrapper<T>> : std::true_type {};
    }
    
    template <typename T>
    constexpr bool IsReferenceWrapper = implementation::IsReferenceWrapper<T>::value;
}

#endif /* METAXXA_IsReferenceWrapper_h */
