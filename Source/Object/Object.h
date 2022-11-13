#pragma once

#include "json/json.h"
#include "glm/mat4x4.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Position.h"
#include "Identify.h"
#include "Physics/Physics.h"
#include <string>

using namespace std;
using namespace glm;

class Model;
typedef std::shared_ptr<Model> ModelPtr;

class Object : public Position, public Name
{
	friend Engine::Physics;

public:
	typedef std::shared_ptr<Object> Ptr;

public:
	int tag = 0;

protected:
	ModelPtr _model;
	bool _visible = true;
	double _createTime;

private:
	// Physics
	Engine::Physics::Type _typePhysics;
	Engine::physics::ActorPhyscs* _actorPhyscs;

public:
	Object();
	Object(const string& name, const string& modelName, const vec3& pos = vec3(0.0f));
	virtual ~Object();

	void getDataJson(Json::Value& dataJson);

	Model& getModel();
	const float& getHeight();

	float hit(const int x, const int y);

	void set(const string& name, const string& modelName, const vec3& pos = vec3(0.0f));
	void setHeight(const float& height);

	void setActorPos(const glm::vec3& pos);
	
	// Physics
	void setTypeActorPhysics(const Engine::Physics::Type typePhysics) { _typePhysics = typePhysics; }
	void updateMatrixPhysics();
	bool createActorPhysics();
	void releaseActorPhysics();


	void addForce(const glm::vec3& vector, const Engine::Physics::Force& forceType = Engine::Physics::Force::IMPULSE);

	inline bool visible() { return _visible; }
	inline void setVisible(const bool visible) { _visible = visible; }
	inline bool hasPhysics() { return _actorPhyscs; }
	inline double getCreateTime() { return _createTime; }

public:
	virtual void action();

public:
	static Object* _default;

public:
	static Object& defaultItem();
	static void removeDefault();
};
