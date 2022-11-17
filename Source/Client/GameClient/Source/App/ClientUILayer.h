#pragma once

#include <HeliosEngine/HeliosEngine.h>


class ClientUILayer : public Helios::UILayer
{
public:
	ClientUILayer();
	~ClientUILayer() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Helios::Timestep ts) override;
	void OnEvent(Helios::Event& e) override;
};
