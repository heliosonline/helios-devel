#include "pch_engine.h"

#include "HeliosEngine/Core/Application.h"
#include "HeliosEngine/Core/EntryPoint.h"
#include "HeliosEngine/Core/Timer.h"
#include "HeliosEngine/Core/Timestep.h"
#include "HeliosEngine/Utils/Path.h"
#include "HeliosEngine/Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <imgui.h>


namespace Helios {


	int AppMain(int argc, char** argv)
	{
		auto app = CreateApplication({ argc, argv });
		app->Run();
		delete app;

		return 0;
	}


	Application* Application::s_Instance = nullptr;


	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		// set working directory
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);
		if (m_Specification.hints & Hints::HINT_USE_CWD)
			m_Specification.WorkingDirectory = Utils::GetCurrentDir();
		if (m_Specification.hints & Hints::HINT_USE_EXEPATH_AS_CWD)
			m_Specification.WorkingDirectory = Utils::GetExePath();

		Log::Init("HeliosEngine.log", m_Specification.WorkingDirectory);

		LOG_CORE_DEBUG("Working path: {0}", m_Specification.WorkingDirectory);

		LOG_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowSpecification(m_Specification.Name));
		m_Window->SetEventCallback(HE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		static std::string inipath = m_Specification.WorkingDirectory;
		inipath += DIR_SEP  "imgui.ini";
		m_ImGuiLayer = new ImGuiLayer(inipath);
		PushOverlay(m_ImGuiLayer);
	}


	Application::~Application()
	{
		Renderer::Shutdown();
	}


	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}


	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}


	void Application::Close()
	{
		m_Running = false;
	}


	void Application::OnEvent(Event& e)
	{
//		LOG_CORE_TRACE("{0}", e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HE_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		Timer RunLoopTimer;
		while (m_Running)
		{
			Timestep timestep = RunLoopTimer.Elapsed();
			RunLoopTimer.Reset();

			{ // tempoary for debuging
				static int fps = 0;
				fps += (int)(1.0f / timestep);
				static int fps_cnt = 0;
				fps_cnt++;
				static float fps_ts = 0;
				fps_ts += timestep;
				if (fps_ts >= 1.0f)
				{
					std::ostringstream title;
					title << "FPS: " << fps/fps_cnt << " (" << 1000*fps_ts/fps_cnt << " ms)";
					glfwSetWindowTitle((GLFWwindow*)m_Window->GetNativeWindow(), title.str().c_str());
					fps = 0;
					fps_cnt = 0;
					fps_ts = 0;
				}
			} // tempoary for debuging

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);

				m_ImGuiLayer->Begin();
static bool show = true;
ImGui::ShowDemoWindow(&show);
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}


	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}


} // namespace Helios
