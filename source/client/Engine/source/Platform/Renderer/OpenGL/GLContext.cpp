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

//		LOG_CORE_INFO("OpenGL {0}.{1} Info:", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
		LOG_CORE_INFO("OpenGL Info:");
		LOG_CORE_INFO("  Vendor:   {0}", glGetString(GL_VENDOR));
		LOG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		LOG_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));

		LOG_CORE_INFO("OpenGL Driver Capabilities:");
		int texSlots;
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &texSlots);
		LOG_CORE_INFO("  Texture Slots: {0}", texSlots);
		float lineWidth[2];
		glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);
		LOG_CORE_INFO("  Line Widths: {0} .. {1}", lineWidth[0], lineWidth[1]);

#ifdef HE_LOG_ASSERTS
		// Check for required OpenGL version 4.5
		LOG_CORE_ASSERT(GLAD_VERSION_MAJOR(version) > 4 || (GLAD_VERSION_MAJOR(version) == 4 && GLAD_VERSION_MINOR(version) >= 5), "OpenGL version >= 4.5 is required!")
#endif
	}


	void GLContext::SwapBuffers()
	{
		HE_PROFILER_FUNCTION();

		glfwSwapBuffers(m_Window);
	}


} // namespace Helios
