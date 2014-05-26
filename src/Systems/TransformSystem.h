#ifndef TransformSystem_h__
#define TransformSystem_h__

#include "System.h"
#include "Components/Transform.h"

namespace Systems
{

class TransformSystem : public System
{
public:
	TransformSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: System(world, eventBroker, resourceManager)
	{ }
	//void Update(double dt) override;
	//void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	
	Components::Transform AbsoluteTransform(EntityID entity);
	glm::vec3 AbsolutePosition(EntityID entity);
	glm::quat AbsoluteOrientation(EntityID entity);
	glm::vec3 AbsoluteScale(EntityID entity);
};

}

#endif // TransformSystem_h__
