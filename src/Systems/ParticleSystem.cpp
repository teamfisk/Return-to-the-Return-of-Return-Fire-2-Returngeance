#include "PrecompiledHeader.h"
#include "ParticleSystem.h"


#include "World.h"

Systems::ParticleSystem::ParticleSystem(World *m_World) : System(m_World)
{
	
}

void Systems::ParticleSystem::Update(double dt)
{
	
}

void Systems::ParticleSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if(!transformComponent)
		return;

	auto emitterComponent = m_World->GetComponent<Components::ParticleEmitter>(entity, "ParticleEmitter");
	if(emitterComponent)
	{
		emitterComponent->TimeSinceLastSpawn += dt;

		auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
		if(emitterComponent->TimeSinceLastSpawn > emitterComponent->SpawnFrequency)
		{
			SpawnParticles(entity, transformComponent->Position, emitterComponent->SpawnCount, emitterComponent->SpreadAngle, emitterComponent->LifeTime, dt);
			emitterComponent->TimeSinceLastSpawn = 0;
		}

		std::list<ParticleData>::iterator it;
		for(it = m_ParticleEmitter[entity].begin(); it != m_ParticleEmitter[entity].end();)
		{
			EntityID particleID = (it)->ParticleID;
			auto transformComponent = m_World->GetComponent<Components::Transform>(particleID, "Transform");
			
			transformComponent->Position +=  transformComponent->Velocity;

			auto particleComponent = m_World->GetComponent<Components::Particle>(particleID, "Particle");
			double timeLived = glfwGetTime() - it->SpawnTime; 
			if(timeLived > particleComponent->LifeTime)
			{
				
				m_World->RemoveEntity(particleID);
				m_ParticleEmitter[entity].erase(it);
				break;
			}
			else
			{
				it++;
			}

			// Interpolates the color for each color channel by the start and end value. Decides how much the color should be interpolated based on time.
			/*// How big fraction the color is multiplied with
			float timeProgress = timeLived / particleComponent->LifeTime; 
			// The difference between the start and end value
			float deltaColor = glm::abs(particleComponent->ColorSpectrum[0].r - particleComponent->ColorSpectrum[1].r); 
			it->color.r = particleComponent->ColorSpectrum[0].r + deltaColor * timeProgress;
			deltaColor = glm::abs(particleComponent->ColorSpectrum[0].g - particleComponent->ColorSpectrum[1].g);
			it->color.g = particleComponent->ColorSpectrum[0].g + deltaColor * timeProgress;
			deltaColor = glm::abs(particleComponent->ColorSpectrum[0].b - particleComponent->ColorSpectrum[1].b);
			it->color.b = particleComponent->ColorSpectrum[0].b + deltaColor * timeProgress;

			//Interpolates the scale of the particle
			float deltaScale = glm::abs(particleComponent->ScaleSpectrum[0] - particleComponent->ScaleSpectrum[1]);
			it->Scale = particleComponent->ScaleSpectrum[0] + deltaScale * timeProgress;

			//Interpolates the velocity of the particle
			float deltaVelocity = glm::abs(particleComponent->VelocitySpectrum[0].x - particleComponent->VelocitySpectrum[1].x);
			it->Velocity.x = particleComponent->VelocitySpectrum[0].x + deltaVelocity * timeProgress;
			deltaVelocity = glm::abs(particleComponent->VelocitySpectrum[0].y - particleComponent->VelocitySpectrum[1].y);
			it->Velocity.y = particleComponent->VelocitySpectrum[0].y + deltaVelocity * timeProgress;	
			deltaVelocity = glm::abs(particleComponent->VelocitySpectrum[0].z - particleComponent->VelocitySpectrum[1].z);
			it->Velocity.z = particleComponent->VelocitySpectrum[0].z + deltaVelocity * timeProgress;*/
		}
	}
}

void Systems::ParticleSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("ParticleEmitter", []() { return new Components::ParticleEmitter(); });
	cf->Register("Particle", []() { return new Components::Particle(); });
}

void Systems::ParticleSystem::SpawnParticles(EntityID emitterID, glm::vec3 pos, float spawnCount, float spreadAngle, double lifeTime, double dt)
{
	auto emitterTransform = m_World->GetComponent<Components::Transform>(emitterID, "Transform");
	glm::quat emitterOrientation = emitterTransform->Orientation;
	float tempSpeed =  5 * dt;
	glm::vec3 speed = glm::vec3(tempSpeed);
	for(int i = 0; i < spawnCount; i++)
	{
		auto ent = m_World->CreateEntity();
		
		auto particleTransform = m_World->AddComponent<Components::Transform>(ent, "Transform");
		particleTransform->Position.x = pos.x;
		particleTransform->Position.y = pos.y;
		particleTransform->Position.z = pos.z;
		particleTransform->Scale = glm::vec3(1, 1, 1);
		//The emitter's orientation as "start value" times the default direction for quaternion. Times the speed, and then rotate on x and y axis with the randomized spread angle. 
		particleTransform->Velocity = emitterOrientation * glm::vec3(0, 0, -1) * speed * 
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(1, 0, 0))) * 
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(0, 1, 0)));


		auto particle = m_World->AddComponent<Components::Particle>(ent, "Particle");
		particle->LifeTime = lifeTime;
		
		/*Color startColor = {.4f, .45f, .2f};
		particle->ColorSpectrum.push_back(startColor);
		Color endColor = {0.f, 45.f, 23.f};
		particle->ColorSpectrum.push_back(endColor);
		particle->ScaleSpectrum.push_back(1);
		particle->ScaleSpectrum.push_back(30);
		particle->VelocitySpectrum.push_back(glm::vec3(0, -.2, 0));
		particle->VelocitySpectrum.push_back(glm::vec3(0, -3, 0));*/

		auto model = m_World->AddComponent<Components::Model>(ent, "Model");
		model->ModelFile = "Models/Placeholders/PhysicsTest/PointLight.obj";

		/*auto light = m_World->AddComponent<Components::PointLight>(ent, "PointLight");
		light->Specular = glm::vec3(0.1f, 0.1f, 0.1f);
		light->Diffuse = glm::vec3(1.f, 1.f, 0.f);
		light->constantAttenuation = 0.03f;
		light->linearAttenuation = 0.00009f;
		light->quadraticAttenuation = 0.07f;
		light->spotExponent = 0.0f;*/


		/*auto physics = m_World->AddComponent<Components::Physics>(ent, "Physics");
		physics->Mass = 1;

		auto physicShape = m_World->AddComponent<Components::Box>(ent, "Box");
		physicShape->Width = 0.5;
		physicShape->Height = 0.5;
		physicShape->Depth = 0.5;*/

		ParticleData data;
		data.ParticleID = ent;
		data.SpawnTime = glfwGetTime();
// 		data.color = particle->ColorSpectrum[0];
// 		data.Scale = particle->ScaleSpectrum[0];

		m_ParticleEmitter[emitterID].push_back(data);
	}
}

float Systems::ParticleSystem::RandomizeAngle(float spreadAngle)
{
	return ((float)rand() / ((float)RAND_MAX + 1) * spreadAngle) - spreadAngle/2;
}