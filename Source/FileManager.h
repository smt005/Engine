#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace Engine
{
	
class FileManager {
public:
	static void setResourcesDir(const std::filesystem::path& resourcesDir);
	static const std::filesystem::path& getResourcesDir();
	static std::filesystem::path fullPath(const std::filesystem::path& fileName);

	static bool readTextFile(const std::filesystem::path& fileName, char*& data, int& lenght);
	static std::string readTextFile(const std::filesystem::path& fileName, const std::filesystem::path& resourcesDir = _resourcesDir);
	static std::string readTextFileRedject(const std::filesystem::path& fileName);

	static bool writeFile(const std::filesystem::path& fileName, const char* data, const std::filesystem::path& resourcesDir);
	static bool writeTextFile(const std::filesystem::path& fileName, const std::string& text, const std::filesystem::path& resourcesDir = _resourcesDir);

	static void FindFiles(const std::filesystem::path& dir, const std::string& mask, std::vector<std::string>& result);

private:
	static std::filesystem::path _resourcesDir;
};

};	// Engine