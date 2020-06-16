project "Engine"
    kind "StaticLib"
    language "C++"

    files {
        "**.cpp",
        "**.h"
    }
    
    includedirs {
        ".",
        "../../Dependencies/glad/include",
        "../../Dependencies/OpenGL",
        "../../Dependencies/glm",
        "../../Dependencies/spdlog",
        "../../Dependencies/stb_image",
        "../../Dependencies/freetype/include"
    }

    links {
        "glad",
        "freetype"
    }

    defines {
    }

    --pchheader "PulsePCH.h"
    --pchsource "Source/PulsePCH.cpp"

    targetdir ("../../Bin/" .. outputdir .. "Engine")
    objdir ("../../Build/" .. outputdir .. "Engine")