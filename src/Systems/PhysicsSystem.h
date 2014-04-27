#ifndef PhysicsSystem_h__
#define PhysicsSystem_h__

#include "System.h"
#include "Components/Transform.h"
#include "Components/Physics.h"
#include "Components/BoxShape.h"
#include "Components/SphereShape.h"
#include "Components/Vehicle.h"
#include "Components/Input.h"
#include "Components/MeshShape.h"
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

#include "Physics/VehicleSetup.h"

#include <unordered_map>
namespace Systems
{

class PhysicsSystem : public System
{
public:
	PhysicsSystem(World* world);
	void RegisterComponents(ComponentFactory* cf) override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	void OnComponentCreated(std::string type, std::shared_ptr<Component> component) override;
	void OnComponentRemoved(std::string type, Component* component) override;
	void OnEntityCommit(EntityID entity) override;

private:
	double m_Accumulator;
	hkpWorld* m_PhysicsWorld;

	void SetUpPhysicsState(EntityID entity, EntityID parent);
	void TearDownPhysicsState(EntityID entity, EntityID parent);

	hkVisualDebugger* m_VisualDebugger;
	void SetupVisualDebugger(hkpPhysicsContext* worlds);
	void StepVisualDebugger();
	static void HK_CALL HavokErrorReport(const char* msg, void*);
	void SetupPhysics(hkpWorld* physicsWorld);

	std::unordered_map<EntityID, hkpRigidBody*> m_RigidBodies;
	

	std::unordered_map<EntityID, hkpVehicleInstance*> m_Vehicles;
	std::vector<EntityID> m_Wheels;

	hkpVehicleInstance* Systems::PhysicsSystem::createVehicle(VehicleSetup& vehicleSetup, hkpRigidBody* chassis);

	

	struct  ExtendedShapeData
	{
		hkpExtendedMeshShape* ExtendedMeshShape;
		std::vector<hkReal>* Vertices;
		std::vector<hkUint16>* VertexIndices;
	};
	std::unordered_map<EntityID, ExtendedShapeData > m_hkpExtendedMeshShapes;
};

}

#endif // PhysicsSystem_h__
