#ifndef ResourceManager_h__
#define ResourceManager_h__

#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

#include "Factory.h"

class Resource
{
public:
	unsigned int TypeID;
	unsigned int ResourceID;
};

class ResourceManager
{
public:
	ResourceManager() 
		: m_CurrentResourceTypeID(0) { }

	// Registers the factory function of a resource type
	void RegisterType(std::string resourceType, std::function<Resource*(std::string)> factoryFunction);

	// Loads a resource and caches it for future use
	void Preload(std::string resourceType, std::string resourceName);
	
	template <typename T>
	// Hot-loads a resource and caches it for future use
	T* Load(std::string resourceType, std::string resourceName);
	
	template <typename T>
	// Fetches a preloaded resource
	T* Fetch(std::string resourceName) const;

private:
	std::unordered_map<std::string, std::function<Resource*(std::string)>> m_FactoryFunctions; // type -> factory function
	std::unordered_map<std::string, Resource*> m_ResourceCache; // name -> resource

	// TODO: Getters for IDs
	unsigned int m_CurrentResourceTypeID;
	std::unordered_map<std::string, unsigned int> m_ResourceTypeIDs;
	// Number of resources of a type. Doubles as local ID.
	std::unordered_map<unsigned int, unsigned int> m_ResourceCount;

	unsigned int GetTypeID(std::string resourceType);
	unsigned int GetNewResourceID(unsigned int typeID);

	// Internal: Create a resource and cache it
	Resource* CreateResource(std::string resourceType, std::string resourceName);
};

template <typename T>
T* ResourceManager::Load(std::string resourceType, std::string resourceName)
{
	return static_cast<T*>(CreateResource(resourceType, resourceName));
}

template <typename T>
T* ResourceManager::Fetch(std::string resourceName) const
{
	if (m_ResourceCache.find(resourceName) == m_ResourceCache.end())
	{
		LOG_ERROR("Failed to fetch resource \"%s\": Resource not loaded!", resourceName.c_str());
		return nullptr;
	}
	else
	{
		return static_cast<T*>(m_ResourceCache.at(resourceName));
	}
}

#endif // ResourceManager_h__
