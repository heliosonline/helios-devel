#include "pch.h"

#include "HeliosEngine/Core/Application.h"
#include <Windows.h>


#ifdef BUILD_DEBUG
// ConsoleApp for stdout debuging
int main(int argc, char** argv)
#else
// WindowedApp
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#endif
{
	return Helios::AppMain(__argc, __argv);
}
