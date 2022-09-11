#pragma once

#include "HeliosEngine/Renderer/Texture.h"


namespace Helios {


	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const uint16_t indexX, const uint16_t indexY, const uint16_t tilesX, const uint16_t tilesY);

		const Ref<Texture2D> GetTexture() { return m_Texture; }
		const glm::vec2* GetTexCoords() { return m_TexCoords; }

		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const uint16_t indexX, const uint16_t indexY, const uint16_t tilesX, const uint16_t tilesY);
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};


} // namespace Helios
