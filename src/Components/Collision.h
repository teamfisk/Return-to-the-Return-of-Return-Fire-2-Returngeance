#ifndef Components_Collision_h__
#define Components_Collision_h__

#include "Entity.h"
#include "Component.h"
#include <vector>

namespace Components
{

struct Collision : Component
{	Collision() : Phantom(false), Interested(false) { }

	bool Phantom;
	bool Interested;
	std::vector<EntityID> CollidingEntities;
};

}
#endif // !Components_Collision_h__