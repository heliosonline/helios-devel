#include "pch.h"

#include "config/version.h"

#include "App/ClientLayer.h"


class HeliosGame : public Helios::Application
{
public:
	HeliosGame(const Helios::ApplicationSpecification& specification)
		: Helios::Application(specification)
	{
		PushLayer(new ClientLayer());
	}

	~HeliosGame()
	{
	}
};


Helios::Application* Helios::CreateApplication(Helios::ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "HeliosGame";
	spec.CommandLineArgs = args;
	spec.hints |= Helios::Hints::HINT_USE_EXEPATH_AS_CWD;

	return new HeliosGame(spec);
}
