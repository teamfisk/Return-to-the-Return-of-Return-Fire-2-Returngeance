#include "PrecompiledHeader.h"

// We’re not using anything product specific yet. We undef these so we don’t get the usual
// product initialization for the products.
#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
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
#define HK_CLASSES_FILE "Common/Serialize/classlist/hkClasses.h"
#include "Common/Serialize/Util/hkBuiltinTypeRegistry.cxx"
#define HK_COMPAT_FILE "Common/Compat/hkCompatVersions.h"
// This include generates an initialization function based on the products
// and the excluded features.
#include <Common/Base/keycode.cxx>
#include <Common/Base/Config/hkProductFeatures.cxx>
#include "PhysicsSystem.h"
#include "World.h"

void Systems::PhysicsSystem::Initialize()
{
	

	m_Accumulator = 0;

	// Events
	EVENT_SUBSCRIBE_MEMBER(m_ETankSteer, &Systems::PhysicsSystem::OnTankSteer);
	EVENT_SUBSCRIBE_MEMBER(m_EJeepSteer, &Systems::PhysicsSystem::OnJeepSteer);
	EVENT_SUBSCRIBE_MEMBER(m_ESetVelocity, &Systems::PhysicsSystem::OnSetVelocity);
	EVENT_SUBSCRIBE_MEMBER(m_EApplyForce, &Systems::PhysicsSystem::OnApplyForce);
	EVENT_SUBSCRIBE_MEMBER(m_EApplyPointImpulse, &Systems::PhysicsSystem::OnApplyPointImpulse);
	EVENT_SUBSCRIBE_MEMBER(m_EEnableCollisions, &Systems::PhysicsSystem::OnEnableCollisions);
	EVENT_SUBSCRIBE_MEMBER(m_EDisableCollisions, &Systems::PhysicsSystem::OnDisableCollisions);
	EVENT_SUBSCRIBE_MEMBER(m_EDead, &Systems::PhysicsSystem::OnDead);

	hkMemorySystem::FrameInfo finfo(10000 * 1024);	// Allocate 10MB of Physics solver buffer
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator, finfo);
	hkBaseSystem::init(memoryRouter, HavokErrorReport);


	// Get the number of physical threads available on the system
	hkHardwareInfo hwInfo;
	hkGetHardwareInfo(hwInfo);
	m_TotalNumThreadsUsed = hwInfo.m_numThreads;

	// We use one less than this for our thread pool, because we must also use this thread for our simulation
	hkCpuJobThreadPoolCinfo threadPoolCinfo;
	threadPoolCinfo.m_numThreads = m_TotalNumThreadsUsed - 1;

	// This line enables timers collection, by allocating 200 Kb per thread.  If you leave this at its default (0),
	// timer collection will not be enabled.
	threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
	m_ThreadPool = new hkCpuJobThreadPool(threadPoolCinfo);

	hkJobQueueCinfo info;
	info.m_jobQueueHwSetup.m_numCpuThreads = m_TotalNumThreadsUsed;
	m_JobQueue = new hkJobQueue(info);

	//
	// Enable monitors for this thread.
	//

	// Monitors have been enabled for thread pool threads already (see above comment).
	hkMonitorStream::getInstance().resize(200000);

	{
		hkpWorldCinfo worldInfo;

		// Set the simulation type of the world to multi-threaded.
		worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;

		worldInfo.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		worldInfo.m_gravity = hkVector4(0.0f, -9.82f, 0.0f);
		worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY;

		// You must specify the size of the broad phase - objects should not be simulated outside this region
		worldInfo.setBroadPhaseWorldSize(1000.0f);
		m_PhysicsWorld = new hkpWorld(worldInfo);

		// When the simulation type is SIMULATION_TYPE_MULTITHREADED, in the debug build, the sdk performs checks
		// to make sure only one thread is modifying the world at once to prevent multithreaded bugs. Each thread
		// must call markForRead / markForWrite before it modifies the world to enable these checks.
		m_PhysicsWorld->markForWrite();

		// Register all collision agents, even though only box - box will be used in this particular example.
		// It's important to register collision agents before adding any entities to the world.
		hkpAgentRegisterUtil::registerAllAgents(m_PhysicsWorld->getCollisionDispatcher());

		// We need to register all modules we will be running multi-threaded with the job queue
		m_PhysicsWorld->registerWithJobQueue(m_JobQueue);


		//
		// Initialize the visual debugger so we can connect remotely to the simulation
		// The context must exist beyond the use of the VDB instance, and you can make
		// whatever contexts you like for your own viewer types.
		//
		m_Context = new hkpPhysicsContext;
		hkpPhysicsContext::registerAllPhysicsProcesses(); // all the physics viewers
		m_Context->addWorld(m_PhysicsWorld); // add the physics world so the viewers can see it
		SetupVisualDebugger(m_Context);
		m_CollisionFilter = new hkpGroupFilter();
		m_PhysicsWorld->setCollisionFilter( m_CollisionFilter );
		m_PhysicsWorld->unmarkForWrite();

		m_collisionResolution = new MyCollisionResolution(this);
	}
	
	

	/*{
		Events::DisableCollisions e;
		e.Layer1 = VEHICLE1_LAYER;
		e.Layer2 = EXPLOSION_LAYER;
		EventBroker->Publish(e);
	}
	{
		Events::DisableCollisions e;
		e.Layer1 = VEHICLE2_LAYER;
		e.Layer2 = EXPLOSION_LAYER;
		EventBroker->Publish(e);
	}*/

}

void Systems::PhysicsSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::Physics>([]() { return new Components::Physics(); });
	cf->Register<Components::BoxShape>([]() { return new Components::BoxShape(); });
	cf->Register<Components::SphereShape>([]() { return new Components::SphereShape(); });
	cf->Register<Components::Vehicle>([]() { return new Components::Vehicle(); });
	cf->Register<Components::Wheel>([]() { return new Components::Wheel(); });
	cf->Register<Components::MeshShape>([]() { return new Components::MeshShape(); });
	cf->Register<Components::HingeConstraint>([]() { return new Components::HingeConstraint(); });
	cf->Register<Components::WheelPair>([]() { return new Components::WheelPair(); });
	cf->Register<Components::TankShell>([]() { return new Components::TankShell(); });
}

void Systems::PhysicsSystem::Update(double dt)
{	


	for (auto pair : *m_World->GetEntities())
	{
		EntityID entity = pair.first;
		EntityID parent = pair.second;

		auto templateComponent = m_World->GetComponent<Components::Template>(entity);
		if(templateComponent)
			continue;

		if (m_RigidBodies.find(entity) == m_RigidBodies.end())
			continue;

		auto transformComponent = m_World->GetComponent<Components::Transform>(entity);
		if (!transformComponent)
			continue;

		
		hkVector4 position;
		hkQuaternion rotation;
		hkVector4 velocity;

		if (parent)
		{
			auto absoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(entity);
			position = GLMVEC3_TO_HKVECTOR4(absoluteTransform.Position);
			rotation = GLMQUAT_TO_HKQUATERNION(absoluteTransform.Orientation);
			velocity = GLMVEC3_TO_HKVECTOR4(absoluteTransform.Velocity);
		}
		else
		{
			position = GLMVEC3_TO_HKVECTOR4(transformComponent->Position);
			rotation = GLMQUAT_TO_HKQUATERNION(transformComponent->Orientation);
			velocity = GLMVEC3_TO_HKVECTOR4(transformComponent->Velocity);
		}
		
		/*auto absoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(entity);
		position = GLMVEC3_TO_HKVECTOR4(absoluteTransform.Position);
		rotation = GLMQUAT_TO_HKQUATERNION(absoluteTransform.Orientation);
		velocity = GLMVEC3_TO_HKVECTOR4(absoluteTransform.Velocity);*/
		
		m_PhysicsWorld->markForWrite();
		m_RigidBodies[entity]->setPositionAndRotation(position, rotation);

		if(m_RigidBodies[entity]->getMotionType() != hkpMotion::MOTION_FIXED)
		{
			m_RigidBodies[entity]->setLinearVelocity(velocity);
		}
		m_PhysicsWorld->unmarkForWrite();

		
/*

		glm::vec3 pos1 = HKVECTOR4_TO_GLMVEC3(m_RigidBodies[m_World->m_Terrain]->getPosition());
		pos1 += glm::vec3(1, 1, 1)*(float)dt;
		hkVector4 pos = GLMVEC3_TO_HKVECTOR4(pos1);

		m_PhysicsWorld->markForWrite();
		m_RigidBodies[m_World->m_Terrain]->setPositionAndRotation(pos, GLMQUAT_TO_HKQUATERNION(glm::quat()));
		m_PhysicsWorld->unmarkForWrite();*/
	}

	static const double timestep = 1 / 60.0;
	m_Accumulator += dt;
	while (m_Accumulator >= timestep)
	{
		m_PhysicsWorld->stepMultithreaded(m_JobQueue, m_ThreadPool, timestep);
		//m_PhysicsWorld->stepDeltaTime(timestep);

		m_Accumulator -= timestep;

		m_Context->syncTimers(m_ThreadPool);
		// Step the visual debugger
		StepVisualDebugger();

		// Clear accumulated timer data in this thread and all slave threads
		hkMonitorStream::getInstance().reset();
		m_ThreadPool->clearTimerData();
	}	
}

void Systems::PhysicsSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{	
	auto templateComponent = m_World->GetComponent<Components::Template>(entity);
	if(templateComponent)
		return;

	auto transformComponent = m_World->GetComponent<Components::Transform>(entity);
	if (!transformComponent)
		return;
	
	auto wheelComponent = m_World->GetComponent<Components::Wheel>(entity);
	if (wheelComponent)
	{
		EntityID car = m_World->GetEntityParent(entity);
		if(m_Vehicles.find(car) != m_Vehicles.end())
		{
			m_PhysicsWorld->markForWrite();
			
			/*auto player = m_World->GetComponent<Components::Player>(car);
			if(player)
			{
				if(player->ID == 1)
				{
					LOG_INFO("Speed: %f, Gear: %i, RPM: %f", m_Vehicles[car]->calcKMPH(), m_Vehicles[car]->m_currentGear,  m_Vehicles[car]->m_rpm);
					
				}
			}*/
			auto vehicleComponent = m_World->GetComponent<Components::Vehicle>(car);
			vehicleComponent->currentRPM = m_Vehicles[car]->calcRPM();
			
			
			m_Vehicles[car]->getChassis()->activate();

			hkVector4 hardPoint = m_Vehicles[car]->m_suspension->m_wheelParams[wheelComponent->ID].m_hardpointChassisSpace;
			hkVector4 suspensionDirection =  m_Vehicles[car]->m_suspension->m_wheelParams[wheelComponent->ID].m_directionChassisSpace;
			hkReal suspensionLength = m_Vehicles[car]->m_wheelsInfo[wheelComponent->ID].m_currentSuspensionLength;
			glm::vec3 position = glm::vec3(hardPoint(0) + (suspensionDirection(0) * suspensionLength), hardPoint(1) + (suspensionDirection(1) * suspensionLength), hardPoint(2) + (suspensionDirection(2) * suspensionLength));
			transformComponent->Position = position;
			
			hkQuaternion steeringOrientation = m_Vehicles[car]->m_wheelsInfo[wheelComponent->ID].m_steeringOrientationChassisSpace;
			hkReal spinAngle = -m_Vehicles[car]->m_wheelsInfo[wheelComponent->ID].m_spinAngle;
			glm::quat orientation = HKQUATERNION_TO_GLMQUAT(steeringOrientation) * glm::angleAxis<float>(spinAngle, glm::vec3(1, 0, 0));
			transformComponent->Orientation = orientation * wheelComponent->OriginalOrientation;
			m_PhysicsWorld->unmarkForWrite();
		}
	}
	else if(m_RigidBodies.find(entity) != m_RigidBodies.end())
	{
		auto transformComponentParent = m_World->GetComponent<Components::Transform>(parent);
		transformComponent->Position = HKVECTOR4_TO_GLMVEC3(m_RigidBodies[entity]->getPosition());
		transformComponent->Orientation = HKQUATERNION_TO_GLMQUAT(m_RigidBodies[entity]->getRotation());
		transformComponent->Velocity = HKVECTOR4_TO_GLMVEC3(m_RigidBodies[entity]->getLinearVelocity());

		if (transformComponentParent)
		{
			auto absoluteTransformParent = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(parent);
			transformComponent->Position -= absoluteTransformParent.Position;
			transformComponent->Position = transformComponent->Position * absoluteTransformParent.Orientation;
			transformComponent->Orientation = glm::inverse(absoluteTransformParent.Orientation) * transformComponent->Orientation;
		}
	}

	
}

void Systems::PhysicsSystem::OnEntityCommit( EntityID entity )
{
	auto tempalteComponent = m_World->GetComponent<Components::Template>(entity);
	if(tempalteComponent)
		return;

	auto transformComponent = m_World->GetComponent<Components::Transform>(entity);
	if (!transformComponent)
		return;

	auto wheelComponent = m_World->GetComponent<Components::Wheel>(entity);
	if (wheelComponent)
	{
		wheelComponent->ID = m_Wheels.size();
		wheelComponent->OriginalOrientation = transformComponent->Orientation;
		m_Wheels.push_back(entity);
	}

	EntityID entityParent = m_World->GetEntityParent(entity);

	auto sphereComponent = m_World->GetComponent<Components::SphereShape>(entity);
	auto boxComponent = m_World->GetComponent<Components::BoxShape>(entity);
	auto meshShapeComponent = m_World->GetComponent<Components::MeshShape >(entity);
	
	if(entityParent == 0 && (sphereComponent || boxComponent || meshShapeComponent))
	{
		LOG_ERROR("Entity: %i, Only the children can have a shapeComponent", entity);
		return;
	}

	auto physicsComponent = m_World->GetComponent<Components::Physics>(entity);
	if (physicsComponent && m_Shapes[entity].size() > 0)
	{
		if(entityParent != 0 && physicsComponent->MotionType == Components::Physics::MotionTypeEnum::Dynamic)
		{
			LOG_ERROR("Entity: %i, Only the baseparent can have a dynamic PhysicsComponent", entity);
			return;
		}

		hkpShape* shape;	
		if(physicsComponent->MotionType == Components::Physics::MotionTypeEnum::Dynamic)
		{
			hkArray<hkpShape*> shapeArray;
			for (auto &shapeData : m_Shapes[entity])
			{
				auto childTransformComponent = m_World->GetComponent<Components::Transform>(shapeData.Entity);
				hkpShape* shape;

				if(shapeData.ConvexShape != nullptr)
				{
					hkQsTransform transform( GLMVEC3_TO_HKVECTOR4(childTransformComponent->Position), GLMQUAT_TO_HKQUATERNION(childTransformComponent->Orientation), GLMVEC3_TO_HKVECTOR4(childTransformComponent->Scale));
					hkpConvexTransformShape* transformedBoxShape = new hkpConvexTransformShape( shapeData.ConvexShape, transform );
					shapeArray.pushBack(transformedBoxShape);
					
				}
				
				if(shapeData.Shape != nullptr)
				{
					shapeArray.pushBack(shapeData.Shape);
					
				}

			}

			// Create a hkpListShape* of all the childEntities collected in m_ShapeArrays
			hkpListShape* listShape = new hkpListShape(shapeArray.begin(), shapeArray.getSize(), hkpShapeContainer::REFERENCE_POLICY_INCREMENT);
			// Save the listShape for further use
			m_ListShapes[entity] = listShape;
			hkMassProperties massProperties;
			hkpBoxShape* box = new hkpBoxShape(listShape->m_aabbHalfExtents, 0.0f);
			shape = new hkpBvShape(listShape, box);
			hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, physicsComponent->Mass, massProperties);
			
			
			for (auto &shapeData : m_Shapes[entity])
			{
				if(shapeData.ConvexShape != nullptr)
				{
					shapeData.ConvexShape->removeReference();
				}
				if(shapeData.Shape != nullptr)
				{
					shapeData.Shape->removeReference();
				}
			}
			// Clean up for less memory usage
			m_Shapes.erase(entity);

			
			
			
			hkpRigidBodyCinfo rigidBodyInfo;
			{
				rigidBodyInfo.m_shape = shape;
				rigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
				auto absoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(entity);
				hkVector4 position = GLMVEC3_TO_HKVECTOR4(absoluteTransform.Position);
				hkQuaternion rotation = GLMQUAT_TO_HKQUATERNION(absoluteTransform.Orientation);
				rigidBodyInfo.m_position.set(position(0), position(1), position(2), position(3));
				rigidBodyInfo.m_rotation.set(rotation(0), rotation(1), rotation(2), rotation(3));

				rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
				if(physicsComponent->CalculateCenterOfMass)
					physicsComponent->CenterOfMass = HKVECTOR4_TO_GLMVEC3(massProperties.m_centerOfMass);

				rigidBodyInfo.m_centerOfMass = GLMVEC3_TO_HKVECTOR4(physicsComponent->CenterOfMass);
				rigidBodyInfo.m_mass = massProperties.m_mass;
				rigidBodyInfo.m_linearVelocity = GLMVEC3_TO_HKVECTOR4(physicsComponent->InitialLinearVelocity);
				rigidBodyInfo.m_angularVelocity = GLMVEC3_TO_HKVECTOR4(physicsComponent->InitialAngularVelocity);
				rigidBodyInfo.m_linearDamping = physicsComponent->LinearDamping;
				rigidBodyInfo.m_angularDamping = physicsComponent->AngularDamping;
				rigidBodyInfo.m_gravityFactor = physicsComponent->GravityFactor;
				rigidBodyInfo.m_linearDamping = physicsComponent->LinearDamping;
				rigidBodyInfo.m_friction = physicsComponent->Friction;
				rigidBodyInfo.m_restitution = physicsComponent->Restitution;
				rigidBodyInfo.m_maxLinearVelocity = physicsComponent->MaxLinearVelocity;
				rigidBodyInfo.m_maxAngularVelocity = physicsComponent->MaxAngularVelocity;
				rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(physicsComponent->CollisionLayer, physicsComponent->CollisionSystemGroup, physicsComponent->CollisionSubSystemId, physicsComponent->CollisionSubSystemDontCollideWith);
				rigidBodyInfo.m_enableDeactivation = true;;
			}
			// Create RigidBody
			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
			m_RigidBodies[entity] = rigidBody;
			m_RigidBodyEntities[rigidBody] = entity;

			auto vehicleComponent = m_World->GetComponent<Components::Vehicle >(entity);
			if (vehicleComponent && m_Vehicles.find(entity) == m_Vehicles.end())
			{
				for (int i = 0; i < m_Wheels.size(); i++)
				{
					if(m_World->GetEntityParent(m_Wheels[i]) != entity)
					{
						m_Wheels.erase(m_Wheels.begin() + i);
						i--;
					}
				}

				VehicleSetup vehicleSetup;
				// Create the basic vehicle.
				m_Vehicles[entity] = new hkpVehicleInstance(m_RigidBodies[entity]);
				m_PhysicsWorld->markForWrite();
				vehicleSetup.buildVehicle(m_World, m_PhysicsWorld, *m_Vehicles[entity], entity, m_Wheels);
				// Add the vehicle's entities and phantoms to the world
				if(physicsComponent->CollisionEvent)
				{
					rigidBody->addContactListener( m_collisionResolution );
				}
				m_Vehicles[entity]->addToWorld(m_PhysicsWorld);


				// The vehicle is an action
				m_PhysicsWorld->addAction(m_Vehicles[entity]);
				m_PhysicsWorld->unmarkForWrite();

				//m_Vehicles[entity]->m_rpm = 0.0f; // Not sure why this one should be here
				m_Wheels.clear();
				shape->removeReference();
				rigidBody->removeReference();
			}
			else
			{
				m_PhysicsWorld->markForWrite();
				if(physicsComponent->CollisionEvent)
				{
					rigidBody->addContactListener( m_collisionResolution );
				}
				m_PhysicsWorld->addEntity(rigidBody);
				
				m_PhysicsWorld->unmarkForWrite();

				shape->removeReference();
				rigidBody->removeReference();
			}
		}
		else if(physicsComponent->MotionType == Components::Physics::MotionTypeEnum::Fixed || physicsComponent->MotionType == Components::Physics::MotionTypeEnum::Keyframed)
		{
			// Create the hkpStaticCompoundShape and add the instances.
			// "meshShape" should not be modified by the user in any way after adding it as an instance.
			hkpStaticCompoundShape* staticCompoundShape = new hkpStaticCompoundShape();

			for (auto &shapeData : m_Shapes[entity])
			{
				
				auto childTransformComponent = m_World->GetComponent<Components::Transform>(shapeData.Entity);

				hkVector4 position = GLMVEC3_TO_HKVECTOR4(childTransformComponent->Position);
				hkQuaternion rotation = GLMQUAT_TO_HKQUATERNION(childTransformComponent->Orientation);
				hkVector4 scale = GLMVEC3_TO_HKVECTOR4(childTransformComponent->Scale);
				hkQsTransform transform(position, rotation, scale);

				if(shapeData.ConvexShape != nullptr)
				{
					staticCompoundShape->addInstance(shapeData.ConvexShape, transform);
				}
				
				if(shapeData.Shape != nullptr)
				{
					staticCompoundShape->addInstance(shapeData.Shape, transform);
				}
			}
			
			// This must be called after adding the instances and before using the shape.
			staticCompoundShape->bake();
			shape = staticCompoundShape;
			hkMassProperties massProperties;
			hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, physicsComponent->Mass, massProperties);
			
			
			for (auto &shapeData : m_Shapes[entity])
			{
				if(shapeData.ConvexShape != nullptr)
				{
					shapeData.ConvexShape->removeReference();
				}
				if(shapeData.Shape != nullptr)
				{
					shapeData.Shape->removeReference();
				}
			}
			m_Shapes.erase(entity);
			
			

			hkpRigidBodyCinfo rigidBodyInfo;
			{
				rigidBodyInfo.m_shape = shape;
				if(physicsComponent->MotionType == Components::Physics::MotionTypeEnum::Fixed)
				{
					rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
				}
				else if(physicsComponent->MotionType == Components::Physics::MotionTypeEnum::Keyframed)
				{
					rigidBodyInfo.m_motionType = hkpMotion::MOTION_KEYFRAMED;
				}
				
				auto absoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(entity);
				hkVector4 position = GLMVEC3_TO_HKVECTOR4(absoluteTransform.Position);
				hkQuaternion rotation = GLMQUAT_TO_HKQUATERNION(absoluteTransform.Orientation);
				rigidBodyInfo.m_position.set(position(0), position(1), position(2), position(3));
				rigidBodyInfo.m_rotation.set(rotation(0), rotation(1), rotation(2), rotation(3));
				
				rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
				if(physicsComponent->CalculateCenterOfMass)
					physicsComponent->CenterOfMass = HKVECTOR4_TO_GLMVEC3(massProperties.m_centerOfMass);
				rigidBodyInfo.m_centerOfMass = GLMVEC3_TO_HKVECTOR4(physicsComponent->CenterOfMass);
				rigidBodyInfo.m_mass = massProperties.m_mass;
				rigidBodyInfo.m_linearVelocity = GLMVEC3_TO_HKVECTOR4(physicsComponent->InitialLinearVelocity);
				rigidBodyInfo.m_angularVelocity = GLMVEC3_TO_HKVECTOR4(physicsComponent->InitialAngularVelocity);
				rigidBodyInfo.m_linearDamping = physicsComponent->LinearDamping;
				rigidBodyInfo.m_angularDamping = physicsComponent->AngularDamping;
				rigidBodyInfo.m_gravityFactor = physicsComponent->GravityFactor;
				rigidBodyInfo.m_linearDamping = physicsComponent->LinearDamping;
				rigidBodyInfo.m_friction = physicsComponent->Friction;
				rigidBodyInfo.m_restitution = physicsComponent->Restitution;
				rigidBodyInfo.m_maxLinearVelocity = physicsComponent->MaxLinearVelocity;
				rigidBodyInfo.m_maxAngularVelocity = physicsComponent->MaxAngularVelocity;
				rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(physicsComponent->CollisionLayer, physicsComponent->CollisionSystemGroup, physicsComponent->CollisionSubSystemId, physicsComponent->CollisionSubSystemDontCollideWith);
				rigidBodyInfo.m_enableDeactivation = true;
			}
			// Create RigidBody
			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

			m_PhysicsWorld->markForWrite();
			m_PhysicsWorld->addEntity(rigidBody);
			m_RigidBodies[entity] = rigidBody;
			m_RigidBodyEntities[rigidBody] = entity;
			m_PhysicsWorld->unmarkForWrite();

			shape->removeReference();
			rigidBody->removeReference();
		}

	}
	else
	{
		if(sphereComponent)
		{
			hkpSphereShape* sphereShape = new hkpSphereShape(sphereComponent->Radius);
			//sphereShape->removeReference();

			auto triggerComponent = m_World->GetComponent<Components::Trigger >(entityParent);
			if(triggerComponent)
			{
				auto parentTransformComponent = m_World->GetComponent<Components::Transform >(entityParent);

				PhantomCallbackShape* phantom = new PhantomCallbackShape(this);
				hkpBvShape* phantomShape = new hkpBvShape(sphereShape, phantom);

				hkpRigidBodyCinfo rigidBodyInfo;
				{
					rigidBodyInfo.m_shape = phantomShape;
					rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
					rigidBodyInfo.m_position = GLMVEC3_TO_HKVECTOR4(parentTransformComponent->Position);

					rigidBodyInfo.m_mass = 1;
					rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(4, 0, 0, 0);
				}
				// Create RigidBody

				hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
				m_PhysicsWorld->markForWrite();
				m_PhysicsWorld->addEntity(rigidBody);
				m_PhysicsWorld->unmarkForWrite();

				m_RigidBodies[entityParent] = rigidBody;
				m_RigidBodyEntities[rigidBody] = entityParent;

				phantomShape->removeReference();
				phantom->removeReference();
				sphereShape->removeReference();
				rigidBody->removeReference();
			}
			else
			{
				m_Shapes[entityParent].push_back(ShapeArrayData(entity, sphereShape, nullptr));
			}
		}
		//TODO: COMMENT THIS SECTION
		else if(boxComponent)
		{
			hkReal thickness = 0.05;
			hkpBoxShape* boxShape = new hkpBoxShape(hkVector4(boxComponent->Width- thickness, boxComponent->Height -thickness, boxComponent->Depth - thickness), thickness);

			auto triggerComponent = m_World->GetComponent<Components::Trigger >(entityParent);
			if(triggerComponent)
			{
				auto parentTransformComponent = m_World->GetComponent<Components::Transform >(entityParent);
				PhantomCallbackShape* phantom = new PhantomCallbackShape(this);
				hkpBvShape* phantomShape = new hkpBvShape(boxShape, phantom);

				hkpRigidBodyCinfo rigidBodyInfo;
				{
					rigidBodyInfo.m_shape = phantomShape;
					rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
					rigidBodyInfo.m_position = GLMVEC3_TO_HKVECTOR4(parentTransformComponent->Position);

					rigidBodyInfo.m_mass = 1;
					rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(4, 0, 0, 0); // HACK:
				}
				// Create RigidBody

				hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
				m_PhysicsWorld->markForWrite();
				m_PhysicsWorld->addEntity(rigidBody);
				m_PhysicsWorld->unmarkForWrite();

				m_RigidBodies[entityParent] = rigidBody;
				m_RigidBodyEntities[rigidBody] = entityParent;

				phantomShape->removeReference();
				boxShape->removeReference();
				phantom->removeReference();
				rigidBody->removeReference();
			}
			else
			{
				m_Shapes[entityParent].push_back(ShapeArrayData(entity, boxShape, nullptr));
			}
			
		}
		else if(meshShapeComponent)
		{
			std::vector<hkReal>* vertices = new std::vector<hkReal>;
			std::vector<hkUint16>* vertexIndices = new std::vector<hkUint16>;
			auto meshShape = ResourceManager->Load<OBJ>("OBJ", meshShapeComponent->ResourceName);
			if(!meshShape)
				return;

			for (auto &vertex : meshShape->Vertices)
			{
				hkReal x, y, z;
				std::tie(x, y, z) = vertex;
				vertices->push_back(x);
				vertices->push_back(y);
				vertices->push_back(z);
			}

			int i = 0;
			for (auto &face : meshShape->Faces)
			{
				for (auto &faceDef : face.Definitions)
				{
					vertexIndices->push_back(faceDef.VertexIndex - 1);
				}
			}

			hkpExtendedMeshShape* mesh = new hkpExtendedMeshShape();
			hkReal thickness = 0.05f; // HACK: Convex radius should be 0 for static shapes and 0.05 for dynamic shapes.
			mesh->setRadius(thickness);
			{
				hkpExtendedMeshShape::TrianglesSubpart part;
				part.m_numTriangleShapes = meshShape->Faces.size();
				part.m_indexBase = vertexIndices->data();
				part.m_indexStriding = sizeof(hkUint16) * 3;

				part.m_numVertices = vertices->size() / 3;
				part.m_vertexBase = vertices->data();
				part.m_vertexStriding = sizeof(hkReal) * 3;

				part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;

				mesh->addTrianglesSubpart(part);
			}
			hkpMoppCompilerInput mci;
			hkpMoppCode* code = hkpMoppUtility::buildCode( mesh, mci );
			hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(mesh, code);

			m_ExtendedMeshShapes[entity].Code = code;
			m_ExtendedMeshShapes[entity].MoppShape = moppShape;
			m_Shapes[entityParent].push_back(ShapeArrayData(entity, nullptr, moppShape));
		}
	}
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
	LOG_INFO("%s", msg);
}

bool Systems::PhysicsSystem::OnSetVelocity( const Events::SetVelocity &event )
{
	if(m_RigidBodies.find(event.Entity) != m_RigidBodies.end())
	{

		m_PhysicsWorld->markForWrite();
		m_RigidBodies[event.Entity]->activate();
		m_RigidBodies[event.Entity]->setLinearVelocity(GLMVEC3_TO_HKVECTOR4(event.Velocity));
		auto transformComponent = m_World->GetComponent<Components::Transform>(event.Entity);
		transformComponent->Velocity = event.Velocity;
		m_PhysicsWorld->unmarkForWrite();
	}
	return true;
}

bool Systems::PhysicsSystem::OnApplyForce(const Events::ApplyForce &event)
{
	if(m_RigidBodies.find(event.Entity) != m_RigidBodies.end())
	{
		m_PhysicsWorld->markForWrite();
		m_RigidBodies[event.Entity]->applyForce(event.DeltaTime, GLMVEC3_TO_HKVECTOR4(event.Force));
		m_PhysicsWorld->unmarkForWrite();
	}
	return true;
}

bool Systems::PhysicsSystem::OnApplyPointImpulse( const Events::ApplyPointImpulse &event )
{
	if(m_RigidBodies.find(event.Entity) != m_RigidBodies.end())
	{
		m_PhysicsWorld->markForWrite();
		m_RigidBodies[event.Entity]->applyPointImpulse(GLMVEC3_TO_HKVECTOR4(event.Impulse), GLMVEC3_TO_HKVECTOR4(event.Position));
		m_PhysicsWorld->unmarkForWrite();
	}
	return true;
}


void Systems::PhysicsSystem::OnComponentRemoved(EntityID entity, std::string type, Component* component)
{
	
	if(m_RigidBodies.find(entity) != m_RigidBodies.end())
	{
		m_PhysicsWorld->markForWrite();
		m_RigidBodyEntities.erase(m_RigidBodies[entity]);
		m_PhysicsWorld->removeEntity(m_RigidBodies[entity]);
		m_RigidBodies.erase(entity);
		m_PhysicsWorld->unmarkForWrite();
	}
}


void Systems::PhysicsSystem::OnEntityRemoved( EntityID entity )
{
	/*
   	else if(m_RigidBodies.find(entity) != m_RigidBodies.end())
	{
		LOG_INFO("Removed rigid body of entity %i abogfusdkifbhdzfsgiudfhghdfhgdhfögödhfgödhfödhfödfjhödgfdgföuhudgf", entity);

		m_PhysicsWorld->markForWrite();
		m_PhysicsWorld->removeEntity(m_RigidBodies[entity]);
		m_PhysicsWorld->unmarkForWrite();

		m_RigidBodies.erase(entity);
		m_RigidBodyEntities.erase(m_RigidBodies[entity]);
		
	}*/
}

bool Systems::PhysicsSystem::OnEnableCollisions( const Events::EnableCollisions &e )
{
	m_CollisionFilter->enableCollisionsBetween(e.Layer1, e.Layer2);
	m_PhysicsWorld->markForWrite();
	m_PhysicsWorld->setCollisionFilter(m_CollisionFilter);
	m_PhysicsWorld->unmarkForWrite();
	return true;
}

bool Systems::PhysicsSystem::OnDisableCollisions( const Events::DisableCollisions &e )
{
	m_CollisionFilter->disableCollisionsBetween(e.Layer1, e.Layer2);
	m_PhysicsWorld->markForWrite();
	m_PhysicsWorld->setCollisionFilter(m_CollisionFilter);
	m_PhysicsWorld->unmarkForWrite();
	return true;
}


bool Systems::PhysicsSystem::OnTankSteer(const Events::TankSteer &event)
{
	auto vehicleComponent = m_World->GetComponent<Components::Vehicle>(event.Entity);

	if (vehicleComponent && m_Vehicles.find(event.Entity) != m_Vehicles.end() && m_RigidBodies.find(event.Entity) != m_RigidBodies.end())
	{
		hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)m_Vehicles[event.Entity]->m_deviceStatus;
		auto transformComponent = m_World->GetComponent<Components::Transform>(event.Entity);

		float steeringX = event.PositionX;

		glm::vec3 velocityNormalized = glm::normalize(HKVECTOR4_TO_GLMVEC3(m_RigidBodies[event.Entity]->getLinearVelocity()));
		glm::vec3 forward = glm::normalize(transformComponent->Orientation * glm::vec3(0, 0, -1));
		float dotProduct = glm::dot(forward, velocityNormalized);

		if(dotProduct < 0)
		{
			steeringX = (1 - (glm::clamp(abs(m_Vehicles[event.Entity]->calcKMPH() /vehicleComponent->TopSpeed), 0.f, 0.7f))) * steeringX;
		}

		if(abs(m_Vehicles[event.Entity]->calcKMPH()) < 2 && abs(m_Vehicles[event.Entity]->m_mainSteeringAngle) > 80.f * (HK_REAL_PI / 180))
		{
			deviceStatus->m_positionY = -0.4f;
			deviceStatus->m_positionX = steeringX;
		}
		else
		{
			deviceStatus->m_positionX = event.PositionX;
			deviceStatus->m_positionY = event.PositionY;
		}

		if(event.PositionY > 0)
		{
			deviceStatus->m_reverseButtonPressed = true;
		}
		deviceStatus->m_handbrakeButtonPressed = event.Handbrake;
	}

	return true;
}


bool Systems::PhysicsSystem::OnJeepSteer( const Events::JeepSteer &event )
{
	auto vehicleComponent = m_World->GetComponent<Components::Vehicle>(event.Entity);

	if (vehicleComponent && m_Vehicles.find(event.Entity) != m_Vehicles.end() && m_RigidBodies.find(event.Entity) != m_RigidBodies.end())
	{
		hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)m_Vehicles[event.Entity]->m_deviceStatus;
		deviceStatus->m_positionX = event.PositionX;
		deviceStatus->m_positionY = event.PositionY;

		if(event.PositionY > 0)
		{
			deviceStatus->m_reverseButtonPressed = true;
		}
		deviceStatus->m_handbrakeButtonPressed = event.Handbrake;
	}

	return true;
}

bool Systems::PhysicsSystem::OnDead( const Events::Dead &e )
{
	if(m_Vehicles.find(e.Entity) != m_Vehicles.end())
	{
		EntityID camera = m_World->GetProperty<EntityID>(e.Entity, "Camera");
		m_World->RemoveComponent<Components::Camera>(camera);
		auto transform = m_World->GetComponent<Components::Transform>(e.Entity);
		transform->Position = glm::vec3(0, -10000.f, 0);
		//m_RigidBodies[e.Entity]->setPosition(hkVector4(0, -10000, 0));

		/*m_Vehicles.erase(e.Entity);
		m_RigidBodyEntities.erase(m_RigidBodies[e.Entity]);
		m_RigidBodies.erase(e.Entity);*/

		auto jeep = m_World->GetComponent<Components::JeepSteering>(e.Entity);
		if(jeep)
		{
			m_World->RemoveComponent<Components::JeepSteering>(e.Entity);
		}

		auto tank = m_World->GetComponent<Components::TankSteering>(e.Entity);
		if(tank)
		{
			m_World->RemoveComponent<Components::TankSteering>(e.Entity);
		}
		
	}
	return true;
}
