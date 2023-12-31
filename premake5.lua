-- premake5.lua
workspace "RisingCore"
    architecture "x64"
    configurations { "Debug", "Release" }

include "projgen.lua"

project "Tests"
    location "tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "bin/%{cfg.buildcfg}"
    dependson { "RisingCore" }


    files { "tests/includes/**.h", "tests/sources/**cpp" }

    includedirs {
        "tests/includes",
        "thirdparty",
        "includes",
        "sources"
    }

    libdirs {
        "tests/libs/gtests/Debug"
    }

    links {
        "gtest_main",
        "gtest",
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        runtime "Debug"
        staticruntime "On"

        flags { "MultiProcessorCompile" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        runtime "Release"
        staticruntime "On"

        flags { "MultiProcessorCompile" }

    filter { "action:vs2022" }
        buildoptions { "/MP" }

