project "assimp"
    kind "StaticLib"
    language "C++"

    files {
        "code/**.cpp",
        "code/**.h",
        "*.h",
        "contrib/clipper/*",
        "contrib/irrXML/*",
        "contrib/Open3DGC/*",
        "contrib/openddlparser/code/*.cpp",
        "contrib/poly2tri/**",
        "contrib/unzip/*",
        "contrib/zip/src/*"
    }

    excludes {
        "code/Importer/IFC/IFCReaderGen_4.cpp",
        "code/Importer/IFC/IFCReaderGen_4.h"
	}

    buildoptions{"/bigobj"}
    
    includedirs {
        "include",
        "code",
        "contrib",
        "contrib/unzip",
        "contrib/irrXML",
        ".",
        "contrib/openddlparser/include",
        "contrib/rapidjson/include",
        "contrib/irrXML",
        "contrib/zlib"
    }

    links {
        "zlib"
    }

    defines {
        "UNICODE",
        "_UNICODE",
        "ASSIMP_BUILD_NO_C4D_IMPORTER",
        "MINIZ_USE_UNALIGNED_LOADS_AND_STORES=0",
        "ASSIMP_IMPORTER_GLTF_USE_OPEN3DGC=1",
        "ASSIMP_BUILD_DLL_EXPORT",
        "_SCL_SECURE_NO_WARNINGS",
        "_CRT_SECURE_NO_WARNINGS",
        "OPENDDLPARSER_BUILD",
        "CMAKE_INTDIR='Debug'"
    }

    targetdir ("../../Bin/" .. outputdir .. "assimp")
    objdir ("../../Build/" .. outputdir .. "assimp")