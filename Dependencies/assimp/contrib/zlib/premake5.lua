project "zlib"
    kind "StaticLib"
    language "C"

    files {
        "*.c",
        "*.h"
    }
    
    includedirs {
        "."
    }

    links {
    }

    defines {
    }

    targetdir ("../../Bin/" .. outputdir .. "zlib")
    objdir ("../../Build/" .. outputdir .. "zlib")