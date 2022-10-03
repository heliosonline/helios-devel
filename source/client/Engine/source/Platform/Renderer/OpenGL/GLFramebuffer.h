#pragma once

#include "HeliosEngine/Renderer/Framebuffer.h"


namespace Helios {


	class GLFramebuffer : public Framebuffer
	{
	public:
		GLFramebuffer(const FramebufferSpecification& spec);
		virtual ~GLFramebuffer();

		void Invalidate();

		void Bind() override;
		void Unbind() override;

		uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		FramebufferSpecification& GetSpecification() override { return m_Specification; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};


} // namespace Helios
