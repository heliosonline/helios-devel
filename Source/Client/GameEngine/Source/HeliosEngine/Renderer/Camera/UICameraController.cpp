#include "pch.h"

#include "HeliosEngine/Renderer/Camera/UICameraController.h"


namespace Helios {

	UICameraController::UICameraController()
		: m_Camera(0.0f, 0.0f, 0.0f, 0.0f)
	{
		HE_PROFILER_FUNCTION();
	}


	void UICameraController::OnUpdate(Timestep ts)
	{
		HE_PROFILER_FUNCTION();

		m_Camera.SetPosition(m_CameraPosition);
	}


	void UICameraController::OnEvent(Event& e)
	{
		HE_PROFILER_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(HE_BIND_EVENT_FN(UICameraController::OnWindowResized));
	}


	void UICameraController::OnResize(float width, float height)
	{
		HE_PROFILER_FUNCTION();

		m_AspectRatio = width / height;
		CalculateView();

		m_PixelSize = { m_ZoomLevel / width / 2.0f, m_ZoomLevel / height / 2.0f };
		m_PixelCount = { width, height };
	}


	void UICameraController::CalculateView()
	{
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}


	bool UICameraController::OnWindowResized(WindowResizeEvent& e)
	{
		HE_PROFILER_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}


} // namespace Helios
