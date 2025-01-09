#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <glm/vec3.hpp>

template<typename T>
class LogType {
public:
	LogType(const T& value)
		: _value(value)
	{}

	std::string Get()
	{
		std::ostringstream oss;
		oss << _value;
		return oss.str();
	}

private:
	T _value;
};

template<>
class LogType <glm::vec3> {
public:
	LogType(const glm::vec3& value)
		: _value(value)
	{}

	std::string Get()
	{
		std::ostringstream oss;
		oss << _value.x << ", "  << _value.y << ", " << _value.z;
		return oss.str();
	}

private:
	glm::vec3 _value;
};

template<typename T>
std::string ToString(const T& value) {
	LogType<T> logType(value);
	return logType.Get();
}

template<typename T>
std::string MakeString(const std::string format, const T& last) {
	const size_t posOpen = format.find('{');
	if (posOpen == format.npos) {
		return format;
	}
	const size_t len = format.length();
	size_t posClose = format.find('}', posOpen);
	if (posClose != format.npos && ++posClose < len) {
		return format.substr(0, posOpen) + ToString(last) + format.substr(posClose, len - posClose);
	}
	return format.substr(0, posOpen) + ToString(last);
}

template<typename T, typename... Args>
std::string MakeString(const std::string format, const T& first, const Args&... args) {
	const size_t posOpen = format.find('{');
	if (posOpen == format.npos) {
		return format;
	}
	const size_t len = format.length();
	size_t posClose = format.find('}', posOpen);
	if (posClose != format.npos && ++posClose < len) {
		return format.substr(0, posOpen) + ToString(first) + MakeString(format.substr(posClose, len - posClose), args...);
	}
	return format.substr(0, posOpen) + ToString(first) + MakeString(std::string(), args...);
}

template<typename... Args>
void Log(const std::string format, const Args&... args) {
	const std::string text = MakeString(format, args...);
	std::cout << text << std::endl;
#ifdef _DEBUG
	_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", text.c_str());
#endif // DEBUG
}
