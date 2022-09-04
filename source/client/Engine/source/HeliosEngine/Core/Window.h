#pragma once

#include "HeliosEngine/Events/Event.h"

#include "HeliosEngine/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>


namespace Helios {


	struct WindowSpecification
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowSpecification(const std::string& title = "HeliosEngine - Default Title",
			uint32_t width = 800,
			uint32_t height = 600)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		static Scope<Window> Create(const WindowSpecification& spec = WindowSpecification());

		Window(const WindowSpecification& spec);
		~Window();

		void OnUpdate();

		unsigned int GetWidth() { return m_Data.Width; }
		unsigned int GetHeight() { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() const;

		void* GetNativeWindow() const { return m_Window; }

	private:
		void Init(const WindowSpecification& spec);
		void InitCallbacks();
		void Shutdown();

	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	};


} // namespace Helios
