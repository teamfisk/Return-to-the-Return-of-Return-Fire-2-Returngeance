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
		: m_CurrentResourceTypeID(0), m_Preloading(false) { }

	// Registers the factory function of a resource type
	void RegisterType(std::string resourceType, std::function<Resource*(std::string)> factoryFunction);

	// Loads a resource and caches it for future use
	void Preload(std::string resourceType, std::string resourceName);

	// Checks if a resource is in cache
	bool IsResourceLoaded(std::string resourceName);

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
	// Flag to suppress hot-load warnings when a preloading resource chain loads another resource
	bool m_Preloading;

	unsigned int GetTypeID(std::string resourceType);
	unsigned int GetNewResourceID(unsigned int typeID);

	// Internal: Create a resource and cache it
	Resource* CreateResource(std::string resourceType, std::string resourceName);
};

template <typename T>
T* ResourceManager::Load(std::string resourceType, std::string resourceName)
{
	auto it = m_ResourceCache.find(resourceName);
	if (it != m_ResourceCache.end())
		return static_cast<T*>(it->second);

	if (m_Preloading)
	{
		LOG_INFO("Preloading resource \"%s\"", resourceName.c_str());
	}
	else
	{
		LOG_WARNING("Hot-loading resource \"%s\"", resourceName.c_str());
	}

	return static_cast<T*>(CreateResource(resourceType, resourceName));
}

template <typename T>
T* ResourceManager::Fetch(std::string resourceName) const
{
	auto it = m_ResourceCache.find(resourceName);
	if (it == m_ResourceCache.end())
	{
		LOG_ERROR("Failed to fetch resource \"%s\": Resource not loaded!", resourceName.c_str());
		return nullptr;
	}
	else
	{
		return static_cast<T*>(it->second);
	}
}

#endif // ResourceManager_h__
