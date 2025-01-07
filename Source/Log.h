#pragma once
#include <string>
#include <iostream>
#include <vector>

namespace _Log_ {
	template<typename T>
	std::string ToString(const T& value) {
		std::ostringstream oss;
		oss << value;
		return oss.str();
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
}

template<typename... Args>
void Log(const std::string format, const Args&... args) {
	const std::string text = _Log_::MakeString(format, args...);
	std::cout << text << std::endl;
#ifdef _DEBUG
	_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", text.c_str());
#endif // DEBUG
}
