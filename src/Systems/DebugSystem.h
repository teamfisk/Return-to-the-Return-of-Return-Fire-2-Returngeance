#ifndef DebugSystem_h__
#define DebugSystem_h__

#include "System.h"
#include "Components/Transform.h"

namespace Systems
{

	class DebugSystem : public System
	{
	public:
		DebugSystem(World* world)
			: System(world) { }

		void Update(double dt) override;
		//void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	};

}
#endif // DebugSystem_h__