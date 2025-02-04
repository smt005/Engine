
#include "Physics/Physics.h"
#include <Log.h>

#if ENABLE_PHYSIC
#include <memory>
#include "PxConfig.h"
#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"
#include "Common/Help.h"
#include "Object/Object.h"
#include "Object/Model.h"
#include "Object/Mesh.h"
#include "Object/Position.h"

namespace Engine {
	using namespace physx;

	namespace physics {
		class UserErrorCallback : public PxErrorCallback
		{
		public:
			virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) {
				Log("PhysX: {} file: {} line: {}", message, file, line);
			}
		};

		physics::ActorPhyscs* createActorConvex(Mesh& mesh, const glm::mat4x4& matrix);
		physics::ActorPhyscs* createActorTriangle(Mesh& mesh, const glm::mat4x4& matrix);
	}

	// PhysX
	static physics::UserErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	static PxFoundation* pFoundation = nullptr;
	static PxPhysics* pPhysics = nullptr;
	static PxPvd* pPvd = nullptr;
	static bool debugVisualizing = false;
	static PxPvdTransport* pTransport = nullptr;
	static PxCooking* pCooking = nullptr;
	static bool physXInited = false;

	// Scene
	static PxVec3 gravity(0.0f, 0.0f, -9.81f);
	static PxReal accumulatorTime = 0.0f;
	static PxReal stepSizeTime = 1.0f / 60.0f;
	static PxScene* pScene = nullptr;

	// Actor
	static PxMaterial* pMaterial = nullptr;

	bool Physics::init() {
		if (physXInited) {
			return true;
		}

		pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		if (!pFoundation) {
			return false;
		}

		/* VISUAL DEBUG
		pPvd = PxCreatePvd(*pFoundation);
		pTransport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
		debugVisualizing = pPvd->connect(*pTransport, PxPvdInstrumentationFlag::eALL);
		if (!debugVisualizing) {
			help::log("PhysX PVD connect");
		} else {
			help::log("PhysX PVD connect FAIL");
		}*/

		pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pFoundation, PxTolerancesScale(), false, pPvd); // trackOutstandingAllocations, ��� ���� ���� ��������
		if (!pPhysics) {
			return false;
		}

		const PxTolerancesScale scale;
		const PxCookingParams params(scale);
		pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *pFoundation, params);
		if (!pCooking) {
			return false;
		}

		physXInited = true;
		Log("PhysX inited '{}'", PX_PHYSICS_VERSION);
		return true;
	}

	void Physics::release() {
		releaseScene();

		if (pPvd) { pPvd->release(); pPvd = nullptr; }
		debugVisualizing = false;
		
		if (pTransport) { pTransport->release(); pTransport = nullptr;}
		if (pCooking) { pCooking->release(); pCooking = nullptr; }
		if (pPhysics) { pPhysics->release(); pPhysics = nullptr; }
		if (pFoundation) { pFoundation->release(); pFoundation = nullptr; }

		if (pMaterial) { pMaterial->release(); pMaterial = nullptr; }

		physXInited = false;
	}


	bool Physics::createScene() {
		if (!pPhysics) {
			return false;
		}

		if (pScene) {
			return true;
		}

		PxSceneDesc sceneDesc(pPhysics->getTolerancesScale());
		sceneDesc.gravity = gravity;

		if (!sceneDesc.cpuDispatcher) {
			PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1); // ��� �� ��������
			if (!mCpuDispatcher) {
				return false;
			}

			sceneDesc.cpuDispatcher = mCpuDispatcher; // ������...

			if (!sceneDesc.filterShader) {
				sceneDesc.filterShader = &PxDefaultSimulationFilterShader;
			}

			pScene = pPhysics->createScene(sceneDesc);
			if (!pScene) {
				return false;
			}
		}

		if (debugVisualizing) {
			pScene->getScenePvdClient()->setScenePvdFlags(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS | PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES | PxPvdSceneFlag::eTRANSMIT_CONTACTS);
		}

		return true;
	}

	void Physics::releaseScene() {
		if (pScene) {
			pScene->release();
			pScene = nullptr;
		}
	}

	bool Physics::updateScene(const float dt) {
		if (!pScene) {
			return false;
		}

		// TODO:
		for (int i = 0; i < 10; ++i) {
			pScene->simulate(0.001f);
			pScene->fetchResults(true);
		}

		return true;
	}
	
	void Physics::GetGravity(const glm::vec3& vector) {
		gravity.x = vector.x;
		gravity.y = vector.y;
		gravity.z = vector.z;
	}

	glm::vec3 Physics::Raycast(const glm::vec3& pos, const glm::vec3& vector)
	{
		glm::vec3 hitPos;
		if (!pScene) {
			return hitPos;
		}

		const PxVec3 origin(pos.x, pos.y, pos.z);
		const PxVec3 unitDir(vector.x, vector.y, vector.z);
		const PxReal maxDistance = 10000.0f;
		PxRaycastBuffer hit;
		PxQueryFilterData filterData(PxQueryFlag::eSTATIC);

		if (pScene->raycast(origin, unitDir, maxDistance, hit, PxHitFlag::eDEFAULT, filterData)) {
			const auto& position = hit.block.position;
			hitPos.x = position.x;
			hitPos.y = position.y;
			hitPos.z = position.z;
		}

		return hitPos;
	}

	glm::vec3 Physics::GetLinearVelocity(Object& object) {
		if (object._typePhysics == Physics::Type::CONVEX) {
			if (!object._actorPhyscs) {
				return { 0.f, 0.f, 0.f };
			}

			PxVec3 pxVec3 = static_cast<PxRigidDynamic*>(object._actorPhyscs)->getLinearVelocity();
			return { pxVec3.x, pxVec3.y, pxVec3.z };
		}

		return { 0.f, 0.f, 0.f };
	}

	glm::vec3 Physics::GetAngularVelocity(Object& object) {
		if (object._typePhysics == Physics::Type::CONVEX) {
			if (!object._actorPhyscs) {
				return { 0.f, 0.f, 0.f };
			}

			PxVec3 pxVec3 = static_cast<PxRigidDynamic*>(object._actorPhyscs)->getAngularVelocity();
			return { pxVec3.x, pxVec3.y, pxVec3.z };
		}

		return { 0.f, 0.f, 0.f };
	}

	void Physics::SetLinearVelocity(Object& object, const glm::vec3& velocity) {
		if (object._typePhysics == Physics::Type::CONVEX && object._actorPhyscs) {
			PxVec3 pxVec3(velocity.x, velocity.y, velocity.z);
			static_cast<PxRigidDynamic*>(object._actorPhyscs)->setLinearVelocity(pxVec3);
		}
	}

	void Physics::SetAngularVelocity(Object& object, const glm::vec3& velocity)
	{
		if (object._typePhysics == Physics::Type::CONVEX && object._actorPhyscs) {
			PxVec3 pxVec3(velocity.x, velocity.y, velocity.z);
			static_cast<PxRigidDynamic*>(object._actorPhyscs)->setAngularVelocity(pxVec3);
		}
	}

	bool Physics::createActor(Object& object) {
		physics::ActorPhyscs* actorPhyscs = nullptr;

		if (object._typePhysics == Physics::Type::CONVEX) {
			actorPhyscs = physics::createActorConvex(object.getModel().getMesh(), object.getMatrix());
		}
		else if (object._typePhysics == Physics::Type::TRIANGLE) {
			actorPhyscs = physics::createActorTriangle(object.getModel().getMesh(), object.getMatrix());
		}

		if (actorPhyscs) {
			object._actorPhyscs = actorPhyscs;
		}
		else {
			object._typePhysics = Physics::Type::NONE;
			return false;
		}

		return true;
	}

	void Physics::updateMatrixActor(Object& object) {
		if (object._typePhysics == Physics::Type::CONVEX) {
			if (!object._actorPhyscs) {
				return;
			}

			PxTransform transform = static_cast<PxRigidDynamic*>(object._actorPhyscs)->getGlobalPose();
			PxMat44 mat44(transform);
			glm::mat4x4& matrix = object._matrix;

			matrix[0][0] = mat44.column0[0];
			matrix[0][1] = mat44.column0[1];
			matrix[0][2] = mat44.column0[2];
			matrix[0][3] = 0;

			matrix[1][0] = mat44.column1[0];
			matrix[1][1] = mat44.column1[1];
			matrix[1][2] = mat44.column1[2];
			matrix[1][3] = 0;

			matrix[2][0] = mat44.column2[0];
			matrix[2][1] = mat44.column2[1];
			matrix[2][2] = mat44.column2[2];
			matrix[2][3] = 0;

			matrix[3][0] = mat44.column3[0];
			matrix[3][1] = mat44.column3[1];
			matrix[3][2] = mat44.column3[2];
			matrix[3][3] = 1;
		}
	}

	void Physics::releaseActor(Object& object) {
		if (object._typePhysics != Physics::Type::NONE && pScene) {
			if (PxActor* actor = (PxActor*)object._actorPhyscs) {
				actor->release();
			}
		}

		object._actorPhyscs = nullptr;
	}
	
	void Physics::addForceToActor(const Object& object, const glm::vec3& vector, const Engine::Physics::Force& forceType) {
		if (object._typePhysics == Physics::Type::CONVEX) {
			if (PxRigidDynamic* pConvexActor = (PxRigidDynamic*)object._actorPhyscs) {
				PxVec3 force = PxVec3(vector.x, vector.y, vector.z);

				switch (forceType) {
					case Engine::Physics::Force::ACCELERATION: pConvexActor->addForce(force, PxForceMode::Enum::eACCELERATION, true); break;
					case Engine::Physics::Force::FORCE: pConvexActor->addForce(force, PxForceMode::Enum::eFORCE, true); break;
					case Engine::Physics::Force::IMPULSE: pConvexActor->addForce(force, PxForceMode::Enum::eIMPULSE, true); break;
					case Engine::Physics::Force::VELOCITY_CHANGE: pConvexActor->addForce(force, PxForceMode::Enum::eVELOCITY_CHANGE, true); break;
					default: pConvexActor->addForce(force, PxForceMode::Enum::eIMPULSE, true);
				}
			}
		}
	}

	void Physics::addTorqueToActor(const Object& object, const glm::vec3& vector, const Engine::Physics::Force& forceType) {
		if (object._typePhysics == Physics::Type::CONVEX) {
			if (PxRigidDynamic* pConvexActor = (PxRigidDynamic*)object._actorPhyscs) {
				PxVec3 force = PxVec3(vector.x, vector.y, vector.z);

				switch (forceType) {
				case Engine::Physics::Force::ACCELERATION: pConvexActor->addTorque(force, PxForceMode::Enum::eACCELERATION, true); break;
				case Engine::Physics::Force::FORCE: pConvexActor->addTorque(force, PxForceMode::Enum::eFORCE, true); break;
				case Engine::Physics::Force::IMPULSE: pConvexActor->addTorque(force, PxForceMode::Enum::eIMPULSE, true); break;
				case Engine::Physics::Force::VELOCITY_CHANGE: pConvexActor->addTorque(force, PxForceMode::Enum::eVELOCITY_CHANGE, true); break;
				default: pConvexActor->addTorque(force, PxForceMode::Enum::eIMPULSE, true);
				}
			}
		}
	}

	float Physics::getMassActor(Object& object) {
		if (object._typePhysics != Physics::Type::CONVEX) {
			return 0;
		}

		if (!object._actorPhyscs) {
			return 0;
		}

		PxRigidDynamic* pConvexActor = (PxRigidDynamic*)object._actorPhyscs;
		if (!pConvexActor) {
			return 0;
		}

		return pConvexActor->getMass();
	}

	void Physics::setMassToActor(Object& object, const float mass) {
		if (object._typePhysics == Physics::Type::CONVEX) {
			if (!object._actorPhyscs) {
				return;
			}

			PxRigidDynamic* pConvexActor = (PxRigidDynamic*)object._actorPhyscs;
			if (!pConvexActor) {
				return;
			}

			pConvexActor->setMass(mass);
		}
	}

	void Physics::setPositionToActor(Object& object, const glm::vec3& pos) {
		if (object._typePhysics == Physics::Type::CONVEX) {
			if (!object._actorPhyscs) {
				return;
			}

			PxRigidDynamic* pConvexActor = (PxRigidDynamic*)object._actorPhyscs;
			if (!pConvexActor) {
				return;
			}

			PxTransform transform = static_cast<PxRigidDynamic*>(object._actorPhyscs)->getGlobalPose();
			transform.p[0] = pos[0];
			transform.p[1] = pos[1];
			transform.p[2] = pos[2];

			pConvexActor->setGlobalPose(transform);
		}
	}

	void Physics::SetMatrixToActor(Object& object, const glm::mat4x4& matrix) {
		if (object._typePhysics != Physics::Type::NONE && object._actorPhyscs) {
			glm::vec3 position(matrix[3][0], matrix[3][1], matrix[3][2]);
			PxVec3 pxPosition(position.x, position.y, position.z);

			glm::mat3 rotationMatrix(matrix);
			glm::quat rotation = glm::quat_cast(rotationMatrix);
			PxQuat pxRotation(rotation.x, rotation.y, rotation.z, rotation.w);

			PxTransform pxTransform(pxPosition, pxRotation);
			static_cast<PxRigidActor*>(object._actorPhyscs)->setGlobalPose(pxTransform);
		}
	}

	//...
	physics::ActorPhyscs* physics::createActorConvex(Mesh& mesh, const glm::mat4x4& matrix) {
		if (mesh.countVertex() == 0) {
			return nullptr;
		}

		if (!pPhysics || !pScene || !pCooking) {
			return nullptr;
		}

		if (!pMaterial) {
			pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.1f); // ������
		}
		if (!pMaterial) {
			return nullptr;
		}

		PxConvexMeshDesc convexDesc;
		convexDesc.points.count = mesh.countVertex();
		convexDesc.points.stride = sizeof(PxVec3);
		convexDesc.points.data = mesh.vertexes();
		convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

		PxDefaultMemoryOutputStream buf;
		if (!pCooking->cookConvexMesh(convexDesc, buf)) {
			return nullptr;
		}

		PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		PxConvexMesh* convexMesh = pPhysics->createConvexMesh(input);

		PxMat44 mat44(	PxVec4(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3]),
						PxVec4(matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3]),
						PxVec4(matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3]),
						PxVec4(matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]));
		PxRigidDynamic* pConvexActor = pPhysics->createRigidDynamic(PxTransform(mat44));
		pConvexActor->setAngularDamping(0.5f);
		PxShape* pConvexShape = PxRigidActorExt::createExclusiveShape(*pConvexActor, PxConvexMeshGeometry(convexMesh), *pMaterial);

		if (!pConvexShape) {
			return nullptr;
		}

		PxReal contactOffset = pConvexShape->getContactOffset();
		contactOffset = 1;
		pConvexShape->setContactOffset(contactOffset);

		/*PxReal restOffset = pConvexShape->getRestOffset();
		restOffset = 1;
		pConvexShape->setRestOffset(restOffset);*/

		pScene->addActor(*pConvexActor);
		return pConvexActor;
	}

	physics::ActorPhyscs* physics::createActorTriangle(Mesh& mesh, const glm::mat4x4& matrix) {
		if (!pPhysics || !pScene || !pCooking) {
			return nullptr;
		}

		if (!pMaterial) {
			pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.1f); // ������
		}
		if (!pMaterial) {
			return nullptr;
		}

		PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = mesh.countVertex();
		meshDesc.points.stride = sizeof(PxVec3); // float * 3
		meshDesc.points.data = mesh.vertexes();
		meshDesc.triangles.count = mesh.countIndex();
		meshDesc.triangles.stride = 3 * sizeof(PxU32); // unsigned int
		meshDesc.triangles.data = mesh.indexes();

		PxDefaultMemoryOutputStream writeBuffer;
		bool status = pCooking->cookTriangleMesh(meshDesc, writeBuffer);
		if (!status) {
			return nullptr;
		}

		PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		PxTriangleMesh* triangleMesh = pPhysics->createTriangleMesh(readBuffer);

		if (!triangleMesh) {
			return nullptr;
		}

		PxMat44 mat44(	PxVec4(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3]),
						PxVec4(matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3]),
						PxVec4(matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3]),
						PxVec4(matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]));
		PxRigidStatic* pTriangleActor = pPhysics->createRigidStatic(PxTransform(mat44));

		PxShape* pTriangleShape = PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry(triangleMesh), *pMaterial);

		if (!pTriangleShape) {
			return nullptr;
		}

		PxReal contactOffset = pTriangleShape->getContactOffset();
		contactOffset = 1;
		pTriangleShape->setContactOffset(contactOffset);

		/*PxReal restOffset = pTriangleShape->getRestOffset();
		restOffset = 1;
		pTriangleShape->setRestOffset(restOffset);*/

		pScene->addActor(*pTriangleActor);
		return pTriangleActor;
	}
};

#else

namespace Engine {
	bool Physics::init() { return false; }
	void Physics::release() {}
	bool Physics::createScene() { return false; }
	void Physics::releaseScene() {}
	bool Physics::updateScene(const float dt) { return false; }
	void Physics::GetGravity(const glm::vec3& vector) {}
	glm::vec3 Physics::GetLinearVelocity(Object& object) { return glm::vec3(0.f, 0.f, 0.f); }
	void Physics::SetLinearVelocity(Object& object, const glm::vec3& velocity) {}
	bool Physics::createActor(Object& object) { return false; }
	void Physics::updateMatrixActor(Object& object) {}
	void Physics::releaseActor(Object& object) {}
	void Physics::setMassToActor(Object& object, const float mass) {}
	void Physics::setPositionToActor(Object& object, const glm::vec3& pos) {}
	void Physics::addForceToActor(const Object& object, const glm::vec3& vector, const Engine::Physics::Force& forceType) {}
}

#endif // DENABLE_PHYSIC
