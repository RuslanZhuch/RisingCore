
project "RisingCore"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "bin/%{cfg.buildcfg}"

    files { "includes/**.h", "sources/**cpp" }

    includedirs {
        "includes"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        runtime "Debug"
        staticruntime "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        runtime "Release"
        staticruntime "On"
        