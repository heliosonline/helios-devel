#include "pch.h"

#include "HeliosEngine/Core/Window.h"
#include "HeliosEngine/Events/ApplicationEvent.h"
#include "HeliosEngine/Events/MouseEvent.h"
#include "HeliosEngine/Events/KeyEvent.h"
#include "HeliosEngine/Renderer/Renderer.h"

#include <GLFW/glfw3.h>


namespace Helios {


	Scope<Window> Window::Create(const WindowSpecification& spec)
	{
		return CreateScope<Window>(spec);
	}
	static uint8_t s_GLFWWindowCount = 0;


	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_GLFW_ERROR("({0}): {1}", error, description);
	}


	Window::Window(const WindowSpecification& spec)
	{
		Init(spec);
	}


	Window::~Window()
	{
		Shutdown();
	}


	void Window::Init(const WindowSpecification& spec)
	{
		m_Data.Title = spec.Title;
		m_Data.Width = spec.Width;
		m_Data.Height = spec.Height;

		if (s_GLFWWindowCount == 0)
		{
			LOG_CORE_DEBUG("GLFW Version: {0}", glfwGetVersionString());

			int success = glfwInit();
			LOG_CORE_ASSERT(success, "Could not initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
		}

#if defined(BUILD_DEBUG) && defined(BUILDWITH_RENDERER_OPENGL)
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
		LOG_CORE_DEBUG("GLFW using debug mode context (hint)");
#endif

		{ // debug
			int count_mon;
			GLFWmonitor** monitors = glfwGetMonitors(&count_mon);
			if (!monitors)
				LOG_GLFW_ERROR("Can't retrieve montors!");
			for (int x = 0; x < count_mon; ++x)
			{
				LOG_CORE_DEBUG("Monitor found: ({0}):\"{1}\"", x, glfwGetMonitorName(monitors[x]));
				int count_modes;
				const GLFWvidmode* modes = glfwGetVideoModes(monitors[x], &count_modes);
				if (!modes)
					LOG_GLFW_ERROR("Can't retrieve video modes!");
				for (int y = 0; y < count_modes; ++y)
				{
					LOG_CORE_DEBUG("  Video mode({0}): {1}x{2} {3}bit {4}Hz",
						y, modes[y].width, modes[y].height, modes[y].redBits + modes[y].greenBits + modes[y].blueBits, modes[y].refreshRate);
				}
			}
		} // debug

		m_Window = glfwCreateWindow((int)spec.Width, (int)spec.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		SetVSync(false);

		// Set GLFW callbacks
		InitCallbacks();
	}


	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}


	void Window::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}


	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}


	bool Window::IsVSync() const
	{
		return m_Data.VSync;
	}


// Code is taken from ImGui_ImplGlfw_TranslateUntranslatedKey()
// see also: ImGui_ImplGlfw_KeyCallback()
// see also: https://github.com/TheCherno/Hazel/pull/262/commits/349eca3bba7a15c135e64da623e1a0e80aefa6bd
	// GLFW 3.1+ attempts to "untranslate" keys, which goes the opposite of what every other framework does, making using lettered shortcuts difficult.
	// (It had reasons to do so: namely GLFW is/was more likely to be used for WASD-type game controls rather than lettered shortcuts, but IHMO the 3.1 change could have been done differently)
	// See https://github.com/glfw/glfw/issues/1502 for details.
	// Adding a workaround to undo this (so our keys are translated->untranslated->translated, likely a lossy process).
	// This won't cover edge cases but this is at least going to cover common cases.
	static int Window_TranslateKeyCode(int key, int scancode)
	{
		if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_EQUAL)
			return key;
		const char* key_name = glfwGetKeyName(key, scancode);
		if (key_name && key_name[0] != 0 && key_name[1] == 0)
		{
			const char char_names[] = "`-=[]\\,;\'./";
			const int char_keys[] = { GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_MINUS, GLFW_KEY_EQUAL, GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_COMMA, GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE, GLFW_KEY_PERIOD, GLFW_KEY_SLASH, 0 };
//			LOG_CORE_ASSERT(sizeof() == sizeof());
//			IM_ASSERT(IM_ARRAYSIZE(char_names) == IM_ARRAYSIZE(char_keys));
			if (key_name[0] >= '0' && key_name[0] <= '9') { key = GLFW_KEY_0 + (key_name[0] - '0'); }
			else if (key_name[0] >= 'A' && key_name[0] <= 'Z') { key = GLFW_KEY_A + (key_name[0] - 'A'); }
			else if (key_name[0] >= 'a' && key_name[0] <= 'z') { key = GLFW_KEY_A + (key_name[0] - 'a'); }
			else if (const char* p = strchr(char_names, key_name[0])) { key = char_keys[p - char_names]; }
		}
		return key;
	}
// Code is taken from ImGui_ImplGlfw_TranslateUntranslatedKey()


	void Window::InitCallbacks()
	{
		//-----------------------------------
		// Set GLFW callbacks (Key input)
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
// TODO: KeyCode translation/localization
//LOG_CORE_DEBUG("KeyCallback(Key:{0}, scancode:{1}, keyname:{2})", key, scancode, glfwGetKeyName(key, scancode));
//int trans_key = Window_TranslateKeyCode(key, scancode);
//LOG_CORE_DEBUG("Translated(Key:{0})", trans_key);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.EventCallback(event);
					break;
				}
				}
			});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});


		//-----------------------------------
		// Set GLFW callbacks (Mouse input)
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
		//glfwSetCursorEnterCallback
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});


		//-----------------------------------
		// Set GLFW callbacks (Joystick)
		//glfwSetJoystickCallback


		//-----------------------------------
		// Set GLFW callbacks (Path drop input)
		//glfwSetDropCallback


		//-----------------------------------
		// Set GLFW callbacks (Window events)
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});
		//glfwSetWindowContentScaleCallback
		//glfwSetWindowPosCallback
		//glfwSetWindowIconifyCallback
		//glfwSetWindowMaximizeCallback
		//glfwSetWindowFocusCallback
		//glfwSetWindowRefreshCallback


		//-----------------------------------
		// Set GLFW callbacks (Framebuffer events)
		//glfwSetFramebufferSizeCallback
	}


} // namespace Helios
