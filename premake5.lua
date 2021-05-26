workspace "Deya"
	architecture "x64"
	startproject "LiquidEditor"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Deya/vendor/GLFW/include"
IncludeDir["Glad"] = "Deya/vendor/glad/include"
IncludeDir["ImGui"] = "Deya/vendor/imgui"
IncludeDir["glm"] = "Deya/vendor/glm"
IncludeDir["stb_image"] = "Deya/vendor/stb_image"
IncludeDir["entt"] = "Deya/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Deya/vendor/yaml-cpp/include"

group "Dependencies"
	include "Deya/vendor/GLFW"
	include "Deya/vendor/glad"
	include "Deya/vendor/imgui"
	include "Deya/vendor/yaml-cpp"
group ""

project "Deya"
	location "Deya"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dypch.h"
	pchsource "Deya/src/dypch.cpp"

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/include",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
	}
	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DY_PLATFORM_WINDOWS",
			"DY_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

        links
        {
		    "opengl32.lib"  
        }

    filter "system:linux"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_LINUX",
            "GLFW_INCLUDE_NONE"
        }

        links
        {
            "GL",
            "pthread",
            "dl"
        }

		disablewarnings
		{
			"int-to-pointer-cast"
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
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp"
	}
	includedirs
	{
		"%{prj.name}/include",
		"Deya/vendor/spdlog/include",
		"Deya/include",
		"Deya/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
	}
	links
	{
		"Deya",
        "GLFW",
		"Glad",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DY_PLATFORM_WINDOWS"
		}

        links
        {
		    "opengl32.lib"  
        }
    
    filter "system:linux"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_LINUX"
        }

        links
        {
            "GL",
            "pthread",
            "dl"
        }

		postbuildcommands 
		{
			"{COPY} ../%{prj.name}/assets %{cfg.targetdir}"
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

project "LiquidEditor"
	location "LiquidEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/include",
		"Deya/vendor/spdlog/include",
		"Deya/include",
		"Deya/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
	}

	links
	{
		"Deya",
        "GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DY_PLATFORM_WINDOWS"
		}

        links
        {
		    "opengl32.lib"  
        }
    
    filter "system:linux"
        systemversion "latest"

        defines
        {
            "DY_PLATFORM_LINUX"
        }

        links
        {
            "GL",
            "pthread",
            "dl"
        }

		postbuildcommands 
		{
			"{COPY} ../%{prj.name}/assets %{cfg.targetdir}"
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