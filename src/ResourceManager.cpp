#include "PrecompiledHeader.h"
#include "ResourceManager.h"

Resource* ResourceManager::CreateResource(std::string resourceType, std::string resourceName)
{
	auto it = m_FactoryFunctions.find(resourceType);
	if (it != m_FactoryFunctions.end())
	{
		return it->second(resourceName);
	}
	else
	{
		return nullptr;
	}
}

void ResourceManager::PreCache()
{
	LOG_INFO("Pre-caching resources...");
	for (auto pair : m_RegisteredResources)
	{
		std::string name = pair.first;
		std::string type = pair.second;

		Resource* resource = CreateResource(type, name);
		if (resource == nullptr)
		{
			LOG_WARNING("Failed to pre-cache %s resource \"%s\": Resource not registered!", type.c_str(), name.c_str());
			continue;
		}

		unsigned int typeID = m_ResourceTypeIDs[type];
		unsigned int resourceID = m_ResourceCount[typeID]++;
		resource->TypeID = typeID;
		resource->ResourceID = resourceID;
		m_ResourceIDs[name] = resourceID;

		m_ResourceCache[name] = resource;
	}
}

void ResourceManager::RegisterResource(std::string resourceType, std::string resourceName)
{
	m_RegisteredResources[resourceName] = resourceType;
	m_ResourceTypeIDs[resourceName] = m_ResourceTypeCount++;
}

void ResourceManager::RegisterType(std::string resourceType, std::function<Resource*(std::string)> factoryFunction)
{
	m_FactoryFunctions[resourceType] = factoryFunction;
}
