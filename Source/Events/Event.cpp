// ◦ Xyz ◦

#include "Event.h"

Event* Event::_eventPtr = nullptr;

Event& Event::Instance()
{
	if (!_eventPtr) {
		_eventPtr = new Event();
	}
	return *_eventPtr;
}

void Event::Erase()
{
	delete _eventPtr;
	_eventPtr = nullptr;
}

void Event::Action()
{
	for (auto it = _subscriptions.begin(); it < _subscriptions.end(); ++it) {
		it->second();
	}
}

void Event::Add(const std::string& id, std::function<void()> function)
{
	auto it = std::find_if(_subscriptions.begin(), _subscriptions.end(), [&id](const auto& itPair) {
		return itPair.first == id;
	});

	if (it != _subscriptions.end()) {
		it->second = function;
	}
	else {
		_subscriptions.emplace_back(id, function);
	}
}

void Event::Remove(const std::string& id)
{
	auto it = std::find_if(_subscriptions.begin(), _subscriptions.end(), [&id](const auto& itPair) {
		return itPair.first == id;
	});

	if (it != _subscriptions.end()) {
		_subscriptions.erase(it);
	}
}