#include "PrecompiledHeader.h"
#include "PhysicsSystem.h"
#include "World.h"



Systems::PhysicsSystem::PhysicsSystem(World* world) : System(world)
{
	m_Broadphase = new btDbvtBroadphase();
	m_CollisionConfiguration = new btDefaultCollisionConfiguration();
	m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
	m_Solver = new btSequentialImpulseConstraintSolver();
	
	m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfiguration);

	m_DynamicsWorld->setGravity(btVector3(0, -9.82f, 0));

}

void Systems::PhysicsSystem::Update(double dt)
{
	// Update entity transform in physics world
	for (auto pair : *m_World->GetEntities()) {
		EntityID entity = pair.first;
		EntityID parent = pair.second;

		if (parent != 0)
			continue;

		if (m_PhysicsData.find(entity) != m_PhysicsData.end()) {
			PhysicsData* physicsData = &m_PhysicsData[entity];
			auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");

			btTransform transform;
			transform.setFromOpenGLMatrix(glm::value_ptr(glm::translate(glm::mat4(), transformComponent->Position) * glm::toMat4(transformComponent->Orientation)));
			physicsData->MotionState->setWorldTransform(transform);
			physicsData->CollisionShape->setLocalScaling(btVector3(transformComponent->Scale.x, transformComponent->Scale.y, transformComponent->Scale.z));


			auto physicsComponent = m_World->GetComponent<Components::Physics>(entity, "Physics");
			physicsData->RigidBody->setGravity(btVector3(physicsComponent->Gravity.x, physicsComponent->Gravity.y, physicsComponent->Gravity.z));
		}
	}

	m_DynamicsWorld->stepSimulation(dt, 10, 1.0f/60.0f);




}

void Systems::PhysicsSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if (!transformComponent)
		return;

	auto physicsComponent = m_World->GetComponent<Components::Physics>(entity, "Physics");
	auto sphereShapeComponent = m_World->GetComponent<Components::SphereShape>(entity, "SphereShape");
	auto boxShapeComponent = m_World->GetComponent<Components::BoxShape>(entity, "BoxShape");

	if (physicsComponent || sphereShapeComponent || boxShapeComponent)
	{
		if (m_PhysicsData.find(entity) == m_PhysicsData.end()) {
			SetUpPhysicsState(entity, parent);
		}

		if (parent != 0)
			return;
		PhysicsData* physicsData = &m_PhysicsData[entity];

		btTransform transform;
		physicsData->MotionState->getWorldTransform(transform);
		transformComponent->Position.x = transform.getOrigin().x();
		transformComponent->Position.y = transform.getOrigin().y();
		transformComponent->Position.z = transform.getOrigin().z();
		btVector3 angle = transform.getRotation().getAxis();
		transformComponent->Orientation.x = transform.getRotation().x();
		transformComponent->Orientation.y = transform.getRotation().y();
		transformComponent->Orientation.z = transform.getRotation().z();
		transformComponent->Orientation.w = transform.getRotation().w();
	}
	else
	{
		if (m_PhysicsData.find(entity) != m_PhysicsData.end()) {
			TearDownPhysicsState(entity, parent);
		}
	}	
}

void Systems::PhysicsSystem::OnComponentCreated(std::string type, std::shared_ptr<Component> component)
{
	
}

void Systems::PhysicsSystem::OnComponentRemoved(std::string type, Component* component)
{

}

void Systems::PhysicsSystem::SetUpPhysicsState(EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if (!transformComponent) {
		LOG_WARNING("Physics component missing transform component on entity %i", entity);
		return;
	}

	auto physicsComponent = m_World->GetComponent<Components::Physics>(entity, "Physics");
	auto compoundShapeComponent = m_World->GetComponent<Components::CompoundShape>(entity, "CompoundShape");
	auto sphereShapeComponent = m_World->GetComponent<Components::SphereShape>(entity, "SphereShape");
	auto boxShapeComponent = m_World->GetComponent<Components::BoxShape>(entity, "BoxShape");

	if (compoundShapeComponent && (sphereShapeComponent || boxShapeComponent)) {
		LOG_WARNING("Entity %i has both compound shape and normal shape! Normal shapes must be children to entity with compound shape.", entity);
	}

	PhysicsData* physicsData = &m_PhysicsData[entity];

	/*EntityID baseParent = m_World->GetEntityBaseParent(entity);
	auto baseCompoundShape = m_World->GetComponent<Components::CompoundShape>(baseParent, "CompoundShape");*/

	// Set-up compound shape
	if (compoundShapeComponent) {
		btCompoundShape* compoundShape = new btCompoundShape();
		physicsData->CollisionShape = compoundShape;

		btTransform transform;
		transform.setFromOpenGLMatrix(glm::value_ptr(glm::translate(glm::mat4(), transformComponent->Position) * glm::toMat4(transformComponent->Orientation)));
		physicsData->MotionState = new btDefaultMotionState(transform);

		btVector3 inertia;
		if (physicsComponent->Mass != 0) {
			physicsData->CollisionShape->calculateLocalInertia(physicsComponent->Mass, inertia);
		}

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(physicsComponent->Mass, physicsData->MotionState, physicsData->CollisionShape, inertia);
		physicsData->RigidBody = new btRigidBody(rigidBodyCI);

		m_DynamicsWorld->addRigidBody(physicsData->RigidBody);
	}
	
	// Set-up normal shapes
	else if (boxShapeComponent) {
		physicsData->CollisionShape = new btBoxShape(btVector3(boxShapeComponent->Width, boxShapeComponent->Height, boxShapeComponent->Depth));
	} else if (sphereShapeComponent) {
		physicsData->CollisionShape = new btSphereShape(sphereShapeComponent->Radius);
	}
	if (boxShapeComponent || sphereShapeComponent) {
		btTransform transform;
		transform.setFromOpenGLMatrix(glm::value_ptr(glm::translate(glm::mat4(), transformComponent->Position) * glm::toMat4(transformComponent->Orientation)));

		// If there's a local physics component
		if (physicsComponent) {
			physicsData->MotionState = new btDefaultMotionState(transform);

			btVector3 inertia;
			if (physicsComponent->Mass != 0) {
				physicsData->CollisionShape->calculateLocalInertia(physicsComponent->Mass, inertia);
			}

			btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(physicsComponent->Mass, physicsData->MotionState, physicsData->CollisionShape, inertia);
			physicsData->RigidBody = new btRigidBody(rigidBodyCI);

			m_DynamicsWorld->addRigidBody(physicsData->RigidBody);
		} else { 
			// Otherwise, find our base parent and attach to compound shape
			EntityID baseParent = m_World->GetEntityBaseParent(entity);
			auto basePhysicsComponent = m_World->GetComponent<Components::Physics>(baseParent, "Physics");
			if (!basePhysicsComponent) {
				LOG_WARNING("Failed to attach orphan collision shape on entity %i: missing physics component on base parent entity %i", entity, baseParent);
				return;
			}
			auto baseCompoundShapeComponent = m_World->GetComponent<Components::CompoundShape>(baseParent, "CompoundShape");
			if (!baseCompoundShapeComponent) {
				LOG_WARNING("Failed to attach orphan collision shape on entity %i: missing compound shape on base parent entity %i", entity, baseParent);
				return;
			}
			PhysicsData* basePhysicsData = &m_PhysicsData.at(baseParent);

			btCompoundShape* baseCompoundShape = dynamic_cast<btCompoundShape*>(basePhysicsData->CollisionShape);
			baseCompoundShape->addChildShape(transform, physicsData->CollisionShape);

			btVector3 inertia;
			baseCompoundShape->calculateLocalInertia(basePhysicsComponent->Mass, inertia);

			basePhysicsData->RigidBody->setMassProps(basePhysicsComponent->Mass, inertia);
			basePhysicsData->RigidBody->updateInertiaTensor();
		}
	}
}

void Systems::PhysicsSystem::TearDownPhysicsState(EntityID entity, EntityID parent)
{
	PhysicsData* physicsData = &m_PhysicsData[entity];
	
	delete physicsData->RigidBody;
	delete physicsData->MotionState;
	delete physicsData->CollisionShape;

	m_PhysicsData.erase(entity);
}

