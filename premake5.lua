workspace "Deya"
    architecture "x64"
    startproject "Sandbox"

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

include "Deya/vendor/GLFW" -- Includes GLFWs premake project (in my fork)
include "Deya/vendor/glad" -- Includes glads premake project (in my fork)

project "Deya"
    location "Deya"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "dypch.h"
    pchsource "Deya/src/dypch.cpp" -- !WINDOWS ONLY (will be ignored on other platforms)

    files
    {
        "Deya/include/**.h",
        "Deya/src/**.cpp"
    }

    includedirs
    {
        "Deya/include",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}"
    }

    links -- !GLOBAL
    {
        "GLFW",
        "glad"
    }


    filter "system:windows"
        cppdialect "c++17"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_WINDOWS",
            "DY_BUILD_DLL"
        }    

        postbuildcommands
        {
            ("mkdir %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"),
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
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
        symbols "On"
        
    filter "configurations:Release"
        defines "DY_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "DY_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

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
        "Deya/include"
    }

    links
    {
        "Deya"
    }

    filter "system:windows"
        cppdialect "c++17"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_WINDOWS",
        }

    filter "system:linux"
        cppdialect "c++17"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_LINUX"
        }
        
    filter "configurations:Debug"
        defines "DY_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DY_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "DY_DIST"
        optimize "On"