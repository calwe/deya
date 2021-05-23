#pragma once

#include <memory>

#ifdef DY_DYNAMIC_LINK
    #ifdef DY_BUILD_DLL
        #define DEYA_API __declspec(dllexport)
    #else
        #define DEYA_API __declspec(dllimport)
    #endif
#else
    #define DEYA_API
#endif

#ifdef DY_DEBUG
    #define DY_ENABLE_ASSERTS
    #if defined(DY_PLATFORM_WINDOWS)
        #define DY_DEBUGBREAK() __debugbreak()
    #elif defined(DY_PLATFORM_LINUX)
        #include <signal.h>
        #define DY_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't currently support debugbreak"
    #endif
#else
    #define DY_DEBUGBREAK()
#endif

// TODO: assert without message
#ifdef DY_ENABLE_ASSERTS
    #define DY_ASSERT(x, ...) { if(!(x)) { DY_ERROR("Assertion Failed: {0}", __VA_ARGS__); DY_DEBUGBREAK(); } }
    #define DY_CORE_ASSERT(x, ...) { if(!(x)) { DY_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DY_DEBUGBREAK(); } }
#else
    #define DY_ASSERT(x, ...)
    #define DY_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define DY_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Deya
{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}