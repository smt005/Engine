#pragma once

#include "ShaderInterface.h"
#include "../../MyStl/Singleton.h"

class ShaderDefault final : public ShaderInterface, public mystd::Singleton<ShaderDefault> {
public:
	void Use() override;
	void GetLocation() override;
	
public:
	static unsigned int u_matProjectionView;
	static unsigned int u_matViewModel;
	static unsigned int u_color;
};
