
#pragma once

#include "glm/vec3.hpp"

class Position;
class Object;

namespace Engine {
	namespace physics {
		typedef void ActorPhyscs;
	};

	class Physics {
		friend Object;

	public:
		enum class Type
		{
			NONE,
			CONVEX,
			TRIANGLE
		};

		enum class Force
		{
			ACCELERATION,
			FORCE,
			IMPULSE,
			VELOCITY_CHANGE
		};

	public:
		static bool init();
		static void release();

		static bool createScene();
		static void releaseScene();
		static bool updateScene(const float dt);
		static void GetGravity(const glm::vec3& vector);

		static glm::vec3 GetLinearVelocity(Object& object);
		static void SetLinearVelocity(Object& object, const glm::vec3& velocity);

	private:
		static bool createActor(Object& object);
		static void updateMatrixActor(Object& object);
		static void releaseActor(Object& object);

		static void setMassToActor(Object& object, const float mass);
		static void setPositionToActor(Object& object, const glm::vec3& pos);

		static void addForceToActor(const Object& object, const glm::vec3& vector, const Engine::Physics::Force& forceType);
	};
	
};	// Engine
