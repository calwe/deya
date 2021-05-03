#pragma once

#ifdef DY_PLATFORM_WINDOWS
    #ifdef DY_DYNAMIC_LINK
        #ifdef DY_BUILD_DLL
            #define DEYA_API __declspec(dllexport)
        #else
            #define DEYA_API __declspec(dllimport)
        #endif
    #else
        #define DEYA_API
    #endif
#endif

#ifdef DY_DEBUG
    #define DY_ENABLE_ASSERTS
#endif

#ifdef DY_ENABLE_ASSERTS
    #define DY_ASSERT(x, ...) { if(!(x)) { DY_ERROR("Assertion Failed: {0}", __VA_ARGS__); } }
    #define DY_CORE_ASSERT(x, ...) { if(!(x)) { DY_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); } }
#else
    #define DY_ASSERT(x, ...)
    #define DY_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define DY_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)