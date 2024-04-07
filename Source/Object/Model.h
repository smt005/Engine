// ◦ Xyz ◦
#pragma once

#include "json/json.h"
#include "Common/DataClass.h"
#include "Shape.h"
#include "Texture.h"
#include "Color.h"

#include <string>

class Model;
typedef std::shared_ptr<Model> ModelPtr;

class Model : public DataClass <Model>, public Color
{
private:
	ShapePtr _shape;
	Texture::Ptr _texture;

public:
	using Ptr = std::shared_ptr<Model>;

	Model();

	bool ValidShape();
	Shape& getShape();
	Mesh& getMesh();
	Texture& texture();
	const unsigned int& textureId();

	bool create(const std::string& name);
	bool load();

private:
public:
	static Json::Value _data;
	static std::string modelPath;

public:
	static Json::Value& data(const std::string &name);
	static void removeData();
	static std::vector<std::string> GetListModels();
};
