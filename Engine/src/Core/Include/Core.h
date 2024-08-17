#ifndef CORE_H
#define CORE_H

#define MS_ENABLE_ASSERTS

#ifdef MS_ENABLE_ASSERTS
#define MS_ASSERT(x, ...)                                                                          \
    {                                                                                              \
        if (!(x))                                                                                  \
        {                                                                                          \
            std::cerr << "assert failed: " << __VA_ARGS__ << '\n';                                 \
            __builtin_trap();                                                                      \
        }                                                                                          \
    }
#endif

#endif // CORE_H
