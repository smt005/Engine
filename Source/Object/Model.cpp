#include "Model.h"
#include "FileManager.h"
#include "Common/Help.h"

#define FILE_NAME_MODEL_DATA	"Models/Models.json"
#define FILE_NAME_SHAPE_FILE	"Models/Box1.obj"
#define FILE_NAME_TEXTURE_FILE	"Textures/BlackWhite.png"

std::string Model::modelPath = FILE_NAME_MODEL_DATA;

Model::Model()
	: Color(Color::WHITE)
{

}

Shape& Model::getShape()
{
	if (_shape) return *_shape;
	_shape = Shape::getByName(getName());
	return *_shape;
}

Mesh& Model::getMesh()
{
	if (_shape) return *_shape;
	_shape = Shape::getByName(getName());
	return *_shape;
}

Texture& Model::texture()
{
	if (_texture) return *_texture;
	_texture = Texture::getByName(getName());
	return *_texture;
}

const unsigned int& Model::textureId()
{
	if (_texture) return _texture->id();
	_texture = Texture::getByName(getName());
	return _texture->id();
}

bool Model::create(const string &newName)
{
	setName(newName);
	return load();
}

bool Model::load() {
	Json::Value& dataModel = data(getName());
	if (dataModel.empty())
		return false;

	const string &nameShape = dataModel["shape"].empty() ? FILE_NAME_SHAPE_FILE : dataModel["shape"].asString();
	const string &nameTexture = dataModel["texture"].empty() ? FILE_NAME_TEXTURE_FILE : dataModel["texture"].asString();

	bool hasScalling = false;
	string suffixScale;
	float scale[3];

	if (!dataModel["scale"].empty()) {
		if (dataModel["scale"].isArray())
		{
			int index = 0;
			auto& arrayScale = dataModel["scale"];

			for (auto it = arrayScale.begin(); it != arrayScale.end(); ++it)
			{
				float value = it->asFloat();
				scale[index] = value;

				if (index >= 2) break;
				++index;
			}

			hasScalling = true;
		}
		else if (dataModel["scale"].isDouble())
		{
			float value = dataModel["scale"].asFloat();

			if (value == 0.0f) {
				value = 1.0f;
			}

			scale[0] = value;
			scale[1] = value;
			scale[2] = value;

			hasScalling = true;
		}
	}

	if (!hasScalling) {
		_shape = Shape::getByName(nameShape);
	} else {
		string nameWithSuffixScale = nameShape +"_[" + std::to_string(scale[0]) + '_' + std::to_string(scale[1]) + '_' + std::to_string(scale[2]) + "]";

		if (!Shape::hasByName(nameWithSuffixScale)) {
			ShapePtr& shape = Shape::getByName(nameShape);
			_shape = std::make_shared<Shape>(*shape);
			_shape->setName(nameWithSuffixScale);
			_shape->setScale(scale);
			Shape::add(_shape);
		}

		_shape = Shape::getByName(nameWithSuffixScale);
	}

	_texture = Texture::getByName(nameTexture);

	return true;
}

// STATIC

Json::Value Model::_data;

Json::Value& Model::data(const string &name)
{
	if (_data.empty()) {
		help::loadJson(FILE_NAME_MODEL_DATA, _data);
	}

	return _data[name];
}

void Model::removeData()
{
	_data.clear();
}
