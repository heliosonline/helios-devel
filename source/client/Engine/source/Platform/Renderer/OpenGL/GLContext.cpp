#include "pch.h"

#include "Platform/Renderer/OpenGL/GLContext.h"

#include "GLFW/glfw3.h"
#include "glad/gl.h"


namespace Helios {


	GLContext::GLContext(GLFWwindow* window)
		: m_Window(window)
	{
		LOG_CORE_ASSERT(window, "Window handle is null!")
	}


	void GLContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
		int version = gladLoadGL(glfwGetProcAddress);
		LOG_CORE_ASSERT(version, "Failed to initialize OpenGL context!");

//		LOG_CORE_INFO("OpenGL {0}.{1} Info:", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
		LOG_CORE_INFO("OpenGL Info:");
		LOG_CORE_INFO("  Vendor:   {0}", glGetString(GL_VENDOR));
		LOG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		LOG_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));
	}


	void GLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}


} // namespace Helios
