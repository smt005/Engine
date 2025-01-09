#pragma once

#include <memory>
#include <vector>
#include "json/json.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Common/DataClass.h"
#include "Object/Color.h"
#include  "Object/Object.h"
#include  "Object/Item.h"

class Camera;
class Map;
typedef std::shared_ptr<Map> MapPtr;

class Map : public DataClass <Map>, public Color
{
public:
	typedef std::shared_ptr<Map> Ptr;

public:
	static Map::Ptr& SetCurrentMap(Map::Ptr& map);
	static Map::Ptr& AddCurrentMap(Map::Ptr& map);
	static void ClearCurrentMap(Map::Ptr& map);
	static Map& GetFirstCurrentMap() { return currentMaps.empty() ? defaultMap : *currentMaps[0]; }
	static Map::Ptr GetFirstCurrentMapPtr() { return currentMaps.empty() ? Map::Ptr() : currentMaps[0]; }
	static std::vector<Map::Ptr>& GetCurrentMaps() { return currentMaps; }

public:
	Map() : _physicsState(false) {};
	Map(const string &name);
	virtual ~Map();

	virtual bool create(const string &name);
	virtual void clear();
	virtual bool load();
	virtual void action();

	void getDataJson(Json::Value& dataJson);

	// Physics
	void initPhysixs();
	void releasePhysixs();
	void updatePhysixs();

	Object& addObjectToPos(const string& nameModel, const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0));
	Object& addObject(const string& nameModel, const glm::mat4x4& mat = glm::mat4x4(1.0));
	Object& addObject(Object* object);
	Object::Ptr& addObject(Object::Ptr& object);

	void RemoveObject(const std::string& name);

	Object::Ptr getObjectPtrByName(const std::string& name);
	Object&	getObjectByName(const std::string& name);
	const Item::Ptr& GetAdditObjectsByName(const std::string& name) const;

	std::shared_ptr<Camera>& getCamera() { return _cameraPtr; }
	std::vector<Object::Ptr>& GetObjects() { return objects; }

protected:
	std::vector<Object::Ptr> objects;

public:
	std::vector<Item::Ptr> additObjects;

private:
	static std::vector<Map::Ptr> currentMaps;
	static Map defaultMap;

private:
	bool _physicsState;
	std::shared_ptr<Camera> _cameraPtr;
	Object _defaultObject;
};
