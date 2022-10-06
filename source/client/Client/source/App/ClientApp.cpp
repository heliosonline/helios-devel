#include "pch.h"

#include "config/version.h"

#include "App/ClientLayer.h"


class HeliosGame : public Helios::Application
{
public:
	HeliosGame(const Helios::ApplicationSpecification& specification)
		: Helios::Application(specification)
	{
		LOG_INFO("HeliosClient v{0}.{1}.{2}.{3}", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_BUILD);
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
