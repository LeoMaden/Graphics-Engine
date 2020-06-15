project "freetype"
    kind "StaticLib"
    language "C"

    files {
        "src/base/**.c",
        "src/lzw/**.c",
        "src/lzw/**.h",
        "include/**.h"
    }
    
    includedirs {
        "../../Dependencies/freetype/include",
        "../../Dependencies/freetype"
    }

    links {
    }

    defines {
        "FT2_BUILD_LIBRARY"
    }

    --pchheader "PulsePCH.h"
    --pchsource "Source/PulsePCH.cpp"

    targetdir ("../../Bin/" .. outputdir .. "freetype")
    objdir ("../../Build/" .. outputdir .. "freetype")