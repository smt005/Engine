
#pragma once

#include "json/json.h"

namespace help
{
	std::string getString(const Json::Value& data, const std::string& key) {
		const Json::Value& nameData = data[key];
		if (!nameData.empty() && nameData.isString()) {
			return nameData.asString();
		}
		else {
			return std::string();
		}
	}


	int getInt(const Json::Value& data, const std::string& key) {
		const Json::Value& valueJson = data[key];
		if (!valueJson.empty() && !valueJson.isArray() && !valueJson.isObject()) {
			return valueJson.asInt();
		}
		else {
			return 0;
		}
	}
};
