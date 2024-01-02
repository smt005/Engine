#pragma once

class Draw2 final {
public:
	template<typename ModelT>
	static void Model(ModelT&& object);

	template<typename ObjectT>
	static void Object(ObjectT&& object);

	template<typename ObjectsT>
	static void Objects(ObjectsT&& object);
};
