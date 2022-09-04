#pragma once


#ifdef BUILDTARGET_WINDOWS
#ifndef NOMINMAX
// See https://github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif
#endif


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

#include "HeliosEngine/Core/Base.h"
#include "HeliosEngine/Core/Log.h"
