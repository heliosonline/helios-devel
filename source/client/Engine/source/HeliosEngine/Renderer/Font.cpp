#include "pch.h"

#include "HeliosEngine/Renderer/Font.h"

#include <msdf-atlas-gen/msdf-atlas-gen.h>


namespace Helios {


	// ============================================================================
	// == class Font
	// ============================================================================


	struct FontData
	{
		// Generated chars
		// Kerning
	};


	Ref<Font> Font::Create(const std::string& name, const std::string& filepath, const uint32_t flags)
	{
LOG_INFO("Creating font object for font: {0}", filepath);
		Ref<Font> fontobj = nullptr;

		std::vector<msdf_atlas::GlyphGeometry> glyphs;
		msdf_atlas::FontGeometry fontGeometry(&glyphs);

		// Load font
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		if (ft)
		{
			msdfgen::FontHandle* font = msdfgen::loadFont(ft, filepath.c_str());
			if (font)
			{
				// TODO: implement multiple (language dependent) charsets
				int glyphsLoaded = fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
				LOG_CORE_ASSERT(glyphsLoaded, "Could not load glyphs from font: {0}", filepath);
			}
			msdfgen::destroyFont(font);
			msdfgen::deinitializeFreetype(ft);
		}

		// Assign glyph edge colors
		// TODO...

		// Pack glyphs
		// TODO...

		// Generate the atlas
		// TODO...

		// Generate Texture
		// TODO...

		// Store font metrics
		// TODO...

		return fontobj;
	}


	Font::Font(const std::string& name, const std::string& filepath, const uint32_t flags)
		: m_Name(name), m_Flags(flags)
	{
LOG_INFO("Loading font: {0}", filepath);
	}


	// ============================================================================
	// == class FontLibrary
	// ============================================================================


	std::unordered_map<std::string, std::unordered_map<uint32_t, Ref<Font>>> FontLibrary::m_Fonts = {}; // [name][flags]


	void FontLibrary::Add(const Ref<Font>& font)
	{
		LOG_CORE_ASSERT(!Exists(font->GetName(), font->GetFlags()), "Font already exists!");
		m_Fonts[font->GetName()][font->GetFlags()] = font;
	}


	Ref<Font> FontLibrary::Load(const std::string& name, const uint32_t flags)
	{
		// find folder
		std::filesystem::path filepath = "Assets/Fonts/";
		filepath += name;
		if (!std::filesystem::exists(filepath))
		{
			LOG_CORE_ASSERT(true, "Font folder not found: {0}", filepath.string());
			return nullptr;
		}

		// try each file in folder
		std::unordered_map<std::string, uint32_t> files;
		for (auto file : std::filesystem::directory_iterator{ filepath })
		{
			// check for *.ttf
			if (!(file.path().extension() == ".ttf"))
				continue;
			// generate flags
			std::string file_name = file.path().filename().string();
			std::string file_path = filepath.string();
			file_path += "/" + file_name;
			size_t start = file_name.rfind('-');
			uint32_t file_flags = 0;
			if (start == std::string::npos)
			{
				// if filename has no flags in its name...
				file_flags = FontFlags::Regular;
			}
			else
			{
				start++;
				if (file_name.find("Regular", start) != std::string::npos)
					{ file_flags += FontFlags::Regular; }
				if (file_name.find("Italic", start) != std::string::npos)
					{ file_flags += FontFlags::Italic; }
				if (file_name.find("Light", start) != std::string::npos)
					{ file_flags += FontFlags::Light; }
				if (file_name.find("Medium", start) != std::string::npos)
					{ file_flags += FontFlags::Medium; }
				if (file_name.find("SemiBold", start) != std::string::npos)
					{ file_flags += FontFlags::SemiBold; }
				if (file_name.find("ExtraBold", start) != std::string::npos)
					{ file_flags += FontFlags::ExtraBold; }
				if ((file_name.find("Bold", start) != std::string::npos)
						&& !(file_flags & FontFlags::ExtraBold)
						&& !(file_flags & FontFlags::SemiBold))
					{ file_flags += FontFlags::Bold; }
			}
			// if found == requested
			if (flags == file_flags)
			{
				auto font = Font::Create(name, file_path, file_flags);
				if (font)
				{
					Add(font);
					return font;
				}
			}
		}
		// no matching file found
		LOG_CORE_ASSERT(true, "Font not found: {0}", name);
		return nullptr;
	}


	Ref<Font> FontLibrary::Get(const std::string& name, const uint32_t flags)
	{
		// if font not loaded...
		if (!Exists(name, flags))
		{
			// try to load font
			return Load(name, flags);
		}
		return m_Fonts[name][flags];
	}


	bool FontLibrary::Exists(const std::string& name, const uint32_t flags)
	{
		// find font with requested name
		if (m_Fonts.find(name) == m_Fonts.end())
			return false;
		// find entry with requested flags
		if (m_Fonts.find(name)->second.find(flags) == m_Fonts.find(name)->second.end())
			return false;
		return true;
	}


} // namespace Helios
