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
	// Registers the factory function of a resource type
	void RegisterType(std::string resourceType, std::function<Resource*(std::string)> factoryFunction);

	// Registers a future instance of a resource
	void RegisterResource(std::string resourceType, std::string resourceName);

	// Loads all registered resources and caches them
	void PreCache();

	template <typename T>
	T* Fetch(std::string resourceName) const;

private:
	unsigned int m_ResourceTypeCount = 0;
	std::unordered_map<std::string, unsigned int> m_ResourceTypeIDs;
	std::unordered_map<unsigned int, unsigned int> m_ResourceCount;
	std::unordered_map<std::string, unsigned int> m_ResourceIDs;

	std::unordered_map<std::string, std::function<Resource*(std::string)>> m_FactoryFunctions; // type -> factory function
	std::unordered_map<std::string, std::string> m_RegisteredResources; // name -> type
	std::unordered_map<std::string, Resource*> m_ResourceCache; // name -> resource

	Resource* CreateResource(std::string resourceType, std::string resourceName);
};

template <typename T>
T* ResourceManager::Fetch(std::string resourceName) const
{
	if (m_ResourceCache.find(resourceName) == m_ResourceCache.end())
	{
		LOG_ERROR("Failed to load resource \"%s\": Resource not precached!", resourceName.c_str());
		return nullptr;
	}
	else
	{
		return static_cast<T*>(m_ResourceCache.at(resourceName));
	}
}

#endif // ResourceManager_h__
