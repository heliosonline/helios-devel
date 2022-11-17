#pragma once

#include "HeliosEngine/Renderer/Texture.h"


namespace Helios {


	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> GetTexture() { return m_Texture; }
		const glm::vec2* GetTexCoords() { return m_TexCoords; }

		static Ref<SubTexture2D> CreateByIndex(const Ref<Texture2D>& texture, const uint16_t indexX, const uint16_t indexY, const uint16_t tilesX, const uint16_t tilesY);
		static Ref<SubTexture2D> CreateByCoord(const Ref<Texture2D>& texture, const glm::vec2& pos, const glm::vec2& size);
		static Ref<SubTexture2D> CreateByCoord(const Ref<Texture2D>& texture, const glm::vec4& coords);
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};


} // namespace Helios
