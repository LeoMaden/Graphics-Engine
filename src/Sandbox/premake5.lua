project "Sandbox"
    --kind "WindowedApp"
    kind "ConsoleApp"
    language "C++"

    files {
        "**.cpp",
        "**.h"
    }
    
    includedirs {
        "../Engine",
        "../../Dependencies/spdlog",
        "../../Dependencies/glad/include",
        "../../Dependencies/glm",
        "../../Dependencies/assimp/include"
    }

    links {
        "Engine",
        "opengl32",
        "assimp"
    }

    --defines {
    --}

    --pchheader "PulsePCH.h"
    --pchsource "Source/PulsePCH.cpp"

    targetdir ("../../Bin/" .. outputdir .. "Sandbox")
    objdir ("../../Build/" .. outputdir .. "Sandbox")