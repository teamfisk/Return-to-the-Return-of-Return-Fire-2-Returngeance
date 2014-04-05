#ifndef Components_Physics_h__
#define Components_Physics_h__

//#include "btBulletDynamicsCommon.h"
#include "Component.h"

namespace Components
{

	struct Physics : Component
	{
		float Mass;
		float Inertia[3]; 
		//btRigidBody RigidBody;
	};

}

#endif // !Components_Physics_h__
