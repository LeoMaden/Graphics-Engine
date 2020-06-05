project "Engine"
    kind "StaticLib"
    language "C++"

    files {
        "**.cpp",
        "**.h",
        "../../Dependencies/glad/**.c",
        "../../Dependencies/glad/**.h",
    }
    
    includedirs {
        "../../Dependencies/glad/include",
        "../../Dependencies/OpenGL",
    }

    links {
    }

    defines {
    }

    --pchheader "PulsePCH.h"
    --pchsource "Source/PulsePCH.cpp"

    targetdir ("../../Bin/" .. outputdir .. "Engine")
    objdir ("../../Build/" .. outputdir .. "Engine")