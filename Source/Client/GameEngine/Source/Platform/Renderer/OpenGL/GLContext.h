#pragma once

#include "HeliosEngine/Renderer/GraphicsContext.h"

#include "GLFW/glfw3.h"


namespace Helios {


	class GLContext : public GraphicsContext
	{
	public:
		GLContext(GLFWwindow* window);

		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_Window;
	};


} // namespace Helios
