project "glad"
    kind "StaticLib"
    language "C"

    files {
        "**.c",
        "**.h"
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

    targetdir ("../../Bin/" .. outputdir .. "glad")
    objdir ("../../Build/" .. outputdir .. "glad")