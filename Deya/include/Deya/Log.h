#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Deya
{
    class DEYA_API Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define DY_CORE_TRACE(...)      ::Deya::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DY_CORE_INFO(...)       ::Deya::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DY_CORE_WARN(...)       ::Deya::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DY_CORE_ERROR(...)      ::Deya::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DY_CORE_FATAL(...)      ::Deya::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define DY_TRACE(...)           ::Deya::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DY_INFO(...)            ::Deya::Log::GetClientLogger()->info(__VA_ARGS__)
#define DY_WARN(...)            ::Deya::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DY_ERROR(...)           ::Deya::Log::GetClientLogger()->error(__VA_ARGS__)
#define DY_FATAL(...)           ::Deya::Log::GetClientLogger()->critical(__VA_ARGS__)