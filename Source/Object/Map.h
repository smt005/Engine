#pragma once

#include "json/json.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Common/DataClass.h"
#include "Object/Color.h"
#include  "Object/Object.h"

#include <memory>
#include <vector>

class Object;
class Glider;
class Camera;
class Map;
typedef std::shared_ptr<Map> MapPtr;

class Map : public DataClass <Map>, public Color
{
public:
	typedef std::shared_ptr<Map> Ptr;

public:
	static void SetCurrentMap(Map::Ptr& map);
	static void AddCurrentMap(Map::Ptr& map);
	static void ClearCurrentMap(Map::Ptr& map);
	static Map& GetFirstCurrentMap() { return currentMaps.empty() ? defaultMap : *currentMaps[0]; }
	static std::vector<Map::Ptr>& GetCurrentMaps() { return currentMaps; }

public:
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

	Object* getObjectPtrByName(const std::string& name);
	Object&	getObjectByName(const std::string& name);
	const Camera& getCamera();

	std::vector<Object*>& GetObjects() { return objects; }
	std::vector<Glider*>& GetGliders() { return gliders; }
	
private:
	std::vector<Object*> objects;
	std::vector<Glider*> gliders;

private:
	static std::vector<Map::Ptr> currentMaps;
	static Map defaultMap;

private:
	bool _physicsState;
	std::shared_ptr<Camera> _cameraPtr;
	Object _defaultObject;
};
