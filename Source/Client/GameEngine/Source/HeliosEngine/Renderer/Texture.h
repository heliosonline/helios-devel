#pragma once


namespace Helios {

#define HE_TEXTURE_FILTER_NEAREST 1
#define HE_TEXTURE_FILTER_LINEAR 2

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual const std::string& GetPath() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void GetData(void* data, uint32_t size) = 0;

		virtual void SetFiltering(uint32_t min, uint32_t mag) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};


	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, int bpp = 4);
		static Ref<Texture2D> Create(const std::string& path);
	};


} // namespace Helios