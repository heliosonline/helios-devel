#pragma once

#include "HeliosEngine/Renderer/Camera/OrthographicCamera.h"
#include "HeliosEngine/Core/Timestep.h"
#include "HeliosEngine/Events/ApplicationEvent.h"
#include "HeliosEngine/Events/MouseEvent.h"


namespace Helios {


	class UICameraController
	{
	public:
		UICameraController();

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }
	private:
		void CalculateView();
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio = 1.0f;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	};


} // namespace Helios
