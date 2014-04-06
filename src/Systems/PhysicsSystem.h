#ifndef PhysicsSystem_h__
#define PhysicsSystem_h__

#include "System.h"
#include "Components/Transform.h"
#include "Components/Physics.h"
#include "Components/BoxShape.h"
#include "Components/SphereShape.h"

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
		{
			btRigidBody*		RigidBody;
			btMotionState*		MotionState;
			btCollisionShape*	CollisionShape;
		};
		std::map<EntityID, PhysicsData> m_PhysicsData;


		void SetUpPhysicsState(EntityID entity);
		void TearDownPhysicsState(EntityID entity);

		

	};
}

#endif // PhysicsSystem_h__
