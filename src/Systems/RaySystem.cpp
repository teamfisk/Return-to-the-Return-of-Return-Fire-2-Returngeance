#include "PrecompiledHeader.h"
#include "RaySystem.h"
#include "World.h"

void Systems::RaySystem::Initialize()
{
	// Subscribe to events
	EVENT_SUBSCRIBE_MEMBER(m_ECastRay, &Systems::RaySystem::OnCastRay);
}

void Systems::RaySystem::Update(double dt)
{

}

void Systems::RaySystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	
}

bool Systems::RaySystem::OnCastRay(const Events::CastRay &event)
{
	Ray r;
	return true;
}