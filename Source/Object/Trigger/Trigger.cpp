#include "Trigger.h"

namespace Trigger {
	void CenterDistance::Action()
	{
		for (const auto& [object, callback] : _objects) {
			const auto& posObject = object->getPos();
			float dist = glm::distance(_pos, posObject);

			if (dist <= _distance) {
				callback();
			}
		}
	}
}
