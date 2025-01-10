// ◦ Xyz ◦
#pragma once

#include <vector>
#include <string>
#include <functional>

class Event {
public:
	static Event& Instance();
	static void Erase();

private:
	Event() = default;
	~Event() = default;

public:
	void Action();
	void Add(const std::string& id, std::function<void()> function);
	void Remove(const std::string& id);

private:
	std::vector<std::pair<std::string, std::function<void()>>> _subscriptions;

private:
	static Event* _eventPtr;
};

