#include "PrecompiledHeader.h"
#include "ResourceManager.h"

Resource* ResourceManager::CreateResource(std::string resourceType, std::string resourceName)
{
	auto facIt = m_FactoryFunctions.find(resourceType);
	if (facIt == m_FactoryFunctions.end())
	{
		LOG_ERROR("Failed to load resource \"%s\" of type \"%s\": Type not registered", resourceName.c_str(), resourceType.c_str());
		return nullptr;
	}
	
	// Call the factory function
	Resource* resource = facIt->second(resourceName);
	// Store IDs
	resource->TypeID = GetTypeID(resourceType);
	resource->ResourceID = GetNewResourceID(resource->TypeID);
	// Cache
	m_ResourceCache[resourceName] = resource;

	return resource;
}

void ResourceManager::RegisterType(std::string resourceType, std::function<Resource*(std::string)> factoryFunction)
{
	m_FactoryFunctions[resourceType] = factoryFunction;
}

void ResourceManager::Preload(std::string resourceType, std::string resourceName)
{
	if (IsResourceLoaded(resourceName))
	{
		LOG_WARNING("Attempted to preload resource \"%s\" multiple times!", resourceName);
		return;
	}

	m_Preloading = true;
	LOG_INFO("Preloading resource \"%s\"", resourceName.c_str());
	CreateResource(resourceType, resourceName);
	m_Preloading = false;
}

unsigned int ResourceManager::GetTypeID(std::string resourceType)
{
	if (m_ResourceTypeIDs.find(resourceType) == m_ResourceTypeIDs.end())
	{
		m_ResourceTypeIDs[resourceType] = m_CurrentResourceTypeID++;
	}
	return m_ResourceTypeIDs[resourceType];
}

unsigned int ResourceManager::GetNewResourceID(unsigned int typeID)
{
	return m_ResourceCount[typeID]++;
}

bool ResourceManager::IsResourceLoaded(std::string resourceName)
{
	return m_ResourceCache.find(resourceName) != m_ResourceCache.end();
}
