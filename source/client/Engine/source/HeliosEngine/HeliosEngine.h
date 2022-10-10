#pragma once

// Core
#include <HeliosEngine/Core/Base.h>
#include <HeliosEngine/Core/Application.h>
#include <HeliosEngine/Core/Layer.h>
#include <HeliosEngine/Core/Input.h>
#include <HeliosEngine/Core/Timestep.h>
#include <HeliosEngine/Core/UUID.h>

// Events
#include <HeliosEngine/Events/Event.h>
#include <HeliosEngine/Events/ApplicationEvent.h>
#include <HeliosEngine/Events/KeyEvent.h>
#include <HeliosEngine/Events/MouseEvent.h>

// Renderer (core)
#include <HeliosEngine/Renderer/Renderer.h>
#include <HeliosEngine/Renderer/Renderer2D.h>
#include <HeliosEngine/Renderer/RenderCommand.h>
// Renderer (wrapper)
#include <HeliosEngine/Renderer/Buffer.h>
#include <HeliosEngine/Renderer/Framebuffer.h>
#include <HeliosEngine/Renderer/Shader.h>
#include <HeliosEngine/Renderer/Texture.h>
#include <HeliosEngine/Renderer/VertexArray.h>
// Renderer (abstract)
#include <HeliosEngine/Renderer/SubTexture2D.h>
#include <HeliosEngine/Renderer/OrthographicCamera.h>
#include <HeliosEngine/Renderer/OrthographicCameraController.h>
#include <HeliosEngine/Renderer/Font.h>

// EntryPoint for the Application
#include <HeliosEngine/Core/EntryPoint.h>

// Most used libraries
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
