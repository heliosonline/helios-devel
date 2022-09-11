#include "pch.h"

#include "Platform/Renderer/OpenGL/GLContext.h"

#include "GLFW/glfw3.h"
#include "glad/gl.h"


namespace Helios {


	GLContext::GLContext(GLFWwindow* window)
		: m_Window(window)
	{
		HE_PROFILER_FUNCTION();

		LOG_CORE_ASSERT(window, "Window handle is null!")
	}


	void GLContext::Init()
	{
		HE_PROFILER_FUNCTION();

		glfwMakeContextCurrent(m_Window);
		int version = gladLoadGL(glfwGetProcAddress);
		LOG_CORE_ASSERT(version, "Failed to initialize OpenGL context!");

		// Log basic OpenGL info
//		LOG_CORE_INFO("OpenGL {0}.{1} Info:", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
		LOG_CORE_INFO("OpenGL Info:");
		LOG_CORE_INFO("  Vendor:   {0}", glGetString(GL_VENDOR));
		LOG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		LOG_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));
		LOG_CORE_INFO("  Shader:   {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

		// Log basic OpenGL capabilities
		LOG_CORE_DEBUG("OpenGL Driver Capabilities:");
		int texSlots;
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &texSlots);
		LOG_CORE_DEBUG("  Texture Slots: {0}", texSlots);
		float lineWidth[2];
		glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);
		LOG_CORE_DEBUG("  Line Widths:   {0} .. {1}", lineWidth[0], lineWidth[1]);
		int texSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
		LOG_CORE_DEBUG("  Texture Size:  {0} x {0}", texSize);

		// Check for required OpenGL version 4.5
		LOG_CORE_ASSERT(GLAD_VERSION_MAJOR(version) > 4 || (GLAD_VERSION_MAJOR(version) == 4 && GLAD_VERSION_MINOR(version) >= 5), "OpenGL version >= 4.5 is required!")
	}


	void GLContext::SwapBuffers()
	{
		HE_PROFILER_FUNCTION();

		glfwSwapBuffers(m_Window);
	}


} // namespace Helios
