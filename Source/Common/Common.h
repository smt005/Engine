#pragma once
#include <string>

namespace Engine {
	class Core;
	
	template<typename T, typename RetT = std::string>
	RetT GetClassName() {
		RetT className = typeid(T).name();
		return className.substr(6, (className.length() - 6)); // 'slass ' length == 6
	}
}
