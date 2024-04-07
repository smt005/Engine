// ◦ Xyz ◦
#pragma once

#include <string>
#include <vector>
#include <set>
#include <functional>
#include "MyStl/Singleton.h"

template<typename T>
class Event : public mystd::Singleton<T> {
public:
	Event() = default;
	virtual ~Event() = default;

	void Action()
	{
		_inActive = true;

		for (auto& pairObserver : _observers) {
			pairObserver.second();
		}

		_inActive = false;

		// TODO: оптимизировать
		if (!_deleteObservers.empty()) {
			for (const std::string& id : _deleteObservers) {
				RemoveFromVector(id);
			}
			_deleteObservers.clear();
		}
	}

	void Add(const std::string& id, std::function<void()> function) {
		_observers.emplace_back(id, function);
	}

	void Remove(const std::string& id) {
		if (_inActive) {
			_deleteObservers.emplace_back(id);
		}
		else {
			RemoveFromVector(id);
		}
	}

private:
	void RemoveFromVector(const std::string& id)
	{
		auto it = std::find_if(_observers.begin(), _observers.end(), [&id](const std::pair<std::string, std::function<void()>>& pair) {
			return pair.first == id;
		});
		if (it != _observers.end()) {
			_observers.erase(it);
		}
	}

private:
	bool _inActive = false;
	std::vector<std::pair<std::string, std::function<void()>>> _observers;
	std::list<std::string> _deleteObservers;
};
