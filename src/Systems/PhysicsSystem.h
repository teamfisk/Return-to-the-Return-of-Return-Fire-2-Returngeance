#ifndef PhysicsSystem_h__
#define PhysicsSystem_h__





#include "System.h"
#include "Components/Transform.h"
#include "Components/Physics.h"
#include "Components/Sphere.h"
#include "Components/Box.h"

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

};

}

#endif // PhysicsSystem_h__
