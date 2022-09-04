#pragma once

#include "HeliosEngine/Core/Layer.h"

#include "HeliosEngine/Events/ApplicationEvent.h"
#include "HeliosEngine/Events/KeyEvent.h"
#include "HeliosEngine/Events/MouseEvent.h"


namespace Helios {


	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& inipath);
		~ImGuiLayer() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block;  }

		void SetThemeColors();
	private:
		bool m_BlockEvents = true;
		std::string m_IniPath;
	};


} // namespace Helios
