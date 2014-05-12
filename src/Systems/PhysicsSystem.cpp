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
	
	hkMemorySystem::FrameInfo finfo(6000 * 1024);	// Allocate 6MB of Physics solver buffer
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
		worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY; // just fix the entity if the object falls off too far

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

		m_PhysicsWorld->unmarkForWrite();
	}

}

void Systems::PhysicsSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("Physics", []() { return new Components::Physics(); });
	cf->Register("BoxShape", []() { return new Components::BoxShape(); });
	cf->Register("SphereShape", []() { return new Components::SphereShape(); });
	cf->Register("Vehicle", []() { return new Components::Vehicle(); });
	cf->Register("Wheel", []() { return new Components::Wheel(); });
	cf->Register("MeshShape", []() { return new Components::MeshShape(); });
	cf->Register("HingeConstraint", []() { return new Components::HingeConstraint(); });
	cf->Register("WheelPair", []() { return new Components::WheelPair(); });
	
}

void Systems::PhysicsSystem::Update(double dt)
{	
	for (auto pair : *m_World->GetEntities())
	{
		EntityID entity = pair.first;
		EntityID parent = pair.second;

		if (m_RigidBodies.find(entity) == m_RigidBodies.end())
			continue;

		auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
		if (!transformComponent)
			continue;

		if(m_RigidBodies[entity]->isActive())
		{
			hkVector4 position;
			hkQuaternion rotation;

			if (parent)
			{
				auto absoluteTransform = m_World->GetSystem<Systems::TransformSystem>("TransformSystem")->AbsoluteTransform(entity);
				position = ConvertPosition(absoluteTransform.Position);
				rotation = ConvertRotation(absoluteTransform.Orientation);
			}
			else
			{
				position = ConvertPosition(transformComponent->Position);
				rotation = ConvertRotation(transformComponent->Orientation);
			}
			m_PhysicsWorld->markForWrite();
			m_RigidBodies[entity]->setPositionAndRotation(position, rotation);
			m_PhysicsWorld->unmarkForWrite();
			
		}
		
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
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if (!transformComponent)
		return;
	
	auto wheelComponent = m_World->GetComponent<Components::Wheel>(entity, "Wheel");
	if (wheelComponent)
	{
		EntityID car = m_World->GetEntityParent(entity);
		if(m_Vehicles.find(car) != m_Vehicles.end())
		{
			m_PhysicsWorld->markForWrite();
			m_Vehicles[car]->getChassis()->activate();

			hkVector4 hardPoint = m_Vehicles[car]->m_suspension->m_wheelParams[wheelComponent->ID].m_hardpointChassisSpace;
			hkVector4 suspensionDirection =  m_Vehicles[car]->m_suspension->m_wheelParams[wheelComponent->ID].m_directionChassisSpace;
			hkReal suspensionLength = m_Vehicles[car]->m_wheelsInfo[wheelComponent->ID].m_currentSuspensionLength;
			glm::vec3 position = glm::vec3(hardPoint(0) + (suspensionDirection(0) * suspensionLength), hardPoint(1) + (suspensionDirection(1) * suspensionLength), hardPoint(2) + (suspensionDirection(2) * suspensionLength));
			transformComponent->Position = position;
			
			hkQuaternion steeringOrientation = m_Vehicles[car]->m_wheelsInfo[wheelComponent->ID].m_steeringOrientationChassisSpace;
			hkReal spinAngle = -m_Vehicles[car]->m_wheelsInfo[wheelComponent->ID].m_spinAngle;
			glm::quat orientation = ConvertRotation(steeringOrientation) * glm::angleAxis<float>(spinAngle, glm::vec3(1, 0, 0));
			transformComponent->Orientation = orientation * wheelComponent->OriginalOrientation;
			m_PhysicsWorld->unmarkForWrite();
		}
	}
	else if(m_RigidBodies.find(entity) != m_RigidBodies.end())
	{
		auto transformComponentParent = m_World->GetComponent<Components::Transform>(parent, "Transform");

		transformComponent->Position = ConvertPosition(m_RigidBodies[entity]->getPosition());
		transformComponent->Orientation = ConvertRotation(m_RigidBodies[entity]->getRotation());
		// TODO: No support for Scale, MIGHT be possible

		if (transformComponentParent)
		{
			transformComponent->Position -= transformComponentParent->Position;
			transformComponent->Position = transformComponent->Position * transformComponentParent->Orientation;
			transformComponent->Orientation =  transformComponent->Orientation * glm::inverse(transformComponentParent->Orientation);
		}
	}

	
}

void Systems::PhysicsSystem::OnEntityCommit( EntityID entity )
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if (!transformComponent)
		return;

	auto wheelComponent = m_World->GetComponent<Components::Wheel>(entity, "Wheel");
	if (wheelComponent)
	{
		wheelComponent->ID = m_Wheels.size();
		wheelComponent->OriginalOrientation = transformComponent->Orientation;
		m_Wheels.push_back(entity);
	}

	EntityID entityParent = m_World->GetEntityBaseParent(entity);

	auto sphereComponent = m_World->GetComponent<Components::SphereShape>(entity, "SphereShape");
	auto boxComponent = m_World->GetComponent<Components::BoxShape>(entity, "BoxShape");
	auto meshShapeComponent = m_World->GetComponent<Components::MeshShape >(entity, "MeshShape");
	
	if(entityParent == entity && (sphereComponent || boxComponent || meshShapeComponent))
	{
		LOG_ERROR("Entity: %i , Only the children can have a shapeComponent", entity);
		return;
	}

	auto physicsComponent = m_World->GetComponent<Components::Physics>(entity, "Physics");
	if (physicsComponent)
	{
		hkpShape* shape;	
		if(entityParent != entity)
		{
			LOG_ERROR("Entity: %i , Only the baseparent can have a PhysicsComponent", entity);
			return;
		}

		if(! physicsComponent->Static) // Not static
		{
			hkArray<hkpShape*> shapeArray;
			for (auto &shapeData : m_Shapes[entity])
			{
				shapeArray.pushBack(shapeData.Shape);
			}


			// Create a hkpListShape* of all the childEntities collected in m_ShapeArrays
			hkpListShape* listShape = new hkpListShape(shapeArray.begin(), shapeArray.getSize(), hkpShapeContainer::REFERENCE_POLICY_INCREMENT);
			// Save the listShape for further use
			m_ListShapes[entity] = listShape;
			shape = listShape;

			//////////////////////////////////
			//******************************//
			//     Add a hkpBvShape         //
			//******************************//
			//////////////////////////////////

			// Clean up for less memory usage
			m_Shapes.erase(entity);

			hkMassProperties massProperties;
			hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, physicsComponent->Mass, massProperties);
			
			hkpRigidBodyCinfo rigidBodyInfo;
			{
				rigidBodyInfo.m_shape = shape;
				rigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
				auto absoluteTransform = m_World->GetSystem<Systems::TransformSystem>("TransformSystem")->AbsoluteTransform(entity);
				hkVector4 position = ConvertPosition(absoluteTransform.Position);
				hkQuaternion rotation = ConvertRotation(absoluteTransform.Orientation);
				rigidBodyInfo.m_position.set(position(0), position(1), position(2), position(3));
				rigidBodyInfo.m_rotation.set(rotation(0), rotation(1), rotation(2), rotation(3));

				rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
				//rigidBodyInfo.m_centerOfMass = massProperties.m_centerOfMass; //HACK: CENTER OF MASS ALWAYS IN THE CENTER
				rigidBodyInfo.m_mass = massProperties.m_mass;
			}
			// Create RigidBody
			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

			auto vehicleComponent = m_World->GetComponent<Components::Vehicle >(entity, "Vehicle");
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
				m_Vehicles[entity] = new hkpVehicleInstance(rigidBody);
				m_PhysicsWorld->markForWrite();
				vehicleSetup.buildVehicle(m_World, m_PhysicsWorld, *m_Vehicles[entity], entity, m_Wheels);
				// Add the vehicle's entities and phantoms to the world
				m_Vehicles[entity]->addToWorld(m_PhysicsWorld);

				m_RigidBodies[entity] = rigidBody;

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
				m_PhysicsWorld->addEntity(rigidBody);
				m_RigidBodies[entity] = rigidBody;
				m_PhysicsWorld->unmarkForWrite();

				shape->removeReference();
				rigidBody->removeReference();
			}
		}
		else // Static
		{
			// Create the hkpStaticCompoundShape and add the instances.
			// "meshShape" should not be modified by the user in any way after adding it as an instance.
			hkpStaticCompoundShape* staticCompoundShape = new hkpStaticCompoundShape();

			for (auto &shapeData : m_Shapes[entity])
			{
				
				auto childTransformComponent = m_World->GetComponent<Components::Transform>(shapeData.Entity, "Transform");

				hkVector4 position = ConvertPosition(childTransformComponent->Position);
				hkQuaternion rotation = ConvertRotation(childTransformComponent->Orientation);
				hkVector4 scale = ConvertScale(childTransformComponent->Scale);
				hkQsTransform transform(position, rotation, scale);

				staticCompoundShape->addInstance(shapeData.Shape, transform);
			}
			
			// This must be called after adding the instances and before using the shape.
			staticCompoundShape->bake();
			shape = staticCompoundShape;
			m_Shapes.erase(entity);
			hkMassProperties massProperties;
			hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, physicsComponent->Mass, massProperties);

			hkpRigidBodyCinfo rigidBodyInfo;
			{
				rigidBodyInfo.m_shape = shape;
				rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
				auto absoluteTransform = m_World->GetSystem<Systems::TransformSystem>("TransformSystem")->AbsoluteTransform(entity);
				hkVector4 position = ConvertPosition(absoluteTransform.Position);
				hkQuaternion rotation = ConvertRotation(absoluteTransform.Orientation);
				rigidBodyInfo.m_position.set(position(0), position(1), position(2), position(3));
				rigidBodyInfo.m_rotation.set(rotation(0), rotation(1), rotation(2), rotation(3));

				rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
				//rigidBodyInfo.m_centerOfMass = massProperties.m_centerOfMass; //HACK: CENTER OF MASS ALWAYS IN THE CENTER
				rigidBodyInfo.m_mass = massProperties.m_mass;
			}
			// Create RigidBody
			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

			m_PhysicsWorld->markForWrite();
			m_PhysicsWorld->addEntity(rigidBody);
			m_RigidBodies[entity] = rigidBody;
			m_PhysicsWorld->unmarkForWrite();

			shape->removeReference();
			rigidBody->removeReference();

			
		}

	}
	else
	{
		
		//TODO: COMMENT THIS SECTION
		if(sphereComponent)
		{
			hkpSphereShape* sphereShape = new hkpSphereShape(sphereComponent->Radius);
			
			hkQsTransform transform( ConvertPosition(transformComponent->Position), ConvertRotation(transformComponent->Orientation), ConvertScale(transformComponent->Scale));
			hkpConvexTransformShape* transformedSphereShape = new hkpConvexTransformShape( sphereShape, transform );
			
			m_Shapes[entityParent].push_back(ShapeArrayData(entity, transformedSphereShape));

			sphereShape->removeReference();
		}
		//TODO: COMMENT THIS SECTION
		else if(boxComponent)
		{
			hkReal thickness = 0.05;
			hkpBoxShape* boxShape = new hkpBoxShape(hkVector4(boxComponent->Width- thickness, boxComponent->Height -thickness, boxComponent->Depth - thickness), thickness);
			
			hkQsTransform transform( ConvertPosition(transformComponent->Position), ConvertRotation(transformComponent->Orientation), ConvertScale(transformComponent->Scale));
			hkpConvexTransformShape* transformedBoxShape = new hkpConvexTransformShape( boxShape, transform );
			m_Shapes[entityParent].push_back(ShapeArrayData(entity, transformedBoxShape));
			boxShape->removeReference();
		}
		else if(meshShapeComponent)
		{
			std::vector<hkReal>* vertices = new std::vector<hkReal>;
			std::vector<hkUint16>* vertexIndices = new std::vector<hkUint16>;
			auto meshShape = m_World->GetResourceManager()->Load<OBJ>("OBJ", meshShapeComponent->ResourceName);

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
			m_Shapes[entityParent].push_back(ShapeArrayData(entity, moppShape)); //HACK: Should maybe have transform, not sure yet
		}
	}

	

}

void Systems::PhysicsSystem::TearDownPhysicsState(EntityID entity, EntityID parent)
{	

}

void Systems::PhysicsSystem::OnComponentCreated(std::string type, std::shared_ptr<Component> component)
{

}

void Systems::PhysicsSystem::OnComponentRemoved(std::string type, Component* component)
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


glm::vec3 Systems::PhysicsSystem::ConvertPosition(const hkVector4 &hkPosition)
{
	return glm::vec3(hkPosition(0), hkPosition(1), hkPosition(2));
}

const hkVector4& Systems::PhysicsSystem::ConvertPosition(glm::vec3 glmPosition)
{
	return hkVector4( glmPosition.x, glmPosition.y, glmPosition.z);
}

glm::quat Systems::PhysicsSystem::ConvertRotation(const hkQuaternion &hkRotation)
{
	return glm::quat(hkRotation(3), hkRotation(0), hkRotation(1), hkRotation(2));
}

const hkQuaternion& Systems::PhysicsSystem::ConvertRotation(glm::quat glmRotation)
{
	return hkQuaternion(glmRotation.x, glmRotation.y, glmRotation.z, glmRotation.w);
}

glm::vec3 Systems::PhysicsSystem::ConvertScale(const hkVector4 &hkScale)
{
	return glm::vec3(hkScale(0), hkScale(1), hkScale(2));
}

const hkVector4& Systems::PhysicsSystem::ConvertScale(glm::vec3 glmScale)
{
	return hkVector4(glmScale.x, glmScale.y, glmScale.z);
}

bool Systems::PhysicsSystem::OnTankSteer(const Events::TankSteer &event)
{
	auto vehicleComponent = m_World->GetComponent<Components::Vehicle>(event.Entity, "Vehicle");
	auto inputComponent = m_World->GetComponent<Components::Input>(event.Entity, "Input");
	if (vehicleComponent && inputComponent && m_Vehicles.find(event.Entity) != m_Vehicles.end() && m_RigidBodies.find(event.Entity) != m_RigidBodies.end())
	{
		m_PhysicsWorld->markForWrite();
		hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)m_Vehicles[event.Entity]->m_deviceStatus;
		deviceStatus->m_positionX = event.PositionX;
		deviceStatus->m_positionY = event.PositionY;
		deviceStatus->m_handbrakeButtonPressed = event.Handbrake;
		m_PhysicsWorld->unmarkForWrite();
	}

	return true;
}

