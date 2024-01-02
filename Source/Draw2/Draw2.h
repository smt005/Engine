#pragma once

class Draw2 final {
public:
	static void SetClearColor(float r, float g, float b, float a);
	static void ClearColor();
	static void ClearDepth();
	static void Viewport();

	template<typename ModelT>
	static void Model(ModelT&& object);

	template<typename ObjectT>
	static void Object(ObjectT&& object);

	template<typename ObjectsT>
	static void Objects(ObjectsT&& object);

	static float clearColor[4];
};
