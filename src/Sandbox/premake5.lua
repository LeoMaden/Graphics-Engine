project "Sandbox"
    --kind "WindowedApp"
    kind "ConsoleApp"
    language "C++"

    files {
        "**.cpp",
        "**.h"
    }
    
    includedirs {
        "../Engine"
    }

    links {
        "Engine",
        "opengl32"
    }

    --defines {
    --}

    --pchheader "PulsePCH.h"
    --pchsource "Source/PulsePCH.cpp"

    targetdir ("../../Bin/" .. outputdir .. "Sandbox")
    objdir ("../../Build/" .. outputdir .. "Sandbox")