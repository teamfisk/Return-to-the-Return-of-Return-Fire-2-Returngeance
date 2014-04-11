#ifndef PhysicsSystem_h__
#define PhysicsSystem_h__

#include "System.h"
#include "Components/Transform.h"

#include <unordered_set>

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
	void SetUpPhysicsState(EntityID entity, EntityID parent);
	void TearDownPhysicsState(EntityID entity, EntityID parent);
};
}

#endif // PhysicsSystem_h__
