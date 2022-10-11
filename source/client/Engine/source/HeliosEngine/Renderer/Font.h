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
		Font(const std::string& name, const std::string& filepath, const uint32_t flags, Ref<FontData>& pfd);
		~Font();

		const std::string& GetName() { return m_Name; }
		const std::string& GetFilePath() { return m_Filepath; }
		uint32_t GetFlags() { return m_Flags; }

		Ref<Texture2D> GetAtlasTexture() { return m_AtlasTexture; }
		FontMetrics& GetFontMetrics() { return m_Metrics; }
//		GlyphMetrics& GetGlyphInfo(const char32_t codepoint);
//		float GetKerning(const uint32_t cp_left, const uint32_t cp_right);

		static Ref<Font> Create(const std::string& name, const std::string& filepath, const uint32_t flags);

	private:
//		bool AddGlyph(const uint32_t codepoint);
//		bool AddCharset(type??? charset);
// 		void GenerateAtlas();
//		void GenerateTexture();

	private:
		std::string m_Name;
		std::string m_Filepath;
		uint32_t m_Flags;

		Ref<Texture2D> m_AtlasTexture;
		FontMetrics m_Metrics;
		std::map<uint32_t, GlyphMetrics> m_Glyphs;
		std::map<std::pair<uint32_t, uint32_t>, float> m_Kernings; // [std::pair(cp_left, cp_right)] = offset_x

		Ref<FontData> m_Data;
	};


	// ============================================================================
	// == class FontLibrary
	// ============================================================================


	class FontLibrary
	{
	public:
		FontLibrary() = delete;

		static void Add(const Ref<Font>& font);
		static Ref<Font> Load(const std::string& name, const uint32_t flags = FontFlags::Regular);

		static Ref<Font> Get(const std::string& name, const uint32_t flags = FontFlags::Regular);
		static Ref<Font> GetNearest(const std::string& name, const uint32_t flags = FontFlags::Regular);

		static bool Exists(const std::string& name, const uint32_t flags);
	private:
		static std::unordered_map<std::string, std::unordered_map<uint32_t, Ref<Font>>> m_Fonts; // [name][flags]
	};


} // namespace Helios
