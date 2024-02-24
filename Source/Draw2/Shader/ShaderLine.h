#pragma once

#include "ShaderInterface.h"

class ShaderLine final : public ShaderInterface {
public:
	using Ptr = std::shared_ptr<ShaderLine>;

public:
	void Use() override;
	void GetLocation() override;

public:
	static unsigned int u_matProjectionView;
	static unsigned int u_matViewModel;
	static unsigned int u_color;

	static ShaderLine::Ptr current;
};
