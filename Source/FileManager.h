#pragma once

#include <string>
#include <experimental\filesystem>

namespace Engine
{
	
class FileManager
{
public:
	static void setResourcesDir(const std::experimental::filesystem::path& resourcesDir);
	static const std::experimental::filesystem::path& getResourcesDir();
	static std::experimental::filesystem::path fullPath(const std::experimental::filesystem::path& fileName);

	static bool readTextFile(const std::experimental::filesystem::path& fileName, char*& data, int& lenght);
	static std::string readTextFile(const std::experimental::filesystem::path& fileName);

	static bool writeFile(const std::experimental::filesystem::path& fileName, const char* data);
	static bool writeTextFile(const std::experimental::filesystem::path& fileName, const std::string& text);

private:
	static std::experimental::filesystem::path _resourcesDir;
};

};	// Engine