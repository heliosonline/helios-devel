#include "pch.h"

#include "HeliosEngine/UI/UILayer.h"


namespace Helios {


	UILayer::UILayer(const std::string& debugName)
		: Layer(debugName), m_CameraController()
	{
	}


	void UILayer::OnAttach()
	{
	}


	void UILayer::OnDetach()
	{
	}


	void UILayer::OnUpdate(Timestep ts)
	{
		m_CameraController.OnUpdate(ts);
	}


	void UILayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}


} // namespace Helios
