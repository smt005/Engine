
#pragma once

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

	public:
		static bool init();
		static void release();

		static bool createScene();
		static void releaseScene();
		static bool updateScene(const float dt);

	private:
		static bool createActor(Object& object);
		static void updateMatrixActor(Object& object);
		static void releaseActor(Object& object);
	};
	
};	// Engine
