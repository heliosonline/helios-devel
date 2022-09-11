#pragma once


#include "HeliosEngine/HeliosConfig.h"


// Platform
#include "Platform/PlatformDetection.h"
#if defined(TARGET_PLATFORM_WINDOWS)
#	include "Platform/System/Windows/WinBase.h"
#elif defined(TARGET_PLATFORM_LINUX)
#	include "Platform/System/Linux/LinuxBase.h"
#elif defined(TARGET_PLATFORM_MACOS)
#	include "Platform/System/MacOS/MacOSBase.h"
#endif


// Standart C/C++
#include <algorithm>
#include <utility>
#include <filesystem>
#include <functional>
#include <memory>

#include <fstream>
#include <iostream>
#include <sstream>

#include <array>
#include <string>
#include <vector>

#include <unordered_map>
#include <unordered_set>

#include "glm/glm.hpp"


namespace Helios {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

} // namespace Helios


#include "HeliosEngine/Core/Log.h"
#include "HeliosEngine/Debug/Instrumentor.h"
