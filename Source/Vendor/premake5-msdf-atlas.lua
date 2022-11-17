-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Vendor.msdf-atlas"
	kind          "StaticLib"
	architecture  "x86_64"
	language      "C"
	cppdialect    "C++20"
	staticruntime "On"
	targetname    "msdf-atlas"
	
	targetdir (dir_bin   .. dir_group .. dir_config .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)

	-- move project in the correct dir
	basedir("msdf-atlas")
	
	
	links {
		"Vendor.msdf-gen",
	}


	includedirs {
		"freetype/include",
		"msdf-gen",
		"msdf-gen/include",
		"msdf-atlas/msdf-atlas-gen",
	}


	defines {
		"MSDFGEN_USE_CPP11",
		"_CRT_SECURE_NO_WARNINGS",
	}


	files {
		--"msdf-atlas/msdf-atlas-gen/artery-font-export.cpp",
		"msdf-atlas/msdf-atlas-gen/bitmap-blit.cpp",
		"msdf-atlas/msdf-atlas-gen/charset-parser.cpp",
		"msdf-atlas/msdf-atlas-gen/Charset.cpp",
		"msdf-atlas/msdf-atlas-gen/csv-export.cpp",
		"msdf-atlas/msdf-atlas-gen/FontGeometry.cpp",
		"msdf-atlas/msdf-atlas-gen/glyph-generators.cpp",
		"msdf-atlas/msdf-atlas-gen/GlyphGeometry.cpp",
		"msdf-atlas/msdf-atlas-gen/image-encode.cpp",
		"msdf-atlas/msdf-atlas-gen/json-export.cpp",
		"msdf-atlas/msdf-atlas-gen/main.cpp",
		"msdf-atlas/msdf-atlas-gen/RectanglePacker.cpp",
		"msdf-atlas/msdf-atlas-gen/shadron-preview-generator.cpp",
		"msdf-atlas/msdf-atlas-gen/size-selectors.cpp",
		"msdf-atlas/msdf-atlas-gen/TightAtlasPacker.cpp",
		"msdf-atlas/msdf-atlas-gen/utf8.cpp",
		"msdf-atlas/msdf-atlas-gen/Workload.cpp",
	}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
