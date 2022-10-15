#include "pch.h"

#include "Platform/Renderer/OpenGL/GLTexture.h"

#include <stb_image.h>


namespace Helios {


	GLTexture2D::GLTexture2D(uint32_t width, uint32_t height, int bpp)
		: m_Width(width), m_Height(height)
	{
		HE_PROFILER_FUNCTION();

		m_InternalFormat = bpp == 4 ? GL_RGBA8 : GL_RGB8;
		m_DataFormat = bpp == 4 ? GL_RGBA : GL_RGB;
		Create();
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}


	GLTexture2D::GLTexture2D(const std::string& path)
		: m_Path(path)
	{
		HE_PROFILER_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			m_IsLoaded = true;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			LOG_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			m_Width = width;
			m_Height = height;
			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;
			Create();
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
	}


	GLTexture2D::~GLTexture2D()
	{
		HE_PROFILER_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}


	void GLTexture2D::SetData(void* data, uint32_t size)
	{
		HE_PROFILER_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		LOG_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}


	void GLTexture2D::Bind(uint32_t slot) const
	{
		HE_PROFILER_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}


	void GLTexture2D::Create()
	{
		HE_PROFILER_FUNCTION();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

//		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}


} // namespace Helios
