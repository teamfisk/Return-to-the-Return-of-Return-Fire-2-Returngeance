#ifndef System_h__
#define System_h__

#include "Factory.h"
#include "Entity.h"
#include "Component.h"
#include "ResourceManager.h"

class World;

class System
{
public:
	System(World* world) : m_World(world) { }
	virtual ~System() { }

	virtual void RegisterComponents(ComponentFactory* cf) { }
	virtual void RegisterResourceTypes(ResourceManager* rm) { }

	virtual void Initialize() { }

	// Called once per system every tick
	virtual void Update(double dt) { }
	// Called once for every entity in the world every tick
	virtual void UpdateEntity(double dt, EntityID entity, EntityID parent) { }

	// Called when a component is created
	virtual void OnComponentCreated(std::string type, std::shared_ptr<Component> component) { }
	// Called when a component is removed
	virtual void OnComponentRemoved(std::string type, Component* component) { }
	// Called when components are committed to an entity
	virtual void OnEntityCommit(EntityID entity) { }

protected:
	World* m_World;
};

class SystemFactory : public Factory<System*> { };

#endif // System_h__