#pragma once
#include <memory>
#include "../Mesh.h"
#include "../Color.h"
#include "Math/Vector.h"

class Box final : public Mesh, public Color {
public:
	using Ptr = std::shared_ptr<Box>;

public:
	Box() {
		Make(Math::Vector3(-1.f, -1.f, -1.f), Math::Vector3(1.f, 1.f, 1.f));
	}

	Box(const Math::Vector3& min, const Math::Vector3& max) {
		Make(min, max);
	}

	void Make(const Math::Vector3& min, const Math::Vector3& max);
};
