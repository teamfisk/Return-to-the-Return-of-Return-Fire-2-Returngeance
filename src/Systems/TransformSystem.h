#ifndef TransformSystem_h__
#define TransformSystem_h__

#include "System.h"
#include "Components/Transform.h"

namespace Systems
{

class TransformSystem : public System
{
public:
	TransformSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
		: System(world, eventBroker) { }
	//void Update(double dt) override;
	//void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

	glm::vec3 AbsolutePosition(EntityID entity);
	glm::quat AbsoluteOrientation(EntityID entity);
	glm::vec3 AbsoluteScale(EntityID entity);
};

}

#endif // TransformSystem_h__
