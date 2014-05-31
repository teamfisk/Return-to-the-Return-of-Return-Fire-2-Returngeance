#ifndef TransformSystem_h__
#define TransformSystem_h__

#include "System.h"
#include "Components/Transform.h"
#include "Events/Move.h"
#include "Events/Rotate.h"
#include <unordered_map>


namespace Systems
{

class TransformSystem : public System
{
public:
	TransformSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: System(world, eventBroker, resourceManager)
	{ }
	//void Update(double dt) override;

	void Initialize() override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	
	Components::Transform AbsoluteTransform(EntityID entity);
	glm::vec3 AbsolutePosition(EntityID entity);
	glm::quat AbsoluteOrientation(EntityID entity);
	glm::vec3 AbsoluteScale(EntityID entity);

	// Events
	EventRelay<TransformSystem, Events::Move> m_EMove;
	bool OnMove(const Events::Move &event);
	EventRelay<TransformSystem, Events::Rotate> m_ERotate;
	bool OnRotate(const Events::Rotate &event);
private:
	struct MoveItems
	{
		EntityID Entity;
		glm::vec3 GoalPosition;
		float Speed;
	};
	std::unordered_map<EntityID, MoveItems> m_MoveItems;
	std::multimap<EntityID, MoveItems> m_QueuedMoveItems;
	struct RotationItems
	{
		EntityID Entity;
		glm::quat GoalRotation;
		double Time;
	};
	std::unordered_map<EntityID, RotationItems> m_RotationItems;
	std::multimap<EntityID, RotationItems> m_QueuedRotationItems;
};

}

#endif // TransformSystem_h__
