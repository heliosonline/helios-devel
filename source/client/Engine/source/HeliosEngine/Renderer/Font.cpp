#include "pch.h"

#include "HeliosEngine/Renderer/Font.h"

#include <msdf-atlas-gen/msdf-atlas-gen.h>


namespace Helios {


	// ============================================================================
	// == class Font
	// ============================================================================


#define DEFAULT_ANGLE_THRESHOLD 3.0
#define DEFAULT_PIXEL_RANGE 2.0
#define DEFAULT_MITER_LIMIT 1.0
#define DEFAULT_MIN_EMSIZE 16.0


	struct FontData
	{
		// Generated chars
		// Kerning
	};


	Ref<Font> Font::Create(const std::string& name, const std::string& filepath, const uint32_t flags)
	{
LOG_DEBUG("Creating font object for font: {0}", filepath);
		Ref<Font> fontobj = nullptr;

		// Load font
		std::vector<msdf_atlas::GlyphGeometry> glyphs;
		msdf_atlas::FontGeometry fontGeometry(&glyphs);
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		if (ft)
		{
			msdfgen::FontHandle* font = msdfgen::loadFont(ft, filepath.c_str());
			if (font)
			{
				// TODO: implement multiple (language dependent) charsets

				int glyphsLoaded = fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
				if (glyphsLoaded < 1)
				{
					LOG_CORE_ERROR("Font: Could not load any glyphs from font \"{0}\"!",
						filepath);
				}
				else if (glyphsLoaded < msdf_atlas::Charset::ASCII.size())
				{
					LOG_CORE_WARN("Font: Font \"{0}\" is missing {1} codepoints!",
						filepath, msdf_atlas::Charset::ASCII.size() - glyphsLoaded);
				}
			}
			msdfgen::destroyFont(font);
			msdfgen::deinitializeFreetype(ft);
		}
		if (glyphs.empty())
			return nullptr;

		// Assign glyph edge colors
		for (msdf_atlas::GlyphGeometry& glyph : glyphs)
			glyph.edgeColoring(msdfgen::edgeColoringByDistance, DEFAULT_ANGLE_THRESHOLD, 1);

		// Pack glyphs
		msdf_atlas::TightAtlasPacker atlasPacker;
		atlasPacker.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::POWER_OF_TWO_RECTANGLE);
		atlasPacker.setMinimumScale(DEFAULT_MIN_EMSIZE);
		atlasPacker.setPixelRange(DEFAULT_PIXEL_RANGE);
		atlasPacker.setMiterLimit(DEFAULT_MITER_LIMIT);
		int glyphPackFailureCount = atlasPacker.pack(glyphs.data(), (int)glyphs.size());
		if (glyphPackFailureCount > 0)
		{
			LOG_CORE_WARN("Font: Failure to pack {1} glyphs from font \"{0}\" into atlas!",
				filepath, glyphPackFailureCount);
		}

		// Generate the atlas
		int width = -1, height = -1;
		atlasPacker.getDimensions(width, height);
		msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<unsigned char, 3>>
			generator(width, height);
		generator.setThreadCount(std::max((int)std::thread::hardware_concurrency(), 1));
		generator.generate(glyphs.data(), (int)glyphs.size());
		auto bitmap = (msdfgen::BitmapConstRef<unsigned char, 3>)generator.atlasStorage();
LOG_DEBUG("atlas size: {0}x{1}", width, height);

		// Create Font object
		fontobj = CreateRef<Font>(name, filepath, flags);

		// Generate Texture
		fontobj->m_Texture = Texture2D::Create(width, height, 3);
		fontobj->m_Texture->SetData((void*)bitmap.pixels, width * height * 3);

		// Store font metrics
		// TODO...

		return fontobj;
	}


	Font::Font(const std::string& name, const std::string& filepath, const uint32_t flags)
		: m_Name(name), m_Filepath(filepath), m_Flags(flags)
	{
LOG_DEBUG("Font: Constructor() {0}", name);
	}


	Font::~Font()
	{
		if (m_Data)
			delete m_Data;
	}


	// ============================================================================
	// == class FontLibrary
	// ============================================================================


	std::unordered_map<std::string, std::unordered_map<uint32_t, Ref<Font>>> FontLibrary::m_Fonts = {}; // [name][flags]


	void FontLibrary::Add(const Ref<Font>& font)
	{
		if (Exists(font->GetName(), font->GetFlags()))
		{
			LOG_CORE_ERROR("FontLibrary: Font \"{0}\" is already loaded!",
				font->GetName());
			return;
		}
		m_Fonts[font->GetName()][font->GetFlags()] = font;
	}


	Ref<Font> FontLibrary::Load(const std::string& name, const uint32_t flags)
	{
		// find folder
		std::filesystem::path filepath = "Assets/Fonts/";
		filepath += name;
		if (!std::filesystem::exists(filepath))
		{
			LOG_CORE_ERROR("FontLibrary: Font folder \"{0}\" not found!",
				filepath.string());
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
		// no matching font file found
		LOG_CORE_WARN("FontLibrary: Font \"{0}\" not found!",
			name);
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
