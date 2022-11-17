#pragma once

#include "HeliosEngine/Renderer/Texture.h"


namespace Helios {


	// ============================================================================
	// == class Font
	// ============================================================================


	struct FontData;


	struct GlyphMetrics
	{
		// Horizontal advance
		float Advance;
		// Boundingbox in atlas
		struct { float l, r, t, b; } AtlasCoords;
		// Boundingbox rel to baseline
		struct { float l, r, t, b; } BaselineCoords;
		// Texture size
		struct { int w, h; } TextureSize;
	};


	struct FontMetrics
	{
		float Ascender;
		float Descender;
		float LineHeight;
		float UnderlineOffset;
		float UnderlineThickness;
	};


	enum FontFlags : uint32_t
	{
		// base font
		Regular   = 0,

		// strength
		Light     = (1 << 0),
		Medium    = (1 << 1),
		SemiBold  = (1 << 2),
		Bold      = (1 << 3),
		ExtraBold = (1 << 4),

		// style
		Italic    = (1 << 5)
	};


	class Font
	{
	public:
		static Ref<Font> CreateFromAsset(const std::string& family, const std::string filename);
		static Ref<Font> CreateFromCache(const std::string& family, const uint32_t style);

		void GenerateCache();

		Font(const Ref<FontData>& pfd);
		~Font() = default;

		const std::string& GetFamilyName() { return m_FontFamily; }
		uint32_t GetStyleFlags() { return m_FontStyle; }

		FontMetrics& GetFontMetrics() { return m_FontMetrics; }
		GlyphMetrics& GetGlyphMetrics(const char32_t codepoint, bool add = false);
		Ref<Texture2D> GetAtlasTexture() { return m_AtlasTexture; }

		float GetAdvance(const char32_t cp_left, const char32_t cp_right = 0);
		float GetKerning(const char32_t cp_left, const char32_t cp_right);

		bool AddGlyph(const char32_t codepoint);
//		bool AddCharset(type??? charset);

	private:
		std::string m_FontFamily;
		std::string m_FontFile;
		uint32_t m_FontStyle;

		FontMetrics m_FontMetrics;
		std::map<char32_t, GlyphMetrics> m_Glyphs;  // codepage -> GlyphMetrics
		std::map<std::pair<char32_t, char32_t>, float> m_Kernings;  // [std::pair(cp_left, cp_right)] = offset_x

		Ref<Texture2D> m_AtlasTexture;
		uint32_t m_AtlasWidth, m_AtlasHeight;
		int m_AtlasBPP;
//		void* m_AtlasData;
	};


	// ============================================================================
	// == class FontLibrary
	// ============================================================================


	class FontLibrary
	{
	public:
		FontLibrary() = delete;

		static void Init();

		static void Add(const Ref<Font>& font);
		static bool Exists(const std::string& name, const uint32_t flags);
		static Ref<Font> Get(const std::string& name, const uint32_t flags = FontFlags::Regular);
//		static Ref<Font> GetNearest(const std::string& name, const uint32_t flags = FontFlags::Regular);
		static Ref<Font> Load(const std::string& name, const uint32_t flags);
	private:
		static std::unordered_map<std::string, std::unordered_map<uint32_t, Ref<Font>>> m_Fonts; // [name][flags]
	};


} // namespace Helios
