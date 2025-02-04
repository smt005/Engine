
#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

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
		/// <summary>
		/// 
		/// </summary>
		enum class FilterFlag {
			STATIC,
			DYNAMIC,
			ePREFILTER,
			POSTFILTER,
			MESH_MULTIPLE,
			BACKFACE
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
		static glm::vec3 Raycast(const glm::vec3& pos, const glm::vec3& vector);

		static glm::vec3 GetLinearVelocity(Object& object);
		static glm::vec3 GetAngularVelocity(Object& object);
		static void SetLinearVelocity(Object& object, const glm::vec3& velocity);
		static void SetAngularVelocity(Object& object, const glm::vec3& velocity);

		static bool createActor(Object& object);
		static void updateMatrixActor(Object& object);
		static void releaseActor(Object& object);

		static float getMassActor(Object& object);

		static void setMassToActor(Object& object, const float mass);
		static void setPositionToActor(Object& object, const glm::vec3& pos);
		static void SetMatrixToActor(Object& object, const glm::mat4x4& matrix);

		static void addForceToActor(const Object& object, const glm::vec3& vector, const Engine::Physics::Force& forceType);
		static void addTorqueToActor(const Object& object, const glm::vec3& vector, const Engine::Physics::Force& forceType);
	};
	
};	// Engine
