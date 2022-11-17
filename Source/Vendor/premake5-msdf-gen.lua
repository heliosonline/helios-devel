-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Vendor.msdf-gen"
	kind          "StaticLib"
	architecture  "x86_64"
	language      "C"
	cppdialect    "C++20"
	staticruntime "On"
	targetname    "msdf-gen"
	
	targetdir (dir_bin   .. dir_group .. dir_config .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)

	-- move project in the correct dir
	basedir("msdf-gen")
	
	
	links {
		"Vendor.freetype",
	}


	includedirs {
		"freetype/include",
		"msdf-gen/include",
	}


	defines {
		"MSDFGEN_USE_CPP11",
	}


	files {
		"msdf-gen/core/contour-combiners.cpp",
		"msdf-gen/core/Contour.cpp",
		"msdf-gen/core/edge-coloring.cpp",
		"msdf-gen/core/edge-segments.cpp",
		"msdf-gen/core/edge-selectors.cpp",
		"msdf-gen/core/EdgeHolder.cpp",
		"msdf-gen/core/equation-solver.cpp",
		"msdf-gen/core/msdf-error-correction.cpp",
		"msdf-gen/core/MSDFErrorCorrection.cpp",
		"msdf-gen/core/Projection.cpp",
		"msdf-gen/core/sdf-error-estimation.cpp",
		"msdf-gen/core/rasterization.cpp",
		"msdf-gen/core/render-sdf.cpp",
		"msdf-gen/core/save-bmp.cpp",
		"msdf-gen/core/save-tiff.cpp",
		"msdf-gen/core/Scanline.cpp",
		"msdf-gen/core/shape-description.cpp",
		"msdf-gen/core/Shape.cpp",
		"msdf-gen/core/SignedDistance.cpp",
		"msdf-gen/core/Vector2.cpp",
		"msdf-gen/ext/import-font.cpp",
		"msdf-gen/ext/import-svg.cpp",
		"msdf-gen/ext/resolve-shape-geometry.cpp",
		"msdf-gen/ext/save-png.cpp",
		"msdf-gen/lib/lodepng.cpp",
		"msdf-gen/lib/tinyxml2.cpp",
		"msdf-gen/core/msdfgen.cpp",
	}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
