#include "pch.h"

#include "HeliosEngine/Renderer/SubTexture2D.h"


namespace Helios {


	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const uint16_t indexX, const uint16_t indexY, const uint16_t tilesX, const uint16_t tilesY)
		: m_Texture(texture)
	{
		const float sizeX = 1.0f / tilesX;
		const float sizeY = 1.0f / tilesY;

		m_TexCoords[0] = {  indexX      * sizeX,  indexY      * sizeY };
		m_TexCoords[1] = { (indexX + 1) * sizeX,  indexY      * sizeY };
		m_TexCoords[2] = { (indexX + 1) * sizeX, (indexY + 1) * sizeY };
		m_TexCoords[3] = {  indexX      * sizeX, (indexY + 1) * sizeY };
	}


	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const uint16_t indexX, const uint16_t indexY, const uint16_t tilesX, const uint16_t tilesY)
	{
		return CreateRef<SubTexture2D>(texture, indexX, indexY, tilesX, tilesY);
	}


} // namespace Helios
