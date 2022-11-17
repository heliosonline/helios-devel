#pragma once


namespace Helios {


	enum AssetType
	{
		FontAsset,
		ShaderAsset,
		TextureAsset
	};


	class Assets
	{
	public:
		Assets() = delete;

		static void Init();

		static std::string GetAssetPath(AssetType type);
		static std::string GetCachePath(AssetType type);
	};


} // namespace Helios
