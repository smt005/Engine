
#include "FileManager.h"
#include <algorithm>
#include <filesystem>

using namespace Engine;

std::filesystem::path FileManager::_resourcesDir = std::filesystem::current_path();

void FileManager::setResourcesDir(const std::filesystem::path& resourcesDir)
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	_resourcesDir = std::filesystem::absolute(currentPath / resourcesDir);
}

const std::filesystem::path& FileManager::getResourcesDir()
{
	return _resourcesDir;
}

std::filesystem::path FileManager::fullPath(const std::filesystem::path& fileName) {
	return _resourcesDir / fileName;
}

bool FileManager::readTextFile(const std::filesystem::path& fileName, char*& data, int& lenght)
{
	FILE* file;
	std::filesystem::path fullFilePath = _resourcesDir / fileName;

	if (fopen_s(&file, fullFilePath.string().c_str(), "r") == NULL)
	{
		fseek(file, 0, SEEK_END);
		lenght = ftell(file);
		rewind(file);

		if (lenght == 0) {
			fclose(file);
			return false;
		}

		data = (char *)malloc(sizeof(char)* (lenght + 1));
		lenght = fread(data, sizeof(char), lenght, file);
		data[lenght] = '\0';

		fclose(file);

		return true;
	}

	return false;
}

std::string FileManager::readTextFile(const std::filesystem::path& fileName)
{
	char* data = nullptr;
	int lenght = 0;

	if (readTextFile(fileName, data, lenght) && data && lenght > 0) {
		return std::string(data);
	}

	return std::string();
}

bool FileManager::writeFile(const std::filesystem::path& fileName, const char* data)
{
	std::filesystem::path fullFilePath = _resourcesDir / fileName;
	std::filesystem::path realFileName;
	std::filesystem::path fullPath;

	fullPath = fullFilePath.parent_path();

	if (!std::filesystem::exists(fullPath)) {
		std::filesystem::create_directory(fullPath);
	}

	FILE* file;
	if (fopen_s(&file, fullFilePath.string().c_str(), "w") != NULL) {
		return false;
	}

	fputs(data, file);
	fclose(file);
	return true;
}

bool FileManager::writeTextFile(const std::filesystem::path& fileName, const std::string& text)
{
	return writeFile(fileName, text.c_str());
}