#pragma once
#include <string>

namespace Engine {
	template<typename T, typename RetT = std::string>
	RetT GetClassName() {
		RetT className = typeid(T).name();
		return className.substr(6, (className.length() - 6)); // 'class ' length == 6
	}

	template<typename T, typename RetT = std::string>
	RetT GetClassName(T*) {
		RetT className = typeid(T).name();
		return className.substr(6, (className.length() - 6)); // 'class ' length == 6
	}
}
