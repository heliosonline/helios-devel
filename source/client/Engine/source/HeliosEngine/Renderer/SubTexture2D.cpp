#include "pch.h"

#include "HeliosEngine/Renderer/SubTexture2D.h"


namespace Helios {


	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		// texture coords (counter clock wise)
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}


	Ref<SubTexture2D> SubTexture2D::CreateByIndex(const Ref<Texture2D>& texture, const uint16_t indexX, const uint16_t indexY, const uint16_t tilesX, const uint16_t tilesY)
	{
		// calculate coords
		const float sizeX = 1.0f / tilesX;
		const float sizeY = 1.0f / tilesY;
		const glm::vec2 min = {  indexX      * sizeX,  indexY      * sizeY }; // bottom-left
		const glm::vec2 max = { (indexX + 1) * sizeX, (indexY + 1) * sizeY }; // top-right

		return CreateRef<SubTexture2D>(texture, min, max);
	}

	Ref<SubTexture2D> SubTexture2D::CreateByCoord(const Ref<Texture2D>& texture, const glm::vec2& pos, const glm::vec2& size)
	{
		const glm::vec2 max = { pos.x + size.x, pos.y + size.y };

		return CreateRef<SubTexture2D>(texture, pos, max);
	}


} // namespace Helios
