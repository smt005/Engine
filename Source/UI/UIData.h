
#pragma once

#include <vector>
#include <memory>
#include "json/value.h"
#include "../Object/Identify.h"
#include "Object/Color.h"

namespace UI
{
	enum class Parameter
	{
		ALIGMENT_CENTER,
		ALIGMENT_LEFT,
		ALIGMENT_RIGHT,
		ALIGMENT_TOP,
		ALIGMENT_BUTTOM
	};

	class UIData;
	typedef std::shared_ptr<UIData> UIDataPtr;

	struct UITexCoord {
		float st0;
	};

	class UIData : public Name
	{
	private:
		float _width;
		float _height;
		Color _color;

		Parameter _vAligment;
		Parameter _hAligment;

		std::vector<UIDataPtr> _childs;
		std::vector<std::string> _textures;

	public:
		static UIDataPtr makeData(Json::Value& dataJson);
		static bool load(UIDataPtr& dataPtr, const std::string& name);
		static Json::Value makeJson(UIDataPtr& dataPtr);
		static bool save(UIDataPtr& dataPtr, const std::string& fileName);
	};
}
