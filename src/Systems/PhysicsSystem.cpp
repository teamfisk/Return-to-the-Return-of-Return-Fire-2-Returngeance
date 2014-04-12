#include "PrecompiledHeader.h"
#include <Common/Base/keycode.cxx>
// We’re not using anything product specific yet. We undef these so we don’t get the usual
// product initialization for the products.
#undef HK_FEATURE_PRODUCT_AI
//#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION_2012
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#undef HK_FEATURE_PRODUCT_PHYSICS_2012
//#undef HK_FEATURE_PRODUCT_PHYSICS
// Also we’re not using any serialization/versioning so we don’t need any of these.
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
#define HK_EXCLUDE_FEATURE_RegisterReflectedClasses
#define HK_EXCLUDE_FEATURE_MemoryTracker
// This include generates an initialization function based on the products
// and the excluded features.
#include <Common/Base/Config/hkProductFeatures.cxx>
#include "PhysicsSystem.h"
#include "World.h"



Systems::PhysicsSystem::PhysicsSystem(World* world) : System(world)
{	

	hkMemorySystem::FrameInfo finfo(500 * 1024);	// Allocate 500KB of Physics solver buffer
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator, finfo);
	hkBaseSystem::init(memoryRouter, HavokErrorReport);

	{
		hkpWorldCinfo worldInfo;
		worldInfo.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		worldInfo.m_gravity = hkVector4(0.0f, -9.8f, 0.0f);
		worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY; // just fix the entity if the object falls off too far

		// You must specify the size of the broad phase - objects should not be simulated outside this region
		worldInfo.setBroadPhaseWorldSize(1000.0f);
		m_PhysicsWorld = new hkpWorld(worldInfo);
	}
	// Register all collision agents, even though only box - box will be used in this particular example.
	// It's important to register collision agents before adding any entities to the world.
	hkpAgentRegisterUtil::registerAllAgents(m_PhysicsWorld->getCollisionDispatcher());
	
	//
	// Initialize the visual debugger so we can connect remotely to the simulation
	// The context must exist beyond the use of the VDB instance, and you can make
	// whatever contexts you like for your own viewer types.
	//
	hkpPhysicsContext* context = new hkpPhysicsContext;
	hkpPhysicsContext::registerAllPhysicsProcesses(); // all the physics viewers
	context->addWorld(m_PhysicsWorld); // add the physics world so the viewers can see it
	SetupVisualDebugger(context);

	SetupPhysics(m_PhysicsWorld);
}

void Systems::PhysicsSystem::Update(double dt)
{	
	m_PhysicsWorld->stepDeltaTime(dt);

	// Step the visual debugger
	StepVisualDebugger();
}

void Systems::PhysicsSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{	

}

void Systems::PhysicsSystem::OnComponentCreated(std::string type, std::shared_ptr<Component> component)
{

}

void Systems::PhysicsSystem::OnComponentRemoved(std::string type, Component* component)
{

}

void Systems::PhysicsSystem::SetUpPhysicsState(EntityID entity, EntityID parent)
{	

}

void Systems::PhysicsSystem::TearDownPhysicsState(EntityID entity, EntityID parent)
{	

}

void Systems::PhysicsSystem::SetupVisualDebugger(hkpPhysicsContext* worlds)
{
	// Setup the visual debugger
	hkArray<hkProcessContext*> contexts;
	contexts.pushBack(worlds);

	m_VisualDebugger = new hkVisualDebugger(contexts);
	m_VisualDebugger->serve();

	// Allocate memory for internal profiling information
	// You can discard this if you do not want Havok profiling information
	hkMonitorStream& stream = hkMonitorStream::getInstance();
	stream.resize(500 * 1024);	// 500K for timer info
	stream.reset();

}

void Systems::PhysicsSystem::StepVisualDebugger()
{
	// Step the debugger
	m_VisualDebugger->step();

	// Reset internal profiling info for next frame
	hkMonitorStream::getInstance().reset();
}

void HK_CALL Systems::PhysicsSystem::HavokErrorReport(const char* msg, void*)
{
	LOG_ERROR("%s", msg);
}


void Systems::PhysicsSystem::SetupPhysics(hkpWorld* physicsWorld)
{
	// Create the floor as a fixed box
	{
		hkpRigidBodyCinfo boxInfo;
		hkVector4 boxSize(5.0f, 0.5f, 5.0f);
		hkpBoxShape* boxShape = new hkpBoxShape(boxSize);
		boxInfo.m_shape = boxShape;
		boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
		boxInfo.m_position.set(0.0f, 0.0f, 0.0f);
		boxInfo.m_restitution = 0.9f;

		hkpRigidBody* floor = new hkpRigidBody(boxInfo);
		boxShape->removeReference();

		physicsWorld->addEntity(floor);
		floor->removeReference();
	}

	// Create a moving sphere
	{
		hkReal sphereRadius = 0.5f;
		hkpConvexShape* sphereShape = new hkpSphereShape(sphereRadius);

		hkpRigidBodyCinfo sphereInfo;
		sphereInfo.m_shape = sphereShape;
		sphereInfo.m_position.set(0.0f, 5.0f, 0.0f);
		sphereInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;

		// Compute mass properties
		hkReal sphereMass = 10.0f;
		hkMassProperties sphereMassProperties;
		hkpInertiaTensorComputer::computeSphereVolumeMassProperties(sphereRadius, sphereMass, sphereMassProperties);
		sphereInfo.m_inertiaTensor = sphereMassProperties.m_inertiaTensor;
		sphereInfo.m_centerOfMass = sphereMassProperties.m_centerOfMass;
		sphereInfo.m_mass = sphereMassProperties.m_mass;

		// Create sphere RigidBody
		hkpRigidBody* sphereRigidBody = new hkpRigidBody(sphereInfo);
		sphereShape->removeReference();

		physicsWorld->addEntity(sphereRigidBody);
		g_ball = sphereRigidBody;
		sphereRigidBody->removeReference();
	}
}