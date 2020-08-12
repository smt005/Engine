#pragma once

#include <vector>
#include <functional>
#include <memory>

namespace Engine {
class Event;
typedef std::function<void()> EventFunction;
typedef std::shared_ptr<Event> EventItemPtr;
typedef std::weak_ptr<Event> EventItemWptr;

class Event {
public:
	Event(EventFunction& function) : _function(function){}

private:
	EventFunction _function;

private:
	static std::vector<EventItemWptr> _events;
};

}

/*namespace Engine {
	typedef std::function<void(void)> EventFunction;
	typedef std::shared_ptr<EventFunction> EventItemPtr;
	typedef std::weak_ptr<EventFunction> EventItem;

	template <class EventT>
	class Event {
	public:
		Event(EventItemPtr ptr);
		~Event();
		virtual void actionOnEvent() {}

	public:
		static void actionEvent();

	private:
		static std::vector<EventItem> items;
	};

	template <class EventT>
	std::vector<EventItem> Event<EventT>::items;

	template <class EventT>
	Event<EventT>::Event(EventItemPtr ptr) {
		std::weak_ptr<EventFunction> wPtr(ptr);
		items.emplace_back(wPtr);
	}

	template <class EventT>
	Event<EventT>::~Event() {
		//for (std::vector<Item>::iterator& it = items.begin(); std::vector<Item>::iterator& it != items.end(); ++it) {
		//for (auto& it = items.begin(); auto& it != items.end(); ++it) {
			//if (*it == this) {
//				items.erase(it);
		//	}
	//	}
	}

	template <class EventT>
	void Event<EventT>::actionEvent() {
		for (EventItem wPtr : items) {
			EventItemPtr ptr = wPtr.lock();
			if (ptr && (*ptr)) {
				(*ptr)();
			}
		}
	}
}*/
