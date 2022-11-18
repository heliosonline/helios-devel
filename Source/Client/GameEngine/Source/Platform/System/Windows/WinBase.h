#pragma once

// On Windows, a header file defines two macros min and max which may result in conflicts
// with their counterparts in the standard library and therefore in errors during compilation.
// See https://github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#ifndef NOMINMAX
#	define NOMINMAX
#endif

// default includes
#include <windows.h>
