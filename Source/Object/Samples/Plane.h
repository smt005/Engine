#pragma once
#include "../Mesh.h"
#include "../Color.h"
#include "Math/Vector.h"

class Plane final : public Mesh, public Color {
public:
	Plane() {
		Make(Math::Vector3(-1.f, -1.f, -1.f), Math::Vector3(1.f, 1.f, 1.f));
	}
	
	Plane(const Math::Vector3& min, const Math::Vector3& max) {
		Make(min, max);
	}

	void Make(const Math::Vector3& min, const Math::Vector3& max);
};
