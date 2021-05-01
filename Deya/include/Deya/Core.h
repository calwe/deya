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
        #define DEYA_API
    #endif
#endif
#define BIT(x) (1 << x)