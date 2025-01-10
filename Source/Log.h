// ◦ Xyz ◦

#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <glm/vec3.hpp>

template<typename T>
struct LogType {
	LogType(const T& value, std::ostringstream& oss) {
		oss << value;
	}
};

// Спецификация . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
template<>
struct LogType <glm::vec3> {
	LogType(const glm::vec3& vec, std::ostringstream& oss) {
		oss << vec.x << ", " << vec.y << ", " << vec.z;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class LogClass {
public:
	std::string GetString() {
		return _oss.str();
	}

	template<typename T>
	void MakeString(const std::string_view format, const T& last) {
		const size_t posOpen = format.find('{');
		if (posOpen == format.npos) {
			_oss << format;
			return;
		}

		const size_t len = format.length();
		size_t posClose = format.find('}', posOpen);
		if (posClose != format.npos && ++posClose < len) {
			_oss << format.substr(0, posOpen);
			LogType<T>(last, _oss);
			_oss << format.substr(posClose, len - posClose);
			return;
		}

		_oss << format.substr(0, posOpen);
		LogType<T>(last, _oss);
	}

	template<typename T, typename... Args>
	void MakeString(const std::string_view format, const T& first, const Args&... args) {
		const size_t posOpen = format.find('{');
		if (posOpen == format.npos) {
			_oss << format;
			return;
		}

		const size_t len = format.length();
		size_t posClose = format.find('}', posOpen);
		if (posClose != format.npos && ++posClose < len) {
			_oss << format.substr(0, posOpen);
			LogType<T>(first, _oss);
			MakeString(format.substr(posClose, len - posClose), args...);
			return;
		}

		_oss << format.substr(0, posOpen);
		LogType<T>(first, _oss);
		MakeString(std::string(), args...);
	}

private:
	std::ostringstream _oss;
};

template<typename... Args>
void Log(const std::string_view format, const Args&... args) {
	LogClass log;
	log.MakeString(format, args...);
	const std::string text = log.GetString();
	std::cout << text << std::endl;

#ifdef _DEBUG
	_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", text.c_str());
#endif // DEBUG
}

template<typename T>
void Log(const T& value) {
	std::ostringstream oss;
	LogType<T>(value, oss);
	const std::string text = oss.str();
	std::cout << text << std::endl;

#ifdef _DEBUG
	_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", text.c_str());
#endif // DEBUG
}
