#include "PrecompiledHeader.h"
#include "PhysicsSystem.h"
#include "World.h"



Systems::PhysicsSystem::PhysicsSystem(World* world) : System(world)
{	

}

void Systems::PhysicsSystem::Update(double dt)
{	

}

void Systems::PhysicsSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{	

}

void Systems::PhysicsSystem::OnComponentCreated(std::string type, std::shared_ptr<Component> component)
{

}

void Systems::PhysicsSystem::OnComponentRemoved(std::string type, Component* component)
{

}

void Systems::PhysicsSystem::SetUpPhysicsState(EntityID entity, EntityID parent)
{	

}

void Systems::PhysicsSystem::TearDownPhysicsState(EntityID entity, EntityID parent)
{	

}

