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
        "../../Dependencies/assimp/include"
    }

    links {
        "glad",
        "assimp"
    }

    defines {
    }

    pchheader "Pch.h"
    pchsource "./Pch.cpp"

    targetdir ("../../Bin/" .. outputdir .. "Engine")
    objdir ("../../Build/" .. outputdir .. "Engine")