#ifndef World_h__
#define World_h__

#include <stack>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>

#include <boost/any.hpp>

#include "Factory.h"
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "ResourceManager.h"

class World
{
public:
	World();
	~World();

	virtual void Initialize();

	virtual void RegisterSystems() = 0;
	virtual void AddSystems() = 0;
	virtual void RegisterComponents() = 0;


	void AddSystem(std::string systemType);
	template <class T>
	std::shared_ptr<T> GetSystem(std::string systemType);

	EntityID CreateEntity(EntityID parent = 0);
	EntityID CloneEntity(EntityID entity, EntityID parent = 0);

	void RemoveEntity(EntityID entity);

	bool ValidEntity(EntityID entity);

	EntityID GetEntityParent(EntityID entity);
	EntityID GetEntityBaseParent(EntityID entity);
	std::list<EntityID> GetEntityChildren(EntityID entity);

	template <class T>
	T GetProperty(EntityID entity, std::string property)
	{
		if(m_EntityProperties.find(entity) == m_EntityProperties.end())
			return T();
		if(m_EntityProperties[entity].find(property) == m_EntityProperties[entity].end())
			return T();

		return boost::any_cast<T>(m_EntityProperties[entity][property]);
	}

	void SetProperty(EntityID entity, std::string property, boost::any value)
	{
		m_EntityProperties[entity][property] = value;
	}

	template <class T>
	std::shared_ptr<T> AddComponent(EntityID entity, std::string componentType);
	std::shared_ptr<Component> AddComponent(EntityID entity, std::string componentType);
	template <class T>
	T* GetComponent(EntityID entity, std::string componentType);

	/*std::vector<EntityID> GetEntityChildren(EntityID entity);*/

	virtual void Update(double dt);
	// Recursively update through the scene graph
	void RecursiveUpdate(std::shared_ptr<System> system, double dt, EntityID parentEntity);

	std::unordered_map<EntityID, EntityID>* GetEntities() { return &m_EntityParents; }

	ResourceManager* GetResourceManager() { return &m_ResourceManager; }

protected:
	SystemFactory m_SystemFactory;
	ComponentFactory m_ComponentFactory;
	ResourceManager m_ResourceManager;

	std::unordered_map<std::string, std::shared_ptr<System>> m_Systems;

	EntityID m_LastEntityID;
	std::stack<EntityID> m_RecycledEntityIDs;

	std::unordered_map<EntityID, EntityID> m_EntityParents; // child -> parent
	std::unordered_map<EntityID, std::list<EntityID>> m_EntityChildren; // parent -> child
	std::unordered_map<EntityID, std::unordered_map<std::string, boost::any>> m_EntityProperties;
	std::unordered_map<std::string, std::list<std::shared_ptr<Component>>> m_ComponentsOfType;
	std::unordered_map<EntityID, std::map<std::string, std::shared_ptr<Component>>> m_EntityComponents;

	// Internal: Add a component to an entity
	void AddComponent(EntityID entity, std::string componentType, std::shared_ptr<Component> component);

	std::list<EntityID> m_EntitiesToRemove;
	void ProcessEntityRemovals();

	EntityID GenerateEntityID();

	void RecycleEntityID(EntityID id);

};

template <class T>
std::shared_ptr<T> World::GetSystem(std::string systemType)
{
	if (m_Systems.find(systemType) == m_Systems.end())
	{
		LOG_WARNING("Tried to get pointer to unregistered system \"%s\"!", systemType.c_str());
		return nullptr;
	}

	return std::static_pointer_cast<T>(m_Systems.at(systemType));
}

template <class T>
std::shared_ptr<T> World::AddComponent(EntityID entity, std::string componentType)
{
	std::shared_ptr<T> component = std::shared_ptr<T>(static_cast<T*>(m_ComponentFactory.Create(componentType)));
	if (component == nullptr)
	{
		LOG_ERROR("Failed to attach invalid component \"%s\" to entity #%i", componentType.c_str(), entity);
		return nullptr;
	}

	AddComponent(entity, componentType, component);
	
	return component;
}


template <class T>
T* World::GetComponent(EntityID entity, std::string componentType)
{
	return (T*)m_EntityComponents[entity][componentType].get();
}

#endif // World_h__