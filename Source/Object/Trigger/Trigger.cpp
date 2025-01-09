#include "Trigger.h"

namespace Trigger {
	void CenterDistance::Action()
	{
		auto itsRemove = std::remove_if(_objects.begin(), _objects.end(), [](const std::pair<Object::Wptr, std::function<void(void)>>& itPair) {
			return itPair.first.expired();
		});

		_objects.erase(itsRemove, _objects.end());

		for (const auto& [object, callback] : _objects) {
			const auto& posObject = object.lock()->getPos();
			float dist = glm::distance(_pos, posObject);

			if (dist <= _distance) {
				callback();
			}
		}
	}
}
