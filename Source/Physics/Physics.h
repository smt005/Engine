
#pragma once

#include "Object/Position.h"

class Mesh;

namespace Engine {
	typedef void ActorPhyscs;

	bool initPhysics();
	void releasePhysics();
	
	bool createScene();
	bool updateScene(const float dt);
	void fetchResultsScene();

	ActorPhyscs* createActorConvex(Mesh* mesh, const float pos[3]);
	ActorPhyscs* createActorTriangle(Mesh* mesh, const float pos[3]);
	ActorPhyscs* createActorBox(const float rect[3], const float pos[3]);

	Position getActorPos();
	Position getActorPos(ActorPhyscs* actorPhyscs);

};	// Engine
