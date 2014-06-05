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
	EVENT_SUBSCRIBE_MEMBER(m_ECollision, &Systems::TowerSystem::OnCollision);
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
	if (!players)
		return;

	for(auto player : *players)
	{
		auto playerComponent = std::dynamic_pointer_cast<Components::Player>(player);
		if(towerComponent->ID != playerComponent->ID) //!= betyder att den bara targetar sitt eget lag, == fienden.
			continue;

		auto playerTransform = m_World->GetComponent<Components::Transform>(player->Entity);
		if(!playerTransform)
			continue;

		Components::Transform absolutePlayerTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(player->Entity);

		if(glm::distance(absolutePlayerTransform.Position, absoluteTowerGunTransform.Position) > 45.f)
			continue;
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
				cloneTransform->Orientation = templateAbsoluteTransform.Orientation;
				cloneTransform->Position = templateAbsoluteTransform.Position - (cloneTransform->Orientation* glm::vec3(0.97163f, 0.f, 0.f));


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
				cloneTransform->Orientation = templateAbsoluteTransform.Orientation;
				cloneTransform->Position = templateAbsoluteTransform.Position + (cloneTransform->Orientation * glm::vec3(0.97163f, 0.f, 0.f));


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

bool Systems::TowerSystem::OnCollision(const Events::Collision &e)
{
	if(m_World->ValidEntity(e.Entity1) && m_World->ValidEntity(e.Entity2))
	{
		auto tankShell1 = m_World->GetComponent<Components::TurretShot>(e.Entity1);
		auto tankShell2 = m_World->GetComponent<Components::TurretShot>(e.Entity2);

		EntityID shellEntity = 0;
		Components::TurretShot* shellComponent;
		EntityID otherEntity = 0;

		if (tankShell1)
		{
			shellEntity = e.Entity1;
			otherEntity = e.Entity2;
			shellComponent = tankShell1;
		}
		else if (tankShell2)
		{
			shellEntity = e.Entity2;
			otherEntity = e.Entity1;
			shellComponent = tankShell2;
		}
		else
		{
			return false;
		}

		if(m_World->GetComponent<Components::Template>(shellEntity))
			return false;

		//auto physicsComponents = m_World->GetComponentsOfType<Components::Physics>();
		auto shellTransform = m_World->GetComponent<Components::Transform>(shellEntity);
		//auto otherTransform = m_World->GetComponent<Components::Transform>(otherEntity);

		
		{
			Events::CreateExplosion e;
			e.LifeTime = 2;
			e.ParticleScale.push_back(3);
			e.ParticlesToSpawn = 10;
			e.Position = shellTransform->Position;
			e.RelativeUpOrientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1, 0, 0));
			e.Speed = 3;
			e.SpreadAngle = glm::pi<float>();
			e.spritePath = "Textures/Sprites/Smoke1.png";
			e.Color = glm::vec4(0.6, 0.6, 0.6, 0.1);
			EventBroker->Publish(e);
			e.LifeTime = 0.7;
			e.ParticleScale.push_back(5);
			e.ParticlesToSpawn = 10;
			e.Position = shellTransform->Position;
			e.RelativeUpOrientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1, 0, 0));
			e.Speed = 17;
			e.SpreadAngle = glm::pi<float>();
			e.spritePath = "Textures/Sprites/Fire.png";
			EventBroker->Publish(e);
			e.LifeTime = 0.2;
			e.ParticleScale.push_back(1);
			e.ParticleScale.push_back(5);
			e.ParticlesToSpawn = 5;
			e.Position = shellTransform->Position;
			e.RelativeUpOrientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1, 0, 0));
			e.Speed = 6;
			e.SpreadAngle = glm::pi<float>();
			e.spritePath = "Textures/Sprites/Blast1.png";
			e.Color = glm::vec4(2, 2, 2, 0.2);
			EventBroker->Publish(e);
		}
			
			//{
			//	Events::CreateExplosion e;
			//	e.LifeTime = 1.5;
			//	e.ParticleScale.push_back(1);
			//	e.ParticleScale.push_back(6);
			//	e.ParticlesToSpawn = 20;
			//	e.Position = shellTransform->Position;
			//	e.SpreadAngle = glm::radians<float>(180) / 4;
			//	e.RelativeUpOrientation = glm::angleAxis(glm::radians<float>(90), glm::vec3(1,0,0));
			//	e.UseGoalVector = true;
			//	e.GoalVelocity = glm::vec3(0,-12,0);
			//	e.Speed = 12;
			//	e.spritePath = "Textures/Sprites/Splash.png";
			//	e.Color = glm::vec4(5.f,5.f,5.f,1);

			//	EventBroker->Publish(e);
			//	e.LifeTime = 1.5;
			//	e.ParticleScale.push_back(1);
			//	e.ParticleScale.push_back(6);
			//	e.ParticlesToSpawn = 20;
			//	e.Position = shellTransform->Position;
			//	e.SpreadAngle = glm::radians<float>(180)/ 4;
			//	e.RelativeUpOrientation = glm::angleAxis(glm::radians<float>(90), glm::vec3(1,0,0));
			//	e.UseGoalVector = true;
			//	e.GoalVelocity = glm::vec3(0,-12,0);
			//	e.Speed = 12;
			//	e.spritePath = "Textures/Sprites/Splash.png";
			//	e.Color = glm::vec4(1.f,1.f,1.f,1);
			//	EventBroker->Publish(e);
			//}
		

		//{
		//	Events::PlaySFX e;
		//	e.MinDistance = 150.f;
		//	e.Position = shellTransform->Position;
		//	e.Resource = "Sounds/SFX/Explosion.wav";
		//	EventBroker->Publish(e);
		//}

		// Direct damage
		auto health = m_World->GetComponent<Components::Health>(otherEntity);
		if (health)
		{
			Events::Damage d;
			d.Entity = otherEntity;
			d.Amount = shellComponent->Damage;
			EventBroker->Publish(d);
		}

		// Splash damage
		//for (auto &physComponent : *physicsComponents)
		//{
		//	EntityID physicsEntity = physComponent->Entity;
		//	// No splash damage to the direct hit target
		//	if (physicsEntity == otherEntity)
		//		continue;
		//	auto physEntityTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(physicsEntity);

		//	float distance = glm::distance(physEntityTransform.Position, shellTransform->Position);
		//	if (distance <= shellComponent->ExplosionRadius)
		//	{
		//		// DO STUFF! :D
		//		float radius = shellComponent->ExplosionRadius;
		//		float strength = (1.f - pow(distance / radius, 2)) * shellComponent->ExplosionStrength;
		//		glm::vec3 direction = glm::normalize(physEntityTransform.Position - shellTransform->Position);

		//		Events::ApplyPointImpulse e;
		//		e.Entity = physicsEntity;
		//		e.Impulse = direction * strength;
		//		e.Position = physEntityTransform.Position;
		//		EventBroker->Publish(e);

		//		auto health = m_World->GetComponent<Components::Health>(physicsEntity);
		//		if (health && health->VulnerableToSplash)
		//		{
		//			Events::Damage d;
		//			d.Entity = physicsEntity;
		//			d.Amount =  (1.f - pow(distance / radius, 2)) * shellComponent->Damage;
		//			EventBroker->Publish(d);
		//		}
		//		
		//		
		//	}
		//}
		m_World->RemoveEntity(shellEntity);

	}
	return true;
}

