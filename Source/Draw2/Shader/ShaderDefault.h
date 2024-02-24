#pragma once

#include "ShaderInterface.h"

class ShaderDefault final : public ShaderInterface {
public:
	using Ptr = std::shared_ptr<ShaderDefault>;

public:
	void Use() override;
	void GetLocation() override;
	
public:
	static unsigned int u_matProjectionView;
	static unsigned int u_matViewModel;

	static ShaderDefault::Ptr current;
};
