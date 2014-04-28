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
			SpawnParticles(entity);
			emitterComponent->TimeSinceLastSpawn = 0;
		}

		std::list<ParticleData>::iterator it;
		for(it = m_ParticleEmitter[entity].begin(); it != m_ParticleEmitter[entity].end();)
		{
			EntityID particleID = (it)->ParticleID;
			auto transformComponent = m_World->GetComponent<Components::Transform>(particleID, "Transform");
			auto particleComponent = m_World->GetComponent<Components::Particle>(particleID, "Particle");

			double timeLived = glfwGetTime() - it->SpawnTime; 
			if(timeLived > particleComponent->LifeTime)
			{
				m_World->RemoveEntity(particleID);
				it = m_ParticleEmitter[entity].erase(it);
			}
			else
			{
				float timeProgress = timeLived / particleComponent->LifeTime; 
				// The difference between the start and end value
				/*float deltaColor = glm::abs(particleComponent->ColorSpectrum[0].r - particleComponent->ColorSpectrum[1].r); 
				it->color.r = particleComponent->ColorSpectrum[0].r + deltaColor * timeProgress;
				deltaColor = glm::abs(particleComponent->ColorSpectrum[0].g - particleComponent->ColorSpectrum[1].g);
				it->color.g = particleComponent->ColorSpectrum[0].g + deltaColor * timeProgress;
				deltaColor = glm::abs(particleComponent->ColorSpectrum[0].b - particleComponent->ColorSpectrum[1].b);
				it->color.b = particleComponent->ColorSpectrum[0].b + deltaColor * timeProgress;*/

			
				ScaleInterpolation(timeProgress, particleComponent->ScaleSpectrum, transformComponent->Scale);
				VelocityInterpolation(timeProgress, particleComponent->VelocitySpectrum, transformComponent->Velocity);

				transformComponent->Position +=  transformComponent->Velocity * (float)dt;

				it++;
			}
		}
	}
}

void Systems::ParticleSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("ParticleEmitter", []() { return new Components::ParticleEmitter(); });
	cf->Register("Particle", []() { return new Components::Particle(); });
}


void Systems::ParticleSystem::SpawnParticles(EntityID emitterID)
{
	auto emitterComponent = m_World->GetComponent<Components::ParticleEmitter>(emitterID, "ParticleEmitter");
	auto emitterTransform = m_World->GetComponent<Components::Transform>(emitterID, "Transform");
	glm::quat emitterOrientation = emitterTransform->Orientation;

	float tempSpeed =  4;
	glm::vec3 speed = glm::vec3(tempSpeed);

	for(int i = 0; i < emitterComponent->SpawnCount; i++)
	{
		auto ent = m_World->CloneEntity(emitterComponent->ParticleTemplate);

		auto particleTransform = m_World->GetComponent<Components::Transform>(ent, "Transform");
		particleTransform->Position = emitterTransform->Position;
		particleTransform->Scale = glm::vec3(1, 1, 1);

		//The emitter's orientation as "start value" times the default direction for quaternion. Times the speed, and then rotate on x and y axis with the randomized spread angle. 
		float spreadAngle = emitterComponent->SpreadAngle;
		particleTransform->Velocity = emitterOrientation * glm::vec3(0, 0, -1) * speed * 
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(1, 0, 0))) * 
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(0, 1, 0))) *
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(0, 0, 1)));

		glm::vec3 testVel = glm::vec3(particleTransform->Velocity.x, -particleTransform->Velocity.y * 1.5, particleTransform->Velocity.z); //TEMP

		auto particle = m_World->AddComponent<Components::Particle>(ent, "Particle");
		particle->LifeTime = emitterComponent->LifeTime;
		particle->ScaleSpectrum.push_back(4); //TEMP
		particle->ScaleSpectrum.push_back(0); //TEMP
		particle->VelocitySpectrum.push_back(particleTransform->Velocity); //TEMP
		particle->VelocitySpectrum.push_back(testVel); //TEMP
// 		particle->AngularVelocitySpectrum.push_back();
// 		particle->AngularVelocitySpectrum.push_back();

// 		Color startColor = {.4f, .45f, .2f};
// 		particle->ColorSpectrum.push_back(startColor);
// 		Color endColor = {0.f, 45.f, 23.f};
// 		particle->ColorSpectrum.push_back(endColor);

		

		ParticleData data;
		data.ParticleID = ent;
		data.SpawnTime = glfwGetTime();
// 		data.color = particle->ColorSpectrum[0];
// 		data.Scale = particle->ScaleSpectrum[0];

		m_ParticleEmitter[emitterID].push_back(data);
	}
}

//Randomizes between -spreadAngle/2 and spreadAngle/2
float Systems::ParticleSystem::RandomizeAngle(float spreadAngle)
{
	return ((float)rand() / ((float)RAND_MAX + 1) * spreadAngle) - spreadAngle/2;
}

//Interpolates the scale of the particle
void Systems::ParticleSystem::ScaleInterpolation(double timeProgress, std::vector<float> scaleSpectrum, glm::vec3 &scale)
{
	float deltaScale = glm::abs(scaleSpectrum[0] - scaleSpectrum[1]);
	if(scaleSpectrum[0] > scaleSpectrum[1])
		deltaScale *= -1;
	scale = glm::vec3(scaleSpectrum[0] + deltaScale * timeProgress);
}

//Interpolates the velocity of the particle
void Systems::ParticleSystem::VelocityInterpolation(double timeProgress, std::vector<glm::vec3> velocitySpectrum, glm::vec3 &velocity)
{
	float deltaVelocity = glm::abs(velocitySpectrum[0].x - velocitySpectrum[1].x);
	if(velocitySpectrum[0].x > velocitySpectrum[1].x)
		deltaVelocity *= -1;
	velocity.x = velocitySpectrum[0].x + deltaVelocity * timeProgress;
	deltaVelocity = glm::abs(velocitySpectrum[0].y - velocitySpectrum[1].y);
	if (velocitySpectrum[0].y > velocitySpectrum[1].y)
		deltaVelocity *= -1;
	velocity.y = velocitySpectrum[0].y + deltaVelocity * timeProgress;	
	deltaVelocity = glm::abs(velocitySpectrum[0].z - velocitySpectrum[1].z);
	if(velocitySpectrum[0].z > velocitySpectrum[1].z)
		deltaVelocity *= -1;
	velocity.z = velocitySpectrum[0].z + deltaVelocity * timeProgress;
}