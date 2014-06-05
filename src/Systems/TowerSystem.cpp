#include "PrecompiledHeader.h"
#include "TowerSystem.h"
#include "World.h"

void Systems::TowerSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Tower>([]() { return new Components::Tower(); });
	cf->Register<Components::Turret>([]() { return new Components::Turret(); });
	cf->Register<Components::TurretShot>([]() {return new Components::TurretShot(); });
}

void Systems::TowerSystem::Initialize()
{
	//EVENT_SUBSCRIBE_MEMBER(m_eDamage, &Systems::TowerSystem::Damage);
	Temp_m_TimeSinceLastShot = 0;
}

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

	auto turretComponent = m_World->GetComponent<Components::Turret>(entity);
	if(!turretComponent)
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
		
		//Rotate the turret towards the enemy player
		glm::quat baseLookAt = glm::inverse(glm::quat_cast(glm::lookAt(absoluteTowerGunTransform.Position, glm::vec3(absolutePlayerTransform.Position.x, absoluteTowerGunTransform.Position.y, absolutePlayerTransform.Position.z), glm::vec3(0, 1, 0))));
		glm::quat gunLookAt = glm::inverse(glm::quat_cast(glm::lookAt(absoluteTowerGunTransform.Position, glm::vec3(absolutePlayerTransform.Position.x, absolutePlayerTransform.Position.y, absolutePlayerTransform.Position.z ), glm::vec3(0, 1, 0))));

		towerGunTransformComponent->Orientation = glm::inverse(baseLookAt) * gunLookAt;
		towerGunBaseTransformComponent->Orientation = baseLookAt;
		
#pragma region GetShotDirectionAndLaunchShot
		Temp_m_TimeSinceLastShot += dt;
		if(Temp_m_TimeSinceLastShot > 1)
		{
			{
				//Shoot code
				EntityID clone = m_World->CloneEntity(turretComponent->ShotTemplate);
				auto templateAbsoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(turretComponent->ShotTemplate);
				auto cloneTransform = m_World->GetComponent<Components::Transform>(clone);
				cloneTransform->Position = templateAbsoluteTransform.Position - glm::vec3(0.97163f, 0.f, 0.f);
				cloneTransform->Orientation = templateAbsoluteTransform.Orientation;

				Events::SetVelocity eSetVelocity;
				eSetVelocity.Entity = clone;
				eSetVelocity.Velocity = gunLookAt * glm::vec3(0.f, 0.f, -1.f) * turretComponent->ShotSpeed;
				EventBroker->Publish(eSetVelocity);

				{
					Events::CreateExplosion e;
					e.LifeTime = 1.5;
					e.ParticleScale.push_back(0.6);
					e.ParticleScale.push_back(1.8);
					e.ParticlesToSpawn = 1;
					e.Position = cloneTransform->Position;
					e.RelativeUpOrientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1,0,0));
					e.Speed = 1.7;
					e.SpreadAngle = glm::pi<float>()/100;
					e.spritePath = "Textures/Sprites/Smoke1.png";
					e.Emitting = true;
					e.SpawnFrequency = 0.2;
					e.Color = glm::vec4(0.6,0.6,0.6,1);
					EventBroker->Publish(e);
				}
				{
					Events::CreateExplosion e;
					e.LifeTime = 0.2;
					e.ParticleScale.push_back(1);
					//e.ParticleScale.push_back(2);
					e.ParticlesToSpawn = 1;
					e.Position = cloneTransform->Position;
					e.Speed = 0;
					e.SpreadAngle = glm::pi<float>();
					e.spritePath = "Textures/Sprites/MuzzleFlash.png";
					e.Color = glm::vec4(2, 2, 2, 0.2);
					EventBroker->Publish(e);
				}
			}

			{
				EntityID clone = m_World->CloneEntity(turretComponent->ShotTemplate);
				auto templateAbsoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(turretComponent->ShotTemplate);
				auto cloneTransform = m_World->GetComponent<Components::Transform>(clone);
				cloneTransform->Position = templateAbsoluteTransform.Position + glm::vec3(0.97163f, 0.f, 0.f);
				cloneTransform->Orientation = templateAbsoluteTransform.Orientation;

				Events::SetVelocity eSetVelocity;
				eSetVelocity.Entity = clone;
				eSetVelocity.Velocity = gunLookAt * glm::vec3(0.f, 0.f, -1.f) * turretComponent->ShotSpeed;
				EventBroker->Publish(eSetVelocity);

				{
					Events::CreateExplosion e;
					e.LifeTime = 1.5;
					e.ParticleScale.push_back(0.6);
					e.ParticleScale.push_back(1.8);
					e.ParticlesToSpawn = 1;
					e.Position = cloneTransform->Position;
					e.RelativeUpOrientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1,0,0));
					e.Speed = 1.7;
					e.SpreadAngle = glm::pi<float>()/100;
					e.spritePath = "Textures/Sprites/Smoke1.png";
					e.Emitting = true;
					e.SpawnFrequency = 0.2;
					e.Color = glm::vec4(0.6,0.6,0.6,1);
					EventBroker->Publish(e);
				}
				{
					Events::CreateExplosion e;
					e.LifeTime = 0.2;
					e.ParticleScale.push_back(1);
					//e.ParticleScale.push_back(2);
					e.ParticlesToSpawn = 1;
					e.Position = cloneTransform->Position;
					e.Speed = 0;
					e.SpreadAngle = glm::pi<float>();
					e.spritePath = "Textures/Sprites/MuzzleFlash.png";
					e.Color = glm::vec4(2, 2, 2, 0.2);
					EventBroker->Publish(e);
				}
			}

			Temp_m_TimeSinceLastShot = 0;
		
#pragma endregion

		}
	}

}

