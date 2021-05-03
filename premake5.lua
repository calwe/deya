workspace "Deya"
    architecture "x64"
    startproject "Sandbox"
    cppdialect "C++17"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Deya/vendor/GLFW/include"
IncludeDir["glad"] = "Deya/vendor/glad/include"
IncludeDir["ImGui"] = "Deya/vendor/imgui"
IncludeDir["glm"] = "Deya/vendor/glm"

include "Deya/vendor/GLFW" -- Includes GLFWs premake project (in my fork)
include "Deya/vendor/glad" -- Includes glads premake project (custom)
include "Deya/vendor/imgui" -- Includes ImGuis premake project (in my fork)

project "Deya"
    location "Deya"  
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "dypch.h"
    pchsource "Deya/src/dypch.cpp" -- !WINDOWS ONLY (will be ignored on other platforms)

    files
    {
        "Deya/include/**.h",
        "Deya/src/**.cpp",
        "Deya/vendor/glm/glm/**.hpp",
        "Deya/vendor/glm/glm/**.inl"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS" -- * suppress depricated function warnings (MSVC)
    }

    includedirs
    {
        "Deya/include",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links -- !GLOBAL
    {
        "GLFW",
        "glad",
        "ImGui"
    }


    filter "system:windows"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_WINDOWS",
            "DY_BUILD_DLL"
        }

        links -- !WINDOWS ONLY
        {
            "opengl32.lib"
        }


    filter "system:linux"
        cppdialect "c++17"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_LINUX"
        }

        links -- !LINUX ONLY
        {
            "GL",
            "pthread"
        }
      
        
    filter "configurations:Debug"
        defines "DY_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "DY_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DY_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++17"
    staticruntime "om"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Sandbox/include/**.h",
        "Sandbox/src/**.cpp",
    }

    includedirs
    {
        "Deya/vendor/spdlog/include",
        "Deya/include",
        "Deya/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Deya"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_WINDOWS",
        }

    filter "system:linux"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_LINUX"
        }
        
    filter "configurations:Debug"
        defines "DY_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "DY_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DY_DIST"
        runtime "Release"
        optimize "on"