#pragma once

#ifdef DY_PLATFORM_WINDOWS
    #ifdef DY_BUILD_DLL
        #define DEYA_API __declspec(dllexport)
    #else
        #define DEYA_API __declspec(dllimport)
    #endif
#else
    #ifdef DY_PLATFORM_LINUX
        #define DEYA_API
    #else
        #error "Deya only support windows and linux"
    #endif
#endif

#ifdef DY_ENABLE_ASSERTS
    #define DY_ASSERT(x, ...) { if(!(x)) { DY_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define DY_CORE_ASSERT(x, ...) { if(!(x)) { DY_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define DY_ASSERT(x, ...)
    #define DY_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)