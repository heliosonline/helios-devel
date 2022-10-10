#pragma once

#include "HeliosEngine/Renderer/Texture.h"


namespace Helios {


	// ============================================================================
	// == class Font
	// ============================================================================


	struct FontData;


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
		Font(const std::string& name, const std::string& filepath, const uint32_t flags);
		~Font() = default;

		const Ref<Texture> GetTexture() { return m_Texture; }
		const std::string& GetName() { return m_Name; }
		uint32_t GetFlags() { return m_Flags; }

//		glm::vec4& GetTexCoords(const char32_t codepoint);
//		float GetAdvance(const char32_t codepoint);
//		float GetWhitespaceAdvance();
//		float GetKerning(const char32_t cp1, const char32_t cp2);

		static Ref<Font> Create(const std::string& name, const std::string& filepath, const uint32_t flags);

	private:
//		void GenerateTexture();
// 		void GenerateAtlas();

	private:
		std::string m_Name;
		uint32_t m_Flags;

		Ref<Texture> m_Texture;
		FontData* m_Data = nullptr;

//		friend class FontLibrary;
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
