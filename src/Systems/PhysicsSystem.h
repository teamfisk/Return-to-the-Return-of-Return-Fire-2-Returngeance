#ifndef PhysicsSystem_h__
#define PhysicsSystem_h__

#include "System.h"
#include "Components/Transform.h"
#include "Components/Physics.h"
#include "Components/CompoundShape.h"
#include "Components/BoxShape.h"
#include "Components/SphereShape.h"
#include "Components/MeshShape.h"
#include "Components/StaticMeshShape.h"
#include "Components/BallSocketConstraint.h"
#include "Components/HingeConstraint.h"
#include "Components/SliderConstraint.h"

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btMatrix3x3.h"

namespace Systems
{

class PhysicsSystem : public System
{
public:
	PhysicsSystem(World* world);
	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	void OnComponentCreated(std::string type, std::shared_ptr<Component> component) override;
	void OnComponentRemoved(std::string type, Component* component) override;

private:
	btBroadphaseInterface*					m_Broadphase;
	btDefaultCollisionConfiguration*		m_CollisionConfiguration;
	btCollisionDispatcher*					m_Dispatcher;
	btSequentialImpulseConstraintSolver*	m_Solver;
	btDiscreteDynamicsWorld*				m_DynamicsWorld;

	struct PhysicsData
	{	btRigidBody*		RigidBody;
		btMotionState*		MotionState;
		btCollisionShape*	CollisionShape;
	};
	std::map<EntityID, PhysicsData> m_PhysicsData;
	std::map<std::pair<EntityID, EntityID>, btTypedConstraint*> m_Constraints;

	void SetUpPhysicsState(EntityID entity, EntityID parent);
	void TearDownPhysicsState(EntityID entity, EntityID parent);
};
}

#endif // PhysicsSystem_h__
