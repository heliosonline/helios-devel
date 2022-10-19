#pragma once

#include "HeliosEngine/Renderer/RendererAPI.h"


namespace Helios {


	class GLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;
		void ClearColorBuffer() override;
		void ClearDepthBuffer() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		void SetLineWidth(float width) override;
	};


} // namespace Helios
