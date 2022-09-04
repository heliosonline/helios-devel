#include "pch.h"

#include <glad/gl.h>


// force ImGui to use the glad loader
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

// disable MSVC's warning about deprecated/unsafe code
#pragma warning(disable : 4996)

// TODO: needs to be switchable on runtime (like Platform/Renderer/...)
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"
