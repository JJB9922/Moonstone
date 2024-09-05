#ifndef CORE_H
#define CORE_H

#include "Core/Include/Logger.h"
#include "mspch.h"

#ifdef _WIN32
    #define MS_PLATFORM_WINDOWS
#elif defined(__linux__)
    #define MS_PLATFORM_LINUX
#endif

#define MS_OPENGL
//#define MS_VULKAN

#define MS_ENABLE_ASSERTS

#ifdef MS_ENABLE_ASSERTS
    #ifdef MS_PLATFORM_WINDOWS
        #define MS_ASSERT(x, ...)                                                                      \
        {                                                                                              \
            if (!(x))                                                                                  \
            {                                                                                          \
                std::stringstream ss;                                                                  \
                ss << "assert failed: " << __VA_ARGS__ << '\n';                                        \
                MS_ERROR(ss.str());                                                                    \
                __debugbreak();                                                                        \
            }                                                                                          \
        }
    #elif defined(MS_PLATFORM_LINUX)
        #define MS_ASSERT(x, ...)                                                                      \
        {                                                                                              \
            if (!(x))                                                                                  \
            {                                                                                          \
                std::stringstream ss;                                                                  \
                ss << "assert failed: " << __VA_ARGS__ << '\n';                                        \
                MS_ERROR(ss.str());                                                                    \
                __builtin_trap();                                                                      \
            }                                                                                          \
        }
    #endif
#endif

#endif // CORE_H