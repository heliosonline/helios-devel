#pragma once

#include "HeliosEngine/Core/Layer.h"
#include "HeliosEngine/Renderer/Camera/UICameraController.h"


namespace Helios {


	class UILayer : public Layer
	{
	public:
		UILayer(const std::string& name = "Layer");
		~UILayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Helios::Timestep ts) override;
		void OnEvent(Helios::Event& e) override;

		OrthographicCamera& GetCamera() { return m_CameraController.GetCamera(); }

	private:
		UICameraController m_CameraController;
	};


} // namespace Helios
