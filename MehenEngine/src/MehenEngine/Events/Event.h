// Tutorial : https://youtu.be/xnopUoZbMEk?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#pragma once

#include "MehenEngine/Core.h"
#include "MehenPrecompiledHeaders.h"

namespace MehenEngine
{
	enum class E_EventType : unsigned __int8
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum e_EventCategory : unsigned __int8
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput =	   BIT(1),
		EventCategoryKeyboard =	   BIT(2),
		EventCategoryMouse =	   BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static E_EventType GetStaticType() { return E_EventType::##type; }\
								virtual E_EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class MEHEN_API Event
	{
		friend class EventDispatcher;
	public:
		virtual E_EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(e_EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		bool m_handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.m_handled = func(*(T*)&m_event);
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}