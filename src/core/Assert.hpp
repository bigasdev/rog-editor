#pragma once

#include <iostream>

#ifdef _DEBUG
#endif

#ifdef _ASSERTS
#define R_ASSERT(x) { \
    if(!(x)){ \
        std::cerr << "Assertion Failed: " << #x << std::endl;  \
        __debugbreak(); \
    }\
}
#define R_TEST(x) { std::cerr << "Test: " << #x << std::endl; }
#else
#define R_ASSERT(x)
#define R_TEST(x)
#endif
