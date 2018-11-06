
//
// Created by Daniel Shiposha on 12 August 2018
// 

#ifndef METAXXA_H
#define METAXXA_H

#if defined(_MSC_VER)
    #define METAXXA_TEMPLATE
#elif defined(__clang__)
    #define METAXXA_TEMPLATE template
#else
    #ifndef METAXXA_TEMPLATE
        #error You are using an unknown compiler. Please specify the METAXXA_TEMPLATE macro
    #endif // METAXXA_TEMPLATE
#endif // __windows__

#include "Tuple.h"
#include "Type.h"
#include "Function.h"
#include "CallableToMethod.h"

#endif // METAXXA_H