workspace "Deya"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Deya"
    location "Deya"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Deya/include/**.h",
        "Deya/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/include/**",
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_WINDOWS",
            "DY_BUILD_DLL"
        }    

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "system:linux"
        cppdialect "c++17"
        staticruntime "On"
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

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Sandbox/include/**.h",
        "Sandbox/src/**.cpp"
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
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_WINDOWS",
        }

    filter "system:linux"
        cppdialect "c++17"
        staticruntime "On"
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