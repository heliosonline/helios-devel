#pragma once


namespace Helios {


	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};


	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual FramebufferSpecification& GetSpecification() = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};


} // namespace Helios