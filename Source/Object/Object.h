#pragma once

#include "json/json.h"
#include "glm/mat4x4.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Position.h"
#include "Identify.h"
#include <string>

using namespace std;
using namespace glm;

class Model;
typedef std::shared_ptr<Model> ModelPtr;

class Object : public Position, public Name
{
public:
	int tag = 0;

protected:
	ModelPtr _model;
	bool _visible = true;

public:
	Object();
	Object(const string &name, const string &modelName, const vec3 &pos = vec3(0.0f), const Json::Value& data = Json::Value());
	virtual ~Object();

	void getDataJson(Json::Value& dataJson);

	Model& getModel();
	const float& getHeight();

	void set(const string &name, const string &modelName, const vec3 &pos = vec3(0.0f), const Json::Value& data = Json::Value());
	void setHeight(const float &height);

	inline bool visible() { return _visible; }
	inline void setVisible(const bool visible) { _visible = visible; }

public:
	virtual void setData(const Json::Value &data);
	virtual void action();

public:
	static Object* _default;

public:
	static Object& defaultItem();
	static void removeDefault();
};
