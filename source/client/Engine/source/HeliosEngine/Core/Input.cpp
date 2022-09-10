#include "pch.h"

#include "HeliosEngine/Core/Input.h"
#include "HeliosEngine/Core/Application.h"

#include <GLFW/glfw3.h>


namespace Helios {


	bool Input::IsKeyPressed(const KeyCode key)
	{
		HE_PROFILER_FUNCTION();

		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}


	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		HE_PROFILER_FUNCTION();

		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}


	glm::vec2 Input::GetMousePosition()
	{
		HE_PROFILER_FUNCTION();

		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}


	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}


	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}


} // namespace Helios
