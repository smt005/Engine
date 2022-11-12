#pragma once

#include <string>

namespace Engine {
	void OpenFolderAndSelectItems(const std::string& filenameStr, const std::string& pathStr = std::string());
	std::string SelectFile();
}
