#include "pch.h"

#include "Platform/Renderer/OpenGL/GLRendererAPI.h"

#include "glad/gl.h"


namespace Helios {


	void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         LOG_CORE_FATAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          LOG_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_CORE_TRACE(message); return;
			default: LOG_CORE_ASSERT(false, "Unknown severity level!");
		}
	}


	void GLRendererAPI::Init()
	{
		HE_PROFILER_FUNCTION();

#ifdef BUILD_DEBUG
		// install a message callback for debuging
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		LOG_CORE_DEBUG("OpenGL Renderer: DebugMessageCallback installed");
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}


	void GLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		HE_PROFILER_FUNCTION();

		glViewport(x, y, width, height);
	}


	void GLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		HE_PROFILER_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}


	void GLRendererAPI::Clear()
	{
		HE_PROFILER_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void GLRendererAPI::ClearColorBuffer()
	{
		HE_PROFILER_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT);
	}


	void GLRendererAPI::ClearDepthBuffer()
	{
		HE_PROFILER_FUNCTION();

		glClear(GL_DEPTH_BUFFER_BIT);
	}


	void GLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		HE_PROFILER_FUNCTION();

		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}


	void GLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}


	void GLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}


} // namespace Helios
