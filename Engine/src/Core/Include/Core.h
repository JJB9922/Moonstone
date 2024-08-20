#ifndef CORE_H
#define CORE_H

#include "Core/Include/Logger.h"
#include "mspch.h"

#define MS_OPENGL
//#define MS_VULKAN

#define MS_ENABLE_ASSERTS

#ifdef MS_ENABLE_ASSERTS
#define MS_ASSERT(x, ...)                                                                          \
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

#endif // CORE_H
