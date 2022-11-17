#include "pch.h"

#include "HeliosEngine/Core/Assets.h"
#include "HeliosEngine/Renderer/Font.h"


namespace Helios {


	void Assets::Init()
	{
		// Generate font cache
		FontLibrary::Init();
	}


	std::string Assets::GetAssetPath(AssetType type)
	{
		switch (type)
		{
		case AssetType::FontAsset:
			return std::string("Assets/Fonts");
		case AssetType::ShaderAsset:
			return std::string("Assets/Shaders");
		case AssetType::TextureAsset:
			return std::string("Assets/Textures");
		default:
			return std::string("");
		}
	}


	std::string Assets::GetCachePath(AssetType type)
	{
		switch (type)
		{
		case AssetType::FontAsset:
			return std::string("AssetsCache/Fonts");
		default:
			return GetAssetPath(type);
		}
	}


} // namespace Helios

