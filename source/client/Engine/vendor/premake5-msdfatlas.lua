-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Engine.msdfatlas"
	kind          "StaticLib"
	architecture  "x86_64"
	language      "C"
	cppdialect    "C++20"
	staticruntime "On"
	targetname    "msdfatlas"
	
	targetdir (dir_bin   .. dir_group .. dir_config .. "engine.vendor/" .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. "engine.vendor/" .. dir_project)

	-- move project in the correct dir
	basedir("msdfatlas")
	
	
	links {
		"Engine.msdfgen",
	}


	includedirs {
		"freetype/include",
		"msdfgen",
		"msdfgen/include",
		"msdfatlas/msdf-atlas-gen",
	}


	defines {
		"MSDFGEN_USE_CPP11",
		"_CRT_SECURE_NO_WARNINGS",
	}


	files {
		--"msdfatlas/msdf-atlas-gen/artery-font-export.cpp",
		"msdfatlas/msdf-atlas-gen/bitmap-blit.cpp",
		"msdfatlas/msdf-atlas-gen/charset-parser.cpp",
		"msdfatlas/msdf-atlas-gen/Charset.cpp",
		"msdfatlas/msdf-atlas-gen/csv-export.cpp",
		"msdfatlas/msdf-atlas-gen/FontGeometry.cpp",
		"msdfatlas/msdf-atlas-gen/glyph-generators.cpp",
		"msdfatlas/msdf-atlas-gen/GlyphGeometry.cpp",
		"msdfatlas/msdf-atlas-gen/image-encode.cpp",
		"msdfatlas/msdf-atlas-gen/json-export.cpp",
		"msdfatlas/msdf-atlas-gen/main.cpp",
		"msdfatlas/msdf-atlas-gen/RectanglePacker.cpp",
		"msdfatlas/msdf-atlas-gen/shadron-preview-generator.cpp",
		"msdfatlas/msdf-atlas-gen/size-selectors.cpp",
		"msdfatlas/msdf-atlas-gen/TightAtlasPacker.cpp",
		"msdfatlas/msdf-atlas-gen/utf8.cpp",
		"msdfatlas/msdf-atlas-gen/Workload.cpp",
	}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
