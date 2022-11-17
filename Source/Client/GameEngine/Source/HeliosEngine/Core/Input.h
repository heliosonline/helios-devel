#pragma once

#include "HeliosEngine/Core/InputCodes.h"

#include <glm/glm.hpp>


namespace Helios {


	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};


} // namespace Helios
