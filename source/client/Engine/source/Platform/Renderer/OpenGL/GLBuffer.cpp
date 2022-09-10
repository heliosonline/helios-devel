#include "pch.h"

#include "Platform/Renderer/OpenGL/GLBuffer.h"

#include "glad/gl.h"


namespace Helios {


	//////////////////////////////////////////////////////////////////////////////
	// VertexBuffer //////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////


	GLVertexBuffer::GLVertexBuffer(uint32_t size)
	{
		HE_PROFILER_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}


	GLVertexBuffer::GLVertexBuffer(float* vertices, uint32_t size)
	{
		HE_PROFILER_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		HE_PROFILER_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}


	void GLVertexBuffer::Bind() const
	{
		HE_PROFILER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}


	void GLVertexBuffer::Unbind() const
	{
		HE_PROFILER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void GLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		HE_PROFILER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}


	//////////////////////////////////////////////////////////////////////////////
	// IndexBuffer ///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////


	GLIndexBuffer::GLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		HE_PROFILER_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		HE_PROFILER_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}


	void GLIndexBuffer::Bind() const
	{
		HE_PROFILER_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}


	void GLIndexBuffer::Unbind() const
	{
		HE_PROFILER_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


} // namespace Helios
