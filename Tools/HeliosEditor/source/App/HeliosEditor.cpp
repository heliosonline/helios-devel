#include "pch.h"

#include "config/version.h"

#include "App/EditorLayer.h"


class HeliosEditor : public Helios::Application
{
public:
	HeliosEditor(const Helios::ApplicationSpecification& specification)
		: Helios::Application(specification)
	{
		LOG_INFO("HeliosEditor v{0}.{1}.{2}.{3}", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_BUILD);
		PushLayer(new EditorLayer());
	}

	~HeliosEditor()
	{
	}
};


Helios::Application* Helios::CreateApplication(Helios::ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "HeliosEditor";
	spec.CommandLineArgs = args;
	spec.hints |= Helios::Hints::HINT_USE_EXEPATH_AS_CWD;

	return new HeliosEditor(spec);
}
