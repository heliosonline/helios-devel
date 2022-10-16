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
#define DEFAULT_MIN_EMSIZE 64.0


	struct FontData
	{
		std::vector<msdf_atlas::GlyphGeometry> glyphs;
		msdf_atlas::FontGeometry fontGeometry;
		std::map<int, char32_t> index2codepoint;
	};


	Ref<Font> Font::Create(const std::string& name, const std::string& filepath, const uint32_t flags)
	{
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

				// Load glyphs
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
				// Load kerning data
				auto x = fontGeometry.loadKerning(font);
			}
			msdfgen::destroyFont(font);
			msdfgen::deinitializeFreetype(ft);
		}
		if (glyphs.empty())
			return nullptr;

		// Create FontData
		auto pfd = CreateRef<FontData>();
		pfd->glyphs = glyphs;
		pfd->fontGeometry = fontGeometry;

		// Create Font object
		return CreateRef<Font>(name, filepath, flags, pfd);
	}


	Font::Font(const std::string& name, const std::string& filepath, const uint32_t flags, Ref<FontData>& pfd)
		: m_Name(name), m_Filepath(filepath), m_Flags(flags), m_Data(pfd)
	{
		// Assign glyph edge colors
		for (msdf_atlas::GlyphGeometry& glyph : pfd->glyphs)
			glyph.edgeColoring(msdfgen::edgeColoringByDistance, DEFAULT_ANGLE_THRESHOLD, 1);

		// Pack glyphs
		msdf_atlas::TightAtlasPacker atlasPacker;
		atlasPacker.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::POWER_OF_TWO_RECTANGLE);
		atlasPacker.setMinimumScale(DEFAULT_MIN_EMSIZE);
		atlasPacker.setPixelRange(DEFAULT_PIXEL_RANGE);
		atlasPacker.setMiterLimit(DEFAULT_MITER_LIMIT);
		int glyphPackFailureCount = atlasPacker.pack(pfd->glyphs.data(), (int)pfd->glyphs.size());
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
		generator.generate(pfd->glyphs.data(), (int)pfd->glyphs.size());
		auto bitmap = (msdfgen::BitmapConstRef<unsigned char, 3>)generator.atlasStorage();

		// Generate Texture
		m_AtlasTexture = Texture2D::Create(width, height, 3);
		m_AtlasTexture->SetData((void*)bitmap.pixels, width * height * 3);

		// Store font metrics
		for (const msdf_atlas::GlyphGeometry& glyph : pfd->glyphs)
		{
			GlyphMetrics gm = {};

			// Unicode codepoint
			char32_t codepoint = glyph.getCodepoint();
			pfd->index2codepoint[glyph.getIndex()] = codepoint;

			// Horizontal advance
			gm.Advance = (float)glyph.getAdvance();

			// Glyph bounding box in atlas
			double l, b, r, t;
			glyph.getQuadAtlasBounds(l, b, r, t);
			gm.AtlasCoords.l = (float)(l /= bitmap.width);
			gm.AtlasCoords.r = (float)(r /= bitmap.width);
			gm.AtlasCoords.b = (float)(b /= bitmap.height);
			gm.AtlasCoords.t = (float)(t /= bitmap.height);

			// Glyph bounding box rel to baseline
			glyph.getQuadPlaneBounds(l, b, r, t);
			gm.BaselineCoords.l = (float)l;
			gm.BaselineCoords.r = (float)r;
			gm.BaselineCoords.b = (float)b;
			gm.BaselineCoords.t = (float)t;

			m_Glyphs[codepoint] = gm;
		}

		// Font metricts
		msdfgen::FontMetrics metrics = pfd->fontGeometry.getMetrics();
		m_Metrics.Ascender = (float)metrics.ascenderY;
		m_Metrics.Descender = (float)metrics.descenderY;
		m_Metrics.LineHeight = (float)metrics.lineHeight;
		m_Metrics.UnderlineOffset = (float)metrics.underlineY;
		m_Metrics.UnderlineThickness = (float)metrics.underlineThickness;

		// Kerning
		for (const auto [key, val] : pfd->fontGeometry.getKerning())
		{
			std::pair<char32_t, char32_t> codePointsKey(
				pfd->index2codepoint[key.first],
				pfd->index2codepoint[key.second]);
			m_Kernings[codePointsKey] = (float)val;
		}
	}


	GlyphMetrics& Font::GetGlyphMetrics(const char32_t codepoint, bool add)
	{
		// Glyph is loaded
		if (m_Glyphs.count(codepoint))
			return m_Glyphs[codepoint];

		// Don't dynamic load glyph
		if (!add)
			return  m_Glyphs[0x20];

		// Dynamic load glyph
		if (AddGlyph(codepoint))
			return m_Glyphs[codepoint];
	}


	float Font::GetAdvance(const char32_t cp_left, const char32_t cp_right)
	{
		if (m_Glyphs.count(cp_right))
		{
			// advance with kerning
			return GetKerning(cp_left, cp_right) + m_Glyphs[cp_right].Advance;
		}
		else if (m_Glyphs.count(cp_right))
		{
			// advance only (no kerning)
			return m_Glyphs[cp_right].Advance;
		}
		else if (m_Glyphs.count(cp_left) && cp_right == 0)
		{
			// single codepoint (the left one)
			return m_Glyphs[cp_left].Advance;
		}
		return 0.0f;
	}


	float Font::GetKerning(const char32_t cp_left, const char32_t cp_right)
	{
		if (m_Kernings.count(std::pair(cp_left, cp_right)))
		{
			return m_Kernings[std::pair(cp_left, cp_right)];
		}
		return 0.0f;
	}


	bool Font::AddGlyph(const char32_t codepoint)
	{
		// TODO...

		return false;
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
