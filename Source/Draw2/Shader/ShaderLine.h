#pragma once

#include "ShaderInterface.h"
#include "../../MyStl/Singleton.h"

class ShaderLine final : public ShaderInterface, public mystd::Singleton<ShaderLine> {
public:
	void Use() override;
	void GetLocation() override;

public:
	static unsigned int u_color;
};

class ShaderLineP final : public ShaderInterface, public mystd::Singleton<ShaderLineP> {
public:
	void Use() override;
	void GetLocation() override;

public:
	static unsigned int u_matProjectionView;
	static unsigned int u_color;
};

class ShaderLinePM final : public ShaderInterface, public mystd::Singleton<ShaderLinePM> {
public:
	void Use() override;
	void GetLocation() override;

public:
	static unsigned int u_matProjectionView;
	static unsigned int u_matViewModel;
	static unsigned int u_color;
};

class ShaderGravityPoint final : public ShaderInterface, public mystd::Singleton<ShaderGravityPoint> {
public:
	void Use() override;
	void GetLocation() override;

public:
	static unsigned int u_matProjectionView;
	static unsigned int u_color;
};
