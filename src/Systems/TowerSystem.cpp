#include "PrecompiledHeader.h"
#include "TowerSystem.h"
#include "World.h"

void Systems::TowerSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Tower>([]() { return new Components::Tower(); });
}

void Systems::TowerSystem::Initialize()
{
	//EVENT_SUBSCRIBE_MEMBER(m_eDamage, &Systems::TowerSystem::Damage);
}

//bool Systems::TowerSystem::Damage( const Events::Damage &event )
//{
//	return true;
//}

void Systems::TowerSystem::Update( double dt )
{

}

void Systems::TowerSystem::UpdateEntity( double dt, EntityID entity, EntityID parent )
{
	auto towerComponent = m_World->GetComponent<Components::Tower>(entity);
	if(!towerComponent)
		return;

	Components::Transform absoluteTowerGunTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(towerComponent->Gun);

	auto towerGunTransformComponent = m_World->GetComponent<Components::Transform>(towerComponent->Gun);
	if(!towerGunTransformComponent)
		return;

	auto towerGunBaseTransformComponent = m_World->GetComponent<Components::Transform>(towerComponent->GunBase);
	if(!towerGunBaseTransformComponent)
		return;

	auto players = m_World->GetComponentsOfType<Components::Player>();

	for(auto player : *players)
	{
		auto playerComponent = std::dynamic_pointer_cast<Components::Player>(player);
		if(towerComponent->ID != playerComponent->ID) //!= betyder att den bara targetar sitt eget lag, == fienden.
			continue;

		auto playerTransform = m_World->GetComponent<Components::Transform>(player->Entity);
		if(!playerTransform)
			continue;
		Components::Transform absolutePlayerTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(player->Entity);
		
		glm::quat baseLookAt = glm::inverse(glm::quat_cast(glm::lookAt(absoluteTowerGunTransform.Position, glm::vec3(absolutePlayerTransform.Position.x, absoluteTowerGunTransform.Position.y, absolutePlayerTransform.Position.z), glm::vec3(0, 1, 0))));
		glm::quat gunLookAt = glm::inverse(glm::quat_cast(glm::lookAt(absoluteTowerGunTransform.Position, glm::vec3(absolutePlayerTransform.Position.x, absolutePlayerTransform.Position.y, absolutePlayerTransform.Position.z ), glm::vec3(0, 1, 0))));

		//glm::vec3 axisAngle = glm::eulerAngles(angle) - glm::pi<float>();

		//glm::quat yaw = glm::quat(axisAngle * glm::vec3(0, 1, 0));
		//glm::quat pitch = glm::quat(axisAngle * glm::vec3(1, 0, 0));

		//LOG_INFO("yaw: %f", glm::degrees(axisAngle.y));

		towerGunTransformComponent->Orientation = glm::inverse(baseLookAt) * gunLookAt;
		towerGunBaseTransformComponent->Orientation = baseLookAt;
	}

}

