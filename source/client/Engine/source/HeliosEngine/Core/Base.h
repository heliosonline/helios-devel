#pragma once

// Standart C/C++
#include <string>
#include <filesystem>
#include <memory>
#include <functional>
#include <utility>
#include <unordered_map>


// Platform
#if defined(BUILDTARGET_WINDOWS)
#	include "Platform/System/Windows/WinBase.h"
#elif defined(BUILDTARGET_LINUX)
#	include "Platform/System/Linux/LinuxBase.h"
#elif defined(BUILDTARGET_MACOS)
#	include "Platform/System/MacOS/MacOSBase.h"
#endif


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
