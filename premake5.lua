workspace "Engine"
    configurations { "Debug", "Release"}
    platforms { "Win32", "Win64" }

    startproject "Sandbox" 

    staticruntime "On"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:Win32"
        system "Windows"
        architecture "x86"

    filter "platforms:Win64"
        system "Windows"
        architecture "x86_64"


outputdir = "%{cfg.platform}-%{cfg.buildcfg}/"

include "src/Sandbox/premake5.lua"
include "src/Engine/premake5.lua"
