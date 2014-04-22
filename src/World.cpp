#include "PrecompiledHeader.h"
#include "World.h"

void World::RecycleEntityID(EntityID id)
{
	m_RecycledEntityIDs.push(id);
}

EntityID World::GenerateEntityID()
{
	if (!m_RecycledEntityIDs.empty())
	{
		EntityID id = m_RecycledEntityIDs.top();
		m_RecycledEntityIDs.pop();
		return id;
	}
	else
	{
		return ++m_LastEntityID;
	}
}

void World::RecursiveUpdate(std::shared_ptr<System> system, double dt, EntityID parentEntity)
{
	for (auto pair : m_EntityParents)
	{
		EntityID child = pair.first;
		EntityID parent = pair.second;

		if (parent == parentEntity)
		{
			system->UpdateEntity(dt, child, parent);
			RecursiveUpdate(system, dt, child);
		}
	}
}

void World::Update(double dt)
{
	for (auto pair : m_Systems)
	{
		auto system = pair.second;
		system->Update(dt);
		RecursiveUpdate(system, dt, 0);
	}

	ProcessEntityRemovals();
}

//std::vector<EntityID> GetEntityChildren(EntityID entity);
//{
//	std::vector<EntityID> children;
//	auto range = m_SceneGraph.equal_range(entity);
//	for (auto it = range.first; it != range.second; ++it)
//		children.push_back(it->second);
//	return children;
//}

EntityID World::GetEntityParent(EntityID entity)
{
	auto it = m_EntityParents.find(entity);
	return it == m_EntityParents.end() ? 0 : it->second;
}

EntityID World::GetEntityBaseParent(EntityID entity)
{
	EntityID parent = GetEntityParent(entity);
	if (parent == 0)
		return entity;
	else
		return GetEntityBaseParent(parent);
}

bool World::ValidEntity(EntityID entity)
{
	return m_EntityParents.find(entity) != m_EntityParents.end();
}

void World::RemoveEntity(EntityID entity)
{
	m_EntitiesToRemove.push_back(entity);
	for (auto pair : m_EntityParents)
	{
		if (pair.second == entity)
		{
			m_EntitiesToRemove.push_back(pair.first);
		}
	}
}

void World::ProcessEntityRemovals()
{
	for (auto entity : m_EntitiesToRemove)
	{
		m_EntityParents.erase(entity);
		// Remove components
		for (auto pair : m_EntityComponents[entity])
		{
			auto type = pair.first;
			auto component = pair.second;
			// Trigger events
			for (auto pair : m_Systems)
			{
				auto system = pair.second;
				system->OnComponentRemoved(type, component.get());
			}
			m_ComponentsOfType[type].remove(component);
		}
		m_EntityComponents.erase(entity);

		RecycleEntityID(entity);
	}
	m_EntitiesToRemove.clear();
}

EntityID World::CreateEntity(EntityID parent /*= 0*/)
{
	EntityID newEntity = GenerateEntityID();
	m_EntityParents.insert(std::pair<EntityID, EntityID>(newEntity, parent));
	return newEntity;
}

World::~World()
{

}

World::World()
{
	m_LastEntityID = 0;
}

void World::Initialize()
{
	RegisterSystems();
	AddSystems();
	for (auto system : m_Systems)
	{
		system.second->Initialize();
	}

	RegisterComponents();
}

std::shared_ptr<Component> World::AddComponent(EntityID entity, std::string componentType)
{
	return AddComponent<Component>(entity, componentType);
}

void World::CommitEntity(EntityID entity)
{
	for (auto pair : m_Systems)
	{
		auto system = pair.second;
		system->OnEntityCommit(entity);
	}
}

void World::AddSystem(std::string systemType)
{
	m_Systems[systemType] = std::shared_ptr<System>(m_SystemFactory.Create(systemType));
}
