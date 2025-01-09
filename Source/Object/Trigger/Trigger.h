#pragma once

#include <functional>
#include <Object/Item.h>
#include <Object/Object.h>

namespace Trigger {
	class BaseTrigger : public Item
	{
	public:
		typename std::shared_ptr<BaseTrigger> Ptr;

		void AddObject(const Object::Wptr& object, const std::function<void(void)> callback) {
			_objects.emplace_back(object, callback);
		}

	protected:
		std::vector<std::pair<Object::Wptr, std::function<void(void)>>> _objects;
	};

	// CenterDistance
	class CenterDistance : public BaseTrigger
	{
	public:
		typename std::shared_ptr<CenterDistance> Ptr;

		CenterDistance(const vec3& pos, float dist, const string& name = std::string())
			: _pos(pos)
			, _distance(dist)
			, _name(name)
		{}
		
		void Action() override;
		const std::string& GetName() const override { return _name; }

		void SetDistance(float distance) {
			_distance = distance;
		} 

		float GetDistance() const {
			return _distance;
		}

	private:
		vec3 _pos = { 0.f, 0.f, 0.f };
		float _distance = 50.f;
		std::string _name;
	};
}
