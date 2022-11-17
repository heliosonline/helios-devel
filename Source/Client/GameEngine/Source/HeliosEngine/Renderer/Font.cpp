#include "pch.h"

#include "HeliosEngine/Renderer/Font.h"
#include "HeliosEngine/Core/Assets.h"

#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include <json.hpp>
using json = nlohmann::json;

namespace Helios {


	// ============================================================================
	// == class Font
	// ============================================================================


#define DEFAULT_ANGLE_THRESHOLD 3.0
#define DEFAULT_PIXEL_RANGE 8.0
#define DEFAULT_MITER_LIMIT 2.0
#define DEFAULT_MIN_EMSIZE 32.0


	struct FontData
	{
		// Fonts family name
		std::string family;
		// Fonts file name
		std::string file;
		// Fonts style flags
		uint32_t style;
		// Font metrics
		FontMetrics metrics;
		// Glyph metrics
		std::map<char32_t, GlyphMetrics> cp2gm;
		// Atlas texture
		Ref<Texture2D> atlasTexture;
		uint32_t atlasWidth, atlasHeight;
		int atlasBPP;
		// Kernings
		std::map<std::pair<char32_t, char32_t>, float> kerning;
	};


	Ref<Font> Font::CreateFromAsset(const std::string& font_family, const std::string font_filename)
	{
		// Path to the fonts asset folder...
		std::filesystem::path asset_folder = Assets::GetAssetPath(AssetType::FontAsset);
		asset_folder += "/" + font_family;

		// Path to the fonts variant asset file...
		std::filesystem::path asset_file = asset_folder;
		asset_file += "/" + font_filename;

		// Filename
		std::string file_name = asset_file.filename().stem().string();

		// Struct to hold all creation data
		auto pfd = CreateRef<FontData>();
		pfd->family = font_family;
		pfd->file = font_filename;

		// Get fonts style flags (from fonts filename)
		{
			pfd->style = 0;
			size_t start = file_name.rfind('-');
			if (start == std::string::npos)
			{
				pfd->style = FontFlags::Regular;
			}
			else
			{
				start++;
				if (file_name.find("Regular", start) != std::string::npos)
					pfd->style += FontFlags::Regular;
				if (file_name.find("Italic", start) != std::string::npos)
					pfd->style += FontFlags::Italic;
				if (file_name.find("Light", start) != std::string::npos)
					pfd->style += FontFlags::Light;
				if (file_name.find("Medium", start) != std::string::npos)
					pfd->style += FontFlags::Medium;
				if (file_name.find("SemiBold", start) != std::string::npos)
					pfd->style += FontFlags::SemiBold;
				if (file_name.find("ExtraBold", start) != std::string::npos)
					pfd->style += FontFlags::ExtraBold;
				if ((file_name.find("Bold", start) != std::string::npos)
					&& !(pfd->style & FontFlags::ExtraBold)
					&& !(pfd->style & FontFlags::SemiBold))
					pfd->style += FontFlags::Bold;
			}
		} // Get fonts flags

		// Load font asset
		std::vector<msdf_atlas::GlyphGeometry> msdf_glyphs;
		msdf_atlas::FontGeometry msdf_fontGeometry(&msdf_glyphs);
		{
			msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
			LOG_CORE_ASSERT(ft, "FontLibrary: Can't initialize freetype!");

			std::string filepath = asset_file.string();
			msdfgen::FontHandle* font = msdfgen::loadFont(ft, filepath.c_str());
			if (font)
			{
				// TODO: implement multiple (language dependent) charsets

				// Load glyphs
				int glyphsLoaded = msdf_fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
				if (glyphsLoaded < 1)
				{
					LOG_CORE_ERROR("FontLibrary: Could not load any glyphs from font \"{0}\"!",
						file_name);
				}
				else if (glyphsLoaded < msdf_atlas::Charset::ASCII.size())
				{
					LOG_CORE_WARN("FontLibrary: Font \"{0}\" is missing {1} codepoints!",
						file_name, msdf_atlas::Charset::ASCII.size() - glyphsLoaded);
				}
				// Load kerning data
				auto x = msdf_fontGeometry.loadKerning(font);
			}
			msdfgen::destroyFont(font);
			msdfgen::deinitializeFreetype(ft);

			if (msdf_glyphs.empty())
				return nullptr;
		} // Load font asset

		// Generate atlas
		msdfgen::BitmapConstRef<unsigned char, 3> msdf_bitmap;
		{
			// Assign glyph edge colors
			for (msdf_atlas::GlyphGeometry& glyph : msdf_glyphs)
				glyph.edgeColoring(msdfgen::edgeColoringByDistance, DEFAULT_ANGLE_THRESHOLD, 1);

			// Pack glyphs
			msdf_atlas::TightAtlasPacker atlasPacker;
			atlasPacker.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::POWER_OF_TWO_RECTANGLE);
			atlasPacker.setMinimumScale(DEFAULT_MIN_EMSIZE);
			atlasPacker.setPixelRange(DEFAULT_PIXEL_RANGE);
			atlasPacker.setMiterLimit(DEFAULT_MITER_LIMIT);
			int glyphPackFailureCount = atlasPacker.pack(msdf_glyphs.data(), (int)msdf_glyphs.size());
			if (glyphPackFailureCount > 0)
			{
				LOG_CORE_WARN("Font: Failure to pack {1} glyphs from font \"{0}\" into atlas!",
					file_name, glyphPackFailureCount);
			}

			// Generate the atlas
			int width = -1, height = -1;
			atlasPacker.getDimensions(width, height);
			msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<unsigned char, 3>>
				generator(width, height);
			generator.setThreadCount(std::max((int)std::thread::hardware_concurrency(), 1));
			generator.generate(msdf_glyphs.data(), (int)msdf_glyphs.size());
			msdf_bitmap = (msdfgen::BitmapConstRef<unsigned char, 3>)generator.atlasStorage();

			// Generate Texture
			pfd->atlasTexture = Texture2D::Create(width, height, 3);
			pfd->atlasTexture->SetData((void*)msdf_bitmap.pixels, width * height * 3);
			pfd->atlasWidth = width;
			pfd->atlasHeight = height;
			pfd->atlasBPP = 3;
		} // Generate atlas

		// Get glyph metrics
		std::map<int, char32_t> idx2cp;
		{
			for (const msdf_atlas::GlyphGeometry& glyph : msdf_glyphs)
			{
				GlyphMetrics gm = {};

				// Unicode codepoint
				char32_t codepoint = glyph.getCodepoint();
				idx2cp[glyph.getIndex()] = codepoint;

				// Horizontal advance
				gm.Advance = (float)glyph.getAdvance();

				// Glyph bounding box in atlas
				double l, b, r, t;
				glyph.getQuadAtlasBounds(l, b, r, t);
				gm.AtlasCoords.l = (float)(l /= msdf_bitmap.width);
				gm.AtlasCoords.r = (float)(r /= msdf_bitmap.width);
				gm.AtlasCoords.b = (float)(b /= msdf_bitmap.height);
				gm.AtlasCoords.t = (float)(t /= msdf_bitmap.height);

				// Glyph bounding box rel to baseline
				glyph.getQuadPlaneBounds(l, b, r, t);
				gm.BaselineCoords.l = (float)l;
				gm.BaselineCoords.r = (float)r;
				gm.BaselineCoords.b = (float)b;
				gm.BaselineCoords.t = (float)t;

				// Glyph box size in atlas
				glyph.getBoxSize(gm.TextureSize.w, gm.TextureSize.h);

				pfd->cp2gm[codepoint] = gm;
			}
		}
		// Get glyph metrics

		// Get font metrics
		{
			msdfgen::FontMetrics metrics = msdf_fontGeometry.getMetrics();
			pfd->metrics.Ascender = (float)metrics.ascenderY;
			pfd->metrics.Descender = (float)metrics.descenderY;
			pfd->metrics.LineHeight = (float)metrics.lineHeight;
			pfd->metrics.UnderlineOffset = (float)metrics.underlineY;
			pfd->metrics.UnderlineThickness = (float)metrics.underlineThickness;
		} // Get font metrics

		// Get kernings
		{
			for (const auto [key, val] : msdf_fontGeometry.getKerning())
			{
				std::pair<char32_t, char32_t> codePointsKey(
					idx2cp[key.first],
					idx2cp[key.second]);
				pfd->kerning[codePointsKey] = (float)val;
			}
		} // Get kernings

		// Create font object
		Ref<Font> font = CreateRef<Font>(pfd);
		return font;
	}


	Ref<Font> Font::CreateFromCache(const std::string& family, const uint32_t style)
	{
		// Generate file names
		std::filesystem::path cache_folder = Assets::GetCachePath(AssetType::FontAsset);
		cache_folder += "/" + family;
		cache_folder += "/" + family + "-";

		if (style == FontFlags::Regular)   cache_folder += "Regular";
		if (style &  FontFlags::Light)     cache_folder += "Light";
		if (style &  FontFlags::Medium)    cache_folder += "Medium";
		if (style &  FontFlags::SemiBold)  cache_folder += "SemiBold";
		if (style &  FontFlags::Bold)      cache_folder += "Bold";
		if (style &  FontFlags::ExtraBold) cache_folder += "ExtraBold";
		if (style &  FontFlags::Italic)    cache_folder += "Italic";
		cache_folder += "/";

		std::filesystem::path cache_atlas = cache_folder;
		cache_atlas += "atlas.cache.png";
		std::filesystem::path cache_data = cache_folder;
		cache_data += "data.cache.json";

		// Struct to hold all creation data
		auto pfd = CreateRef<FontData>();

		// Read cached data
		std::ifstream i(cache_data);
		json j;
		i >> j;

		pfd->family = j["FontFamily"];
		pfd->file = j["FontFile"];
		pfd->style = j["FontStyle"];
		pfd->metrics.Ascender = j["FontMetrics"]["Ascender"];
		pfd->metrics.Descender = j["FontMetrics"]["Descender"];
		pfd->metrics.LineHeight = j["FontMetrics"]["LineHeight"];
		pfd->metrics.UnderlineOffset = j["FontMetrics"]["UnderlineOffset"];
		pfd->metrics.UnderlineThickness = j["FontMetrics"]["UnderlineThickness"];
		pfd->atlasWidth = j["Atlas"][0];
		pfd->atlasHeight = j["Atlas"][1];
		pfd->atlasBPP = j["Atlas"][2];
		for (auto& [cp, g] : j["GlyphMetrics"].items())
		{
			GlyphMetrics gm;
			gm.Advance = g["Advance"];
			gm.AtlasCoords.l = g["AtlasCoords"][0];
			gm.AtlasCoords.r = g["AtlasCoords"][1];
			gm.AtlasCoords.t = g["AtlasCoords"][2];
			gm.AtlasCoords.b = g["AtlasCoords"][3];
			gm.BaselineCoords.l = g["BaselineCoords"][0];
			gm.BaselineCoords.r = g["BaselineCoords"][1];
			gm.BaselineCoords.t = g["BaselineCoords"][2];
			gm.BaselineCoords.b = g["BaselineCoords"][3];
			gm.TextureSize.w = g["AtlasCoords"][0];
			gm.TextureSize.h = g["AtlasCoords"][1];
			pfd->cp2gm[(uint32_t)stoul(cp)] = gm;
		}

		/*
		* TODO....
		std::map<std::pair<char32_t, char32_t>, float> kerning;
		*/

		// Read cached atlas
		pfd->atlasTexture = Texture2D::Create(cache_atlas.string());
		pfd->atlasTexture->SetFiltering(HE_TEXTURE_FILTER_LINEAR, HE_TEXTURE_FILTER_LINEAR);

		// Create font object
		Ref<Font> font = CreateRef<Font>(pfd);
		return font;
	}


	void Font::GenerateCache()
	{
		// Path to the fonts cache folder...
		std::filesystem::path cache_folder = Assets::GetCachePath(AssetType::FontAsset);
		cache_folder += "/" + m_FontFamily;
		cache_folder += "/" + m_FontFamily + "-";

		if (m_FontStyle == FontFlags::Regular)   cache_folder += "Regular";
		if (m_FontStyle &  FontFlags::Light)     cache_folder += "Light";
		if (m_FontStyle &  FontFlags::Medium)    cache_folder += "Medium";
		if (m_FontStyle &  FontFlags::SemiBold)  cache_folder += "SemiBold";
		if (m_FontStyle &  FontFlags::Bold)      cache_folder += "Bold";
		if (m_FontStyle &  FontFlags::ExtraBold) cache_folder += "ExtraBold";
		if (m_FontStyle &  FontFlags::Italic)    cache_folder += "Italic";
		cache_folder += "/";

		if (!std::filesystem::exists(cache_folder))
			std::filesystem::create_directories(cache_folder);

		// Base filename
		std::string filename = cache_folder.string();
		std::string filepath;

		// Save atlas
		stbi_uc* data = new(std::nothrow) stbi_uc[m_AtlasWidth * m_AtlasHeight * m_AtlasBPP];
		m_AtlasTexture->GetData(data, m_AtlasWidth * m_AtlasHeight * m_AtlasBPP);
		filepath = filename + "atlas.cache.png";
		stbi_flip_vertically_on_write(true);
		stbi_write_png(filepath.c_str(), m_AtlasWidth, m_AtlasHeight, 3, data, m_AtlasWidth * m_AtlasBPP);
		delete(data);

		// Save font data
		filepath = filename + "data.cache.json";

		json j;
		j["FontFamily"] = m_FontFamily;
		j["FontFile"] = m_FontFile;
		j["FontStyle"] = m_FontStyle;
		j["FontMetrics"]["Ascender"] = m_FontMetrics.Ascender;
		j["FontMetrics"]["Descender"] = m_FontMetrics.Descender;
		j["FontMetrics"]["LineHeight"] = m_FontMetrics.LineHeight;
		j["FontMetrics"]["UnderlineOffset"] = m_FontMetrics.UnderlineOffset;
		j["FontMetrics"]["UnderlineThickness"] = m_FontMetrics.UnderlineThickness;
		for (auto g : m_Glyphs)
		{
			std::string cp = std::to_string(g.first);
			j["GlyphMetrics"][cp]["Advance"] = g.second.Advance;
			j["GlyphMetrics"][cp]["AtlasCoords"] =
				{ g.second.AtlasCoords.l, g.second.AtlasCoords.r, g.second.AtlasCoords.t, g.second.AtlasCoords.b };
			j["GlyphMetrics"][cp]["BaselineCoords"] =
				{ g.second.BaselineCoords.l, g.second.BaselineCoords.r, g.second.BaselineCoords.t, g.second.BaselineCoords.b };
			j["GlyphMetrics"][cp]["TextureSize"] =
				{ g.second.TextureSize.w, g.second.TextureSize.h };
		}
		for (auto k : m_Kernings)
		{
			// TODO...
			int x = 1;
			x += 1;
		}
		j["Atlas"] = { m_AtlasWidth, m_AtlasHeight, m_AtlasBPP };

		std::ofstream ofs(filepath);
		if (HE_CACHE_NICE_JSON)
			ofs << std::setw(4) << j << std::endl;
		else
			ofs << j << std::endl;
	}


	Font::Font(const Ref<FontData>& pfd) :
		m_FontFamily(pfd->family),
		m_FontFile(pfd->file),
		m_FontStyle(pfd->style),
		m_FontMetrics(pfd->metrics),
		m_Glyphs(pfd->cp2gm),
		m_Kernings(pfd->kerning),
		m_AtlasTexture(pfd->atlasTexture),
		m_AtlasWidth(pfd->atlasWidth),
		m_AtlasHeight(pfd->atlasHeight),
		m_AtlasBPP(pfd->atlasBPP)
	{
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
		AddGlyph(codepoint);
		return m_Glyphs[codepoint];
	}


	float Font::GetAdvance(const char32_t cp_left, const char32_t cp_right)
	{
		if (m_Glyphs.count(cp_left) && m_Glyphs.count(cp_right))
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
		// TODO... Adding glyphs is not supported yet!

		return false;
	}



	// ============================================================================
	// == class FontLibrary
	// ============================================================================


	std::unordered_map<std::string, std::unordered_map<uint32_t, Ref<Font>>> FontLibrary::m_Fonts = {}; // [name][flags]


	void FontLibrary::Init()
	{
		// Check cache folder
		std::filesystem::path path_cache = Assets::GetCachePath(AssetType::FontAsset);
		if (!std::filesystem::exists(path_cache))
		{
			std::filesystem::create_directories(path_cache);
			LOG_CORE_DEBUG("FontLibrary: Creating font cache folder \"{0}\"",
				path_cache.string());
		}

		// For each available font...
		//     ASSET_FOLDER/FONT_NAME
		std::filesystem::path path_asset = Assets::GetAssetPath(AssetType::FontAsset);
		for (auto path_font : std::filesystem::directory_iterator{ path_asset })
		{
			// For each available font variant...
			//     ASSET_FOLDER/FONT_NAME/FONT_VARIANT
			for (auto path_variant : std::filesystem::directory_iterator{ path_font })
			{
				// Only *.ttf
				if (!(path_variant.path().extension() == ".ttf"))
					continue;

				// Check cache files
				std::filesystem::path cache_variant = path_cache;
				cache_variant += "/" + path_font.path().filename().string();
				cache_variant += "/" + path_font.path().filename().string() + "-";

				uint32_t style = 0;
				size_t start = path_variant.path().filename().string().rfind('-');
				if (start == std::string::npos)
				{
					style = FontFlags::Regular;
				}
				else
				{
					start++;
					if (path_variant.path().filename().string().find("Regular", start) != std::string::npos)
						style += FontFlags::Regular;
					if (path_variant.path().filename().string().find("Italic", start) != std::string::npos)
						style += FontFlags::Italic;
					if (path_variant.path().filename().string().find("Light", start) != std::string::npos)
						style += FontFlags::Light;
					if (path_variant.path().filename().string().find("Medium", start) != std::string::npos)
						style += FontFlags::Medium;
					if (path_variant.path().filename().string().find("SemiBold", start) != std::string::npos)
						style += FontFlags::SemiBold;
					if (path_variant.path().filename().string().find("ExtraBold", start) != std::string::npos)
						style += FontFlags::ExtraBold;
					if ((path_variant.path().filename().string().find("Bold", start) != std::string::npos)
						&& !(style & FontFlags::ExtraBold)
						&& !(style & FontFlags::SemiBold))
						style += FontFlags::Bold;
				}

				if (style == FontFlags::Regular)   cache_variant += "Regular";
				if (style &  FontFlags::Light)     cache_variant += "Light";
				if (style &  FontFlags::Medium)    cache_variant += "Medium";
				if (style &  FontFlags::SemiBold)  cache_variant += "SemiBold";
				if (style &  FontFlags::Bold)      cache_variant += "Bold";
				if (style &  FontFlags::ExtraBold) cache_variant += "ExtraBold";
				if (style &  FontFlags::Italic)    cache_variant += "Italic";
				cache_variant += "/";

				std::filesystem::path cache_data = cache_variant;
				cache_data += "data.cache.json";
				std::filesystem::path cache_atlas = cache_variant;
				cache_atlas += "atlas.cache.png";

				bool generate = true;
				if (std::filesystem::exists(cache_atlas) && std::filesystem::exists(cache_data))
				{
					std::filesystem::file_time_type mtime_asset = std::filesystem::last_write_time(path_variant);
					std::filesystem::file_time_type mtime_atlas = std::filesystem::last_write_time(cache_atlas);
					std::filesystem::file_time_type mtime_data  = std::filesystem::last_write_time(cache_data);

					if ((mtime_asset < mtime_atlas) && (mtime_asset < mtime_data))
						generate = false;
				}

				// Cache is out-of-date
				if (generate)
				{
					LOG_CORE_WARN("Generating cache for \"{0}\"", path_variant.path().filename().string());

					// Generate font data
					auto font = Font::CreateFromAsset(path_font.path().filename().string(),
						path_variant.path().filename().string());
					if (!font)
						continue;

					// Generate cache files
					font->GenerateCache();
				}
			} // path_font
		} // path_asset
	}


	void FontLibrary::Add(const Ref<Font>& font)
	{
		if (Exists(font->GetFamilyName(), font->GetStyleFlags()))
		{
			LOG_CORE_ERROR("FontLibrary: Font \"{0}\" is already loaded!",
				font->GetFamilyName());
			return;
		}
		m_Fonts[font->GetFamilyName()][font->GetStyleFlags()] = font;
	}


	bool FontLibrary::Exists(const std::string& name, const uint32_t flags)
	{
		// Find font with requested name
		if (m_Fonts.find(name) == m_Fonts.end())
			return false;
		// Find entry with requested style flags
		if (m_Fonts.find(name)->second.find(flags) == m_Fonts.find(name)->second.end())
			return false;
		return true;
	}


	Ref<Font> FontLibrary::Get(const std::string& name, const uint32_t flags)
	{
		// If font not loaded...
		if (!Exists(name, flags))
		{
			// Load font
			Ref<Font> font = Load(name, flags);
			// Add font to library
			Add(font);
		}
		return m_Fonts[name][flags];
	}


	Ref<Font> FontLibrary::Load(const std::string& name, const uint32_t flags)
	{
		return Font::CreateFromCache(name, flags);
	}



} // namespace Helios
