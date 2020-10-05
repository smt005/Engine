
#include "Physics/Physics.h"
#include "Common/Help.h"
#include "Object/Mesh.h"

#include <memory>

#include "PxConfig.h"
#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"

// D:\CMake\CMakeProject\Source\Engine\ThirdParty\PhysX\physx\externals\physx\bin\win.x86_32.vc141.md\debug\PhysXCooking_static.lib
// D:\CMake\CMakeProject\Source\Engine\ThirdParty\PhysX\physx\externals\physx\bin\win.x86_32.vc141.md\debug\PhysXExtensions_static.lib

namespace Engine {
	using namespace physx;

	class UserErrorCallback : public PxErrorCallback
	{
	public:
		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) {
			// error processing implementation
			
			help::log("PhysX: " + std::string(message) + " file: " + std::string(file) + " line: " + std::to_string(line));
		}
	};

	static UserErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	static PxFoundation* mFoundation = nullptr;
	static PxPhysics* mPhysics = nullptr;
	static PxPvd* mPvd = nullptr;
	static bool debugVisualizing = false;
	static PxPvdTransport* transport = nullptr;
	static PxCooking* mCooking = nullptr;

	static PxVec3 gravity(0.0f, 0.0f, -9.81f);
	static PxReal mAccumulator = 0.0f;
	static PxReal mStepSize = 1.0f / 60.0f;
	static PxScene* mScene = nullptr;

	static PxMaterial* mMaterial = nullptr;

	static PxRigidDynamic* aLastActor = nullptr;

	bool initPhysics() {
		help::log("Physic: init: BEGIN");

		mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		if (!mFoundation) {
			help::log("Physic: init: PxCreateFoundation failed!");
			return false;
		}

		help::log("Physic: init: PxCreateFoundation OK!");

		bool recordMemoryAllocations = true;

		mPvd = PxCreatePvd(*mFoundation);
		transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
		debugVisualizing = mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
		if (!debugVisualizing) {			
			help::log("Physic: init: PVD FAIL");
		}
		else {
			help::log("Physic: init: PVD connect");
		}

		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), recordMemoryAllocations, mPvd);
		if (!mPhysics) {
			help::log("Physic: init: PxCreatePhysics failed!");
			return false;
		}

		const PxTolerancesScale scale;
		const PxCookingParams params(scale);
		mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, params);
		if (!mCooking) {
			help::log("Physic: init: PxCreateCooking failed!");
			return false;
		}

		help::log("Physic: init: OK");
		return true;
	}

	void releasePhysics() {
		if (mPhysics) {
			mPhysics->release();
			mPhysics = nullptr;
		}

		if (mFoundation) {
			mFoundation->release();
			mFoundation = nullptr;
		}

		if (mPvd) {
			mPvd->release();
			mPvd = nullptr;
		}

		if (transport) {
			transport->release();
			transport = nullptr;
		}

		mCooking = nullptr;
		mScene = nullptr;
		mMaterial = nullptr;
	}

	bool createScene() {
		help::log("Physic: Scene: BEGIN");

		if (!mPhysics) {
			return false;
		}

		PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
		sceneDesc.gravity = gravity;

		if (!sceneDesc.cpuDispatcher)
		{
			PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1); // Что за параметро
			if (!mCpuDispatcher) {
				help::log("Physic: Scene: PxDefaultCpuDispatcherCreate failed!");
				return false;
			}

			sceneDesc.cpuDispatcher = mCpuDispatcher; // Узнать...

			if (!sceneDesc.filterShader) {
				sceneDesc.filterShader = &PxDefaultSimulationFilterShader;
			}

			mScene = mPhysics->createScene(sceneDesc);
			if (!mScene) {
				help::log("Physic: Scene: createScene failed!");
				return false;
			}
		}

		help::log("Physic: Scene: OK");

		if (debugVisualizing) {
			mScene->getScenePvdClient()->setScenePvdFlags(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS | PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES | PxPvdSceneFlag::eTRANSMIT_CONTACTS);
		}

		return true;
	}

	bool updateScene(const float dt) {
		if (!mScene) {
			return false;
		}

		mAccumulator += dt;
		if (mAccumulator < mStepSize) {
			return false;
		}

		mAccumulator -= mStepSize;
		mScene->simulate(mStepSize);
		fetchResultsScene();
		return true;
	}

	void fetchResultsScene() {
		if (mScene) {
			mScene->fetchResults(true);
		}
	}

	ActorPhyscs* createActorConvex(Mesh* mesh, const float pos[3]) {
		if (!mesh) {
			help::log("Physic: Convex actor: mesh == nullptr");
			return nullptr;
		}

		if (!mPhysics || !mScene) {
			help::log("Physic: Convex actor: FAIL: mPhysics == nullptr || mScene == nullptr");
		}

		if (!mMaterial) {
			mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.1f); // Трение
		}
		if (!mMaterial) {
			help::log("Physic: Convex actor: createMaterial failed!");
			return nullptr;
		}

		PxRigidDynamic* aConvexActor = mPhysics->createRigidDynamic(PxTransform(pos[0], pos[1], pos[2]));

		unsigned int count = mesh->countVertex();

		PxConvexMeshDesc convexDesc;
		convexDesc.points.count = count;
		convexDesc.points.stride = sizeof(PxVec3);
		convexDesc.points.data = mesh->vertexes();
		convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

		PxDefaultMemoryOutputStream buf;
		if (!mCooking->cookConvexMesh(convexDesc, buf)) {
			help::log("Physic: Convex actor: cookConvexMesh FAIL");
			return nullptr;
		}

		PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		PxConvexMesh* convexMesh = mPhysics->createConvexMesh(input);

		PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*aConvexActor, PxConvexMeshGeometry(convexMesh), *mMaterial);


		mScene->addActor(*aConvexActor);
		return aConvexActor;
	}

	ActorPhyscs* createActorTriangle(Mesh* mesh, const float pos[3]) {
		if (!mesh) {
			help::log("Physic: Triangle actor: mesh == nullptr");
			return nullptr;
		}

		if (!mPhysics || !mScene) {
			help::log("Physic: Triangle actor: FAIL: mPhysics == nullptr || mScene == nullptr");
		}

		if (!mMaterial) {
			mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.1f); // Трение
		}
		if (!mMaterial) {
			help::log("Physic: Triangle actor: createMaterial failed!");
			return nullptr;
		}

		PxRigidStatic* aTriangleActor = mPhysics->createRigidStatic(PxTransform(pos[0], pos[1], pos[2]));

		PxTriangleMeshDesc meshDesc;

		unsigned int count = mesh->countVertex();

		meshDesc.points.count = count;
		meshDesc.points.stride = sizeof(PxVec3); // float * 3
		meshDesc.points.data = mesh->vertexes();

		unsigned int countIndex = mesh->countIndex();

		meshDesc.triangles.count = countIndex;
		meshDesc.triangles.stride = 3 * sizeof(PxU32); // unsigned int
		meshDesc.triangles.data = mesh->indexes();

		PxDefaultMemoryOutputStream writeBuffer;
		bool status = mCooking->cookTriangleMesh(meshDesc, writeBuffer);
		if (!status) {
			help::log("Physic: Triangle actor: cookTriangleMesh FAIL");
			return nullptr;
		}

		PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		PxTriangleMesh* triangleMesh = mPhysics->createTriangleMesh(readBuffer);

		if (!triangleMesh) {
			help::log("Physic: Triangle actor: triangleMesh == nullptr");
			return nullptr;
		}

		PxShape* aTriangleShape = PxRigidActorExt::createExclusiveShape(*aTriangleActor, PxTriangleMeshGeometry(triangleMesh), *mMaterial);

		if (!aTriangleShape) {
			help::log("Physic: Triangle actor: aTriangleShape == nullptr");
			return nullptr;
		}

		mScene->addActor(*aTriangleActor);
		return aTriangleActor;
	}

	ActorPhyscs* createActorBox(const float rect[3], const float pos[3]) {
		if (!mPhysics || !mScene) {
			help::log("Physic: Box actor: FAIL: mPhysics == nullptr || mScene == nullptr");
		}

		if (!mMaterial) {
			mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.1f); // Трение
		}
		if (!mMaterial) {
			help::log("Physic: Box actor: createMaterial failed!");
			return nullptr;
		}

		PxRigidDynamic* aBoxActor = mPhysics->createRigidDynamic(PxTransform(pos[0], pos[1], pos[2]));
		PxShape* aBoxShape = PxRigidActorExt::createExclusiveShape(*aBoxActor, PxBoxGeometry(rect[0]/2.0f, rect[1]/2.0f, rect[2]/2.0f), *mMaterial);

		mScene->addActor(*aBoxActor);

		aLastActor = aBoxActor;
		return aBoxActor;
	}

	Position getActorPos() {
		if (!aLastActor) {
			return Position();
		}

		PxTransform transform = aLastActor->getGlobalPose();
		PxMat44 mat44(transform);

		float mat[16];

		mat[0] = mat44.column0[0];
		mat[1] = mat44.column0[1];
		mat[2] = mat44.column0[2];
		mat[3] = 0;

		mat[4] = mat44.column1[0];
		mat[5] = mat44.column1[1];
		mat[6] = mat44.column1[2];
		mat[7] = 0;

		mat[8] = mat44.column2[0];
		mat[9] = mat44.column2[1];
		mat[10] = mat44.column2[2];
		mat[11] = 0;

		mat[12] = mat44.column3[0];
		mat[13] = mat44.column3[1];
		mat[14] = mat44.column3[2];
		mat[15] = 1;

		return mat;
	}

	Position getActorPos(ActorPhyscs* actorPhyscs) {
		if (!actorPhyscs) {
			return Position();
		}

		PxTransform transform = static_cast<PxRigidDynamic*>(actorPhyscs)->getGlobalPose();
		PxMat44 mat44(transform);

		float mat[16];

		mat[0] = mat44.column0[0];
		mat[1] = mat44.column0[1];
		mat[2] = mat44.column0[2];
		mat[3] = 0;

		mat[4] = mat44.column1[0];
		mat[5] = mat44.column1[1];
		mat[6] = mat44.column1[2];
		mat[7] = 0;

		mat[8] = mat44.column2[0];
		mat[9] = mat44.column2[1];
		mat[10] = mat44.column2[2];
		mat[11] = 0;

		mat[12] = mat44.column3[0];
		mat[13] = mat44.column3[1];
		mat[14] = mat44.column3[2];
		mat[15] = 1;

		return mat;
	}
};
