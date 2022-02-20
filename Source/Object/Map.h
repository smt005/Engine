#pragma once

#include "json/json.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Common/DataClass.h"
#include "Object/Color.h"

#include <memory>
#include <vector>

class Object;
class Glider;
class Map;
typedef std::shared_ptr<Map> MapPtr;

class Map : public DataClass <Map>, public Color
{
public:
	typedef std::shared_ptr<Map> Ptr;

public:
	std::vector<Object*> objects;
	std::vector<Glider*> gliders;

	Map() : _physicsState(false) {};
	Map(const string &name);
	virtual ~Map();

	bool create(const string &name);
	void clear();
	bool load();

	void getDataJson(Json::Value& dataJson);
	void action();

	// Physics
	void initPhysixs();
	void releasePhysixs();
	void updatePhysixs();

	Object& addObjectToPos(const string& nameModel, const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0));
	Object& addObject(const string& nameModel, const glm::mat4x4& mat = glm::mat4x4(1.0));
	Object& addObject(Object* object);

	Object*	getObjectByName(const std::string& name);

private:
	bool _physicsState;
};
