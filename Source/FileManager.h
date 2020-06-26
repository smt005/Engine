#pragma once

#include <string>
#include <filesystem>

namespace Engine
{
	
class FileManager
{
public:
	static void setResourcesDir(const std::filesystem::path& resourcesDir);
	static const std::filesystem::path& getResourcesDir();
	static std::filesystem::path fullPath(const std::filesystem::path& fileName);

	static bool readTextFile(const std::filesystem::path& fileName, char*& data, int& lenght);
	static std::string readTextFile(const std::filesystem::path& fileName);

	static bool writeFile(const std::filesystem::path& fileName, const char* data);
	static bool writeTextFile(const std::filesystem::path& fileName, const std::string& text);

private:
	static std::filesystem::path _resourcesDir;
};

};	// Engine