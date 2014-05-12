#ifndef PhysicsSystem_h__
#define PhysicsSystem_h__

#include "System.h"
#include "Systems/TransformSystem.h"
#include "Components/Transform.h"
#include "Components/Physics.h"
#include "Components/BoxShape.h"
#include "Components/SphereShape.h"
#include "Components/Vehicle.h"
#include "Components/Input.h"
#include "Components/MeshShape.h"
#include "Components/HingeConstraint.h"
#include "Components/WheelPair.h"
#include "Events/TankSteer.h"
#include "OBJ.h"

// Math and base include
#include <Common/Base/hkBase.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Config/hkConfigVersion.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Container/String/hkStringBuf.h>

// Dynamics includes
#include <Physics2012/Collide/hkpCollide.h>
#include <Physics2012/Collide/Agent/ConvexAgent/SphereBox/hkpSphereBoxAgent.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics2012/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>

#include <Physics2012/Dynamics/World/hkpWorld.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

// Visual Debugger includes
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics2012/Utilities/VisualDebugger/hkpPhysicsContext.h>

#include <Physics2012/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics2012/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics2012/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/DebugUtil/MultiThreadCheck/hkMultiThreadCheck.h>
#include <Physics/Constraint/Data/Hinge/hkpHingeConstraintData.h>
#include <Physics/Constraint/Data/LimitedHinge/hkpLimitedHingeConstraintData.h>

#include <Physics2012/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics2012/Internal/Collide/StaticCompound/hkpStaticCompoundShape.h>
#include <Physics2012/Collide/Util/ShapeShrinker/hkpShapeShrinker.h>
#include <Physics2012/Collide/Shape/Misc/Bv/hkpBvShape.h>

#include "Physics/VehicleSetup.h"

#include <unordered_map>
namespace Systems
{

class PhysicsSystem : public System
{
public:
	PhysicsSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
		: System(world, eventBroker) { }

	void RegisterComponents(ComponentFactory* cf) override;
	void Initialize() override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	void OnComponentCreated(std::string type, std::shared_ptr<Component> component) override;
	void OnComponentRemoved(std::string type, Component* component) override;
	void OnEntityCommit(EntityID entity) override;

private:
	double m_Accumulator;
	hkpWorld* m_PhysicsWorld;

	// Events
	EventRelay<Events::TankSteer> m_ETankSteer;
	bool OnTankSteer(const Events::TankSteer &event);

	void SetUpPhysicsState(EntityID entity, EntityID parent);
	void TearDownPhysicsState(EntityID entity, EntityID parent);

	hkVisualDebugger* m_VisualDebugger;
	void SetupVisualDebugger(hkpPhysicsContext* worlds);
	void StepVisualDebugger();
	static void HK_CALL HavokErrorReport(const char* msg, void*);
	void SetupPhysics(hkpWorld* physicsWorld);
	
	// Converterfunctions
	glm::vec3 ConvertPosition(const hkVector4 &hkPosition);
	const hkVector4& ConvertPosition(glm::vec3 glmPosition);

	glm::quat ConvertRotation(const hkQuaternion &hkRotation);
	const hkQuaternion& ConvertRotation(glm::quat glmRotation);

	glm::vec3 ConvertScale(const hkVector4 &hkScale);
	const hkVector4&ConvertScale(glm::vec3 glmScale);

	std::unordered_map<EntityID, hkpRigidBody*> m_RigidBodies;
	
	hkJobThreadPool* m_ThreadPool;
	hkJobQueue* m_JobQueue;
	int m_TotalNumThreadsUsed;
	hkpPhysicsContext* m_Context;

	std::unordered_map<EntityID, hkpVehicleInstance*> m_Vehicles;
	std::vector<EntityID> m_Wheels;

	hkpVehicleInstance* Systems::PhysicsSystem::createVehicle(VehicleSetup& vehicleSetup, hkpRigidBody* chassis);


	struct ShapeArrayData
	{
		ShapeArrayData(EntityID entity, hkpShape* shape)
		{
			Entity = entity;
			Shape = shape;
		}
		EntityID Entity;
		hkpShape* Shape;
	};
	std::unordered_map<EntityID, std::list<ShapeArrayData>> m_Shapes;
	std::unordered_map<EntityID, hkpListShape*> m_ListShapes;



	struct  ExtendedShapeData
	{
		hkpExtendedMeshShape* ExtendedMeshShape;
		std::vector<hkReal>* Vertices;
		std::vector<hkUint16>* VertexIndices;
		hkpMoppCode* Code;
		hkpMoppBvTreeShape* MoppShape;
	};
	std::unordered_map<EntityID, ExtendedShapeData > m_ExtendedMeshShapes;
};

}

#endif // PhysicsSystem_h__
