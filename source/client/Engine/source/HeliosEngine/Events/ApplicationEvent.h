#pragma once

#include "HeliosEngine/Events/Event.h"


namespace Helios {


	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		HE_EVENT_CLASS_TYPE(WindowResize)
		HE_EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};


	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		HE_EVENT_CLASS_TYPE(WindowClose)
		HE_EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		HE_EVENT_CLASS_TYPE(AppTick)
		HE_EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		HE_EVENT_CLASS_TYPE(AppUpdate)
		HE_EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		HE_EVENT_CLASS_TYPE(AppRender)
		HE_EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


} // namespace Helios
