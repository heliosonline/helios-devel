-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Engine.freetype"
	kind          "StaticLib"
	architecture  "x86_64"
	language      "C"
	cppdialect    "C++20"
	staticruntime "On"
	targetname    "freetype"
	
	targetdir (dir_bin   .. dir_group .. dir_config .. "engine.vendor/" .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. "engine.vendor/" .. dir_project)

	-- move project in the correct dir
	basedir("freetype")


	includedirs {
		"freetype/include"
	}


	defines {
		"WIN32",
		"_LIB",
		"_CRT_SECURE_NO_WARNINGS",
		"FT2_BUILD_LIBRARY",
	}


	files {
		"freetype/src/autofit/autofit.c",
		"freetype/src/bdf/bdf.c",
		--"freetype/src/bzip2/ftbzip2.c",
		"freetype/src/cache/ftcache.c",
		"freetype/src/cff/cff.c",
		"freetype/src/cid/type1cid.c",
		"freetype/src/dlg/dlgwrap.c",
		--"freetype/src/gxvalid/gxvalid.c",
		"freetype/src/gzip/ftgzip.c",
		"freetype/src/lzw/ftlzw.c",
		--"freetype/src/otvalid/otvalid.c",
		"freetype/src/pcf/pcf.c",
		"freetype/src/pfr/pfr.c",
		"freetype/src/psaux/psaux.c",
		"freetype/src/pshinter/pshinter.c",
		"freetype/src/psnames/psmodule.c",
		"freetype/src/raster/raster.c",
		"freetype/src/sdf/sdf.c",
		"freetype/src/sfnt/sfnt.c",
		"freetype/src/smooth/smooth.c",
		"freetype/src/svg/svg.c",
		"freetype/src/truetype/truetype.c",
		"freetype/src/type1/type1.c",
		"freetype/src/type42/type42.c",
		"freetype/src/winfonts/winfnt.c",      

		"freetype/src/base/ftbase.c",
		"freetype/src/base/ftbbox.c",
		"freetype/src/base/ftbdf.c",
		"freetype/src/base/ftbitmap.c",
		"freetype/src/base/ftcid.c",
		"freetype/src/base/ftfstype.c",
		"freetype/src/base/ftgasp.c",
		"freetype/src/base/ftglyph.c",
		"freetype/src/base/ftgxval.c",
		"freetype/src/base/ftinit.c",
		"freetype/src/base/ftmm.c",
		"freetype/src/base/ftotval.c",
		"freetype/src/base/ftpatent.c",
		"freetype/src/base/ftpfr.c",
		"freetype/src/base/ftstroke.c",
		"freetype/src/base/ftsynth.c",
		"freetype/src/base/fttype1.c",
		"freetype/src/base/ftwinfnt.c",

		-- windows only?
		"freetype/builds/windows/ftdebug.c",
		"freetype/builds/windows/ftsystem.c",

		--"freetype/src/base/ftsystem.c",
		--"freetype/src/base/ftgxval.c",
		--"freetype/src/base/ftlcdfil.c",
		--"freetype/src/base/ftxf86.c",
		--"freetype/src/base/ftfstype.c",
	}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

		defines {
			"FT_DEBUG_LEVEL_ERROR",
			"FT_DEBUG_LEVEL_TRACE",
		}
		
	filter "configurations:Release"
		runtime "Release"
		optimize "On"

		defines {
		}
