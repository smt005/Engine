
#include "UIData.h"
#include "../FileManager.h"
#include "Common/Help.h"
#include "Common/HelpJson.h"

namespace UI
{
	UIDataPtr UIData::makeData(Json::Value& dataJson) {
		if (!dataJson.isObject()) {
			return nullptr;
		}

		UIData* data = new UIData();
		UIDataPtr dataPtr(data);

		Json::Value& nameJson = dataJson["name"];
		if (!nameJson.empty() && nameJson.isString()) {
			const std::string &name = nameJson.asString();
			data->setName(name);
		}

		Json::Value& heightJson = dataJson["height"];
		if (!heightJson.empty()) {
			if (heightJson.isInt() || heightJson.isDouble()) {
				float value = heightJson.asFloat();
				data->_height = value;
			}
			else  if (heightJson.isString()) {
				std::string valueStr = heightJson.asString();
				size_t len = valueStr.length();
				size_t index = valueStr.find("%");
				if (index == (len - 1)) {
					valueStr.replace(index, len, std::string());
					float value = stof(valueStr);
					data->_height = value;
				}
			}
		}

		Json::Value& widthJson = dataJson["width"];
		if (!widthJson.empty()) {
			if (widthJson.isInt() || widthJson.isDouble()) {
				float value = widthJson.asFloat();
				data->_width = value;
			}
			else  if (widthJson.isString()) {
				std::string valueStr = widthJson.asString();
				size_t len = valueStr.length();
				size_t index = valueStr.find("%");
				if (index == (len - 1)) {
					valueStr.replace(index, len, std::string());
					float value = stof(valueStr);
					data->_width = value;
				}
			}
		}

		Json::Value& alignJson = dataJson["align"];
		if (!alignJson.empty()) {
			if (alignJson.isArray()) {
				data->_vAligment = Parameter::ALIGMENT_CENTER;
				data->_hAligment = Parameter::ALIGMENT_CENTER;

				for (auto it = alignJson.begin(); it != alignJson.end(); ++it)
				{
					if (it->isString()) {
						const std::string& align = it->asString();

						if (align == "left") {
							data->_hAligment = Parameter::ALIGMENT_LEFT;
						}
						else if (align == "right") {
							data->_hAligment = Parameter::ALIGMENT_RIGHT;
						}
						else if (align == "top") {
							data->_vAligment = Parameter::ALIGMENT_TOP;
						}
						else if (align == "bottom") {
							data->_vAligment = Parameter::ALIGMENT_BUTTOM;
						}
					}
				}
			}
			else if (alignJson.isString()) {
				std::string& align = alignJson.asString();
				if (align == "center") {
					data->_vAligment = Parameter::ALIGMENT_CENTER;
					data->_hAligment = Parameter::ALIGMENT_CENTER;
				}
				else if (align == "left") {
					data->_vAligment = Parameter::ALIGMENT_CENTER;
					data->_hAligment = Parameter::ALIGMENT_LEFT;
				}
				else if (align == "right") {
					data->_vAligment = Parameter::ALIGMENT_CENTER;
					data->_hAligment = Parameter::ALIGMENT_RIGHT;
				}
				else if (align == "top") {
					data->_vAligment = Parameter::ALIGMENT_TOP;
					data->_hAligment = Parameter::ALIGMENT_CENTER;
				}
				else if (align == "bottom") {
					data->_vAligment = Parameter::ALIGMENT_BUTTOM;
					data->_hAligment = Parameter::ALIGMENT_CENTER;
				}
			}
		}

		Json::Value& childsUIData = dataJson["childs"];
		if (!childsUIData.empty() && childsUIData.isArray()) {
			for (auto itemJson : dataJson["childs"]) {
				UIDataPtr child = makeData(itemJson);
				if (child) {
					data->_childs.push_back(child);
				}
			}
		}

		Json::Value& textureUIData = dataJson["textures"];
		if (!textureUIData.empty() && textureUIData.isArray()) {
			for (auto itemJson : dataJson["textures"]) {
				if (itemJson.isString() && !itemJson.empty()) {
					const std::string &texture = itemJson.asString();
					data->_textures.push_back(texture);
				}
			}
		}

		return dataPtr;
	}

	bool UIData::load(UIDataPtr& dataPtr, const std::string& name) {
		std::string fileName = "UI/" + name + ".json";
		Json::Value data;
		help::loadJson(fileName, data);

		dataPtr = makeData(data);
		if (!dataPtr) {
			return false;
		}

		return true;
	}

	Json::Value UIData::makeJson(UIDataPtr& dataPtr) {
		Json::Value dataJson;

		dataJson["name"] = dataPtr->getName();

		if (!dataPtr->_childs.empty()) {
			for (auto& child : dataPtr->_childs) {
				dataJson["childs"].append(makeJson(child));
			}
		}

		for (auto& item : dataPtr->_textures) {
			dataJson["textures"].append(item);
		}

		return dataJson;
	}

	bool UIData::save(UIDataPtr& dataPtr, const std::string& fileName) {
		if (!dataPtr) {
			return false;
		}

		Json::Value rootUIData;

		rootUIData["name"] = dataPtr->getName();
		for (auto& item : dataPtr->_childs) {
			rootUIData["childs"].append(makeJson(item));
		}

		if (rootUIData.empty()) {
			return false;
		}

		std::string fullFileName = fileName + ".json";
		help::saveJson(fullFileName, rootUIData);
	}
}
