#include <boost/any.hpp>

#include "Entity.h"
#include "World.h"
#include "EventBroker.h"

// A slower wrapper class for EntityID to make
// creation of pre-defined entity hierarchies easier
class EntityGroup
{
public:
	EntityGroup(World* world, EntityID parent = 0)
		: World(world)
		, EventBroker(world->EventBroker())
	{
		m_ID = World->CreateEntity(parent);
		Initialize();
		World->CommitEntity(m_ID);
	}
	
	~EntityGroup()
	{
		World->RemoveEntity(m_ID);
	}

	virtual void Initialize() { }

	template <class T>
	std::shared_ptr<T> AddComponent(std::string componentType)
	{
		return World->AddComponent<T>(m_ID, componentType);
	}
	std::shared_ptr<Component> AddComponent(std::string componentType)
	{
		return World->AddComponent(m_ID, componentType);
	}
	
	operator EntityID () const { return m_ID; }

private:
	EntityID m_ID;
	::World* World;
	std::shared_ptr<::EventBroker> EventBroker;
};