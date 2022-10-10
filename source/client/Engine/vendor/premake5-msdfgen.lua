-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Engine.msdfgen"
	kind          "StaticLib"
	architecture  "x86_64"
	language      "C"
	cppdialect    "C++20"
	staticruntime "On"
	targetname    "msdfgen"
	
	targetdir (dir_bin   .. dir_group .. dir_config .. "engine.vendor/" .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. "engine.vendor/" .. dir_project)

	-- move project in the correct dir
	basedir("msdfgen")
	
	
	links {
		"Engine.freetype",
	}


	includedirs {
		"freetype/include",
		"msdfgen/include",
	}


	defines {
		"MSDFGEN_USE_CPP11",
	}


	files {
		"msdfgen/core/contour-combiners.cpp",
		"msdfgen/core/Contour.cpp",
		"msdfgen/core/edge-coloring.cpp",
		"msdfgen/core/edge-segments.cpp",
		"msdfgen/core/edge-selectors.cpp",
		"msdfgen/core/EdgeHolder.cpp",
		"msdfgen/core/equation-solver.cpp",
		"msdfgen/core/msdf-error-correction.cpp",
		"msdfgen/core/MSDFErrorCorrection.cpp",
		"msdfgen/core/Projection.cpp",
		"msdfgen/core/sdf-error-estimation.cpp",
		"msdfgen/core/rasterization.cpp",
		"msdfgen/core/render-sdf.cpp",
		"msdfgen/core/save-bmp.cpp",
		"msdfgen/core/save-tiff.cpp",
		"msdfgen/core/Scanline.cpp",
		"msdfgen/core/shape-description.cpp",
		"msdfgen/core/Shape.cpp",
		"msdfgen/core/SignedDistance.cpp",
		"msdfgen/core/Vector2.cpp",
		"msdfgen/ext/import-font.cpp",
		"msdfgen/ext/import-svg.cpp",
		"msdfgen/ext/resolve-shape-geometry.cpp",
		"msdfgen/ext/save-png.cpp",
		"msdfgen/lib/lodepng.cpp",
		"msdfgen/lib/tinyxml2.cpp",
		"msdfgen/core/msdfgen.cpp",
	}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
