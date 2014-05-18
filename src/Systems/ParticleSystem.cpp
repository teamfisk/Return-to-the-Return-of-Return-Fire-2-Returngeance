#include "PrecompiledHeader.h"
#include "ParticleSystem.h"


#include "World.h"

void Systems::ParticleSystem::Initialize()
{
	m_TransformSystem = m_World->GetSystem<Systems::TransformSystem>("TransformSystem");
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
		auto emitterTransformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
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
				// FIX: calculate once
				float timeProgress = timeLived / particleComponent->LifeTime; 
				// ColorInterpolation(timeProgress, particleComponent->ColorSpectrum, color);
				// Scale interpolation
				if(particleComponent->ScaleSpectrum.size() > 1)
					VectorInterpolation(timeProgress, particleComponent->ScaleSpectrum, transformComponent->Scale);
				// Velocity interpolation
				if(particleComponent->VelocitySpectrum.size() > 1)
					VectorInterpolation(timeProgress, particleComponent->VelocitySpectrum, transformComponent->Velocity);
				
				// Angular velocity interpolation
				if (particleComponent->AngularVelocitySpectrum.size() != 0)
				{
					if(particleComponent->AngularVelocitySpectrum.size() > 1)
					{
						ScalarInterpolation(timeProgress, particleComponent->AngularVelocitySpectrum, it->AngularVelocity);
						transformComponent->Orientation = glm::angleAxis(it->AngularVelocity, it->Orientation);
					}
					else
					{
						transformComponent->Orientation *= glm::angleAxis(it->AngularVelocity, it->Orientation);
						//it->Orientation =  glm::angleAxis(it->AngularVelocity, it->Orientation);
					}
				}
				
				//Angular velocity interpolation
				if(particleComponent->OrientationSpectrum.size() > 1)
				{
					VectorInterpolation(timeProgress, particleComponent->OrientationSpectrum, it->Orientation);
					glm::vec3 v1 = (particleComponent->OrientationSpectrum[0]);
					glm::vec3 v2 = (it->Orientation);
					glm::vec3 v3 = glm::normalize(glm::cross(v1,v2));
					float angle = glm::acos(glm::dot(v1, v2) / (glm::length(v1) * glm::length(v2)));

					transformComponent->Orientation = glm::angleAxis(angle, v3);
				}
				
				
				transformComponent->Position += transformComponent->Velocity * (float)dt;

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
	auto eComponent = m_World->GetComponent<Components::ParticleEmitter>(emitterID, "ParticleEmitter");
	auto eTransform = m_World->GetComponent<Components::Transform>(emitterID, "Transform");
	glm::vec3 ePosition = m_TransformSystem->AbsolutePosition(emitterID);
	glm::quat eOrientation = eTransform->Orientation;

	glm::vec3 paticleSpeed = glm::vec3(eComponent->Speed);

	for(int i = 0; i < eComponent->SpawnCount; i++)
	{
		auto particleEntity = m_World->CloneEntity(eComponent->ParticleTemplate);

		auto particleTransform = m_World->GetComponent<Components::Transform>(particleEntity, "Transform");
		particleTransform->Position = ePosition;

		particleTransform->Orientation = eOrientation;
		//The emitter's orientation as "start value" times the default direction for emitter. Times the speed, and then rotate on x and y axis with the randomized spread angle. 
		float spreadAngle = eComponent->SpreadAngle;
		particleTransform->Velocity = eOrientation * glm::vec3(0, 0, -1) * paticleSpeed * 
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(1, 0, 0))) * 
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(0, 1, 0))) *
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(0, 0, 1)));

		auto particleComponent = m_World->AddComponent<Components::Particle>(particleEntity, "Particle");
		particleComponent->LifeTime = eComponent->LifeTime;
		particleComponent->ScaleSpectrum = eComponent->ScaleSpectrum; 
		particleComponent->VelocitySpectrum.push_back(particleTransform->Velocity);
		
		if (eComponent->ScaleSpectrum.size() > 0)
		{
			if (eComponent->ScaleSpectrum.size() > 1)
			{
				particleComponent->ScaleSpectrum = eComponent->ScaleSpectrum;
			}
			else
			{
				particleTransform->Scale = eComponent->ScaleSpectrum[0];
			}
		}
		else
		{
			particleTransform->Scale = glm::vec3(1, 1, 1);
		}
		
		if(eComponent->UseGoalVelocity)
			particleComponent->VelocitySpectrum.push_back(eComponent->GoalVelocity);
		particleComponent->OrientationSpectrum = eComponent->OrientationSpectrum;
		if(particleComponent->OrientationSpectrum.size() != 0)
			particleTransform->Orientation = glm::angleAxis(0.f, particleComponent->OrientationSpectrum[0]);
		particleComponent->AngularVelocitySpectrum = eComponent->AngularVelocitySpectrum;
		

		ParticleData data;
		data.ParticleID = particleEntity;
		data.SpawnTime = glfwGetTime();
		if (particleComponent->AngularVelocitySpectrum.size() != 0)
			data.AngularVelocity = particleComponent->AngularVelocitySpectrum[0];
		if (particleComponent->OrientationSpectrum.size() != 0)
			data.Orientation = particleComponent->OrientationSpectrum[0];
		else data.Orientation = eOrientation * glm::vec3(0,0,-1);
		m_ParticleEmitter[emitterID].push_back(data);
	}
}

//Randomizes between -spreadAngle/2 and spreadAngle/2
float Systems::ParticleSystem::RandomizeAngle(float spreadAngle)
{
	return ((float)rand() / ((float)RAND_MAX + 1) * spreadAngle) - spreadAngle/2;
}

//Interpolates the velocity of the particle
void Systems::ParticleSystem::VectorInterpolation(double timeProgress, std::vector<glm::vec3> spectrum, glm::vec3 &v)
{
	float dAxisValue = glm::abs(spectrum[0].x - spectrum[1].x);
	if(spectrum[0].x > spectrum[1].x)
		dAxisValue *= -1;
	v.x = spectrum[0].x + dAxisValue * timeProgress;
	dAxisValue = glm::abs(spectrum[0].y - spectrum[1].y);
	if (spectrum[0].y > spectrum[1].y)
		dAxisValue *= -1;
	v.y = spectrum[0].y + dAxisValue * timeProgress;	
	dAxisValue = glm::abs(spectrum[0].z - spectrum[1].z);
	if(spectrum[0].z > spectrum[1].z)
		dAxisValue *= -1;
	v.z = spectrum[0].z + dAxisValue * timeProgress;
}

// void Systems::ParticleSystem::ColorInterpolation(double timeProgress, std::vector<Color> spectrum, Color &c)
// {
// 	float dColor = glm::abs(spectrum[0].r - spectrum[1].r); 
// 	c.r = spectrum[0].r + dColor * timeProgress;
// 	dColor = glm::abs(spectrum[0].g - spectrum[1].g);
// 	c.g = spectrum[0].g + dColor * timeProgress;
// 	dColor = glm::abs(spectrum[0].b - spectrum[1].b);
// 	c.b = spectrum[0].b + dColor * timeProgress;
// }

void Systems::ParticleSystem::ScalarInterpolation(double timeProgress, std::vector<float> spectrum, float &alpha)
{
	float dAlpha = glm::abs(spectrum[0] - spectrum[1]);
	if(spectrum[0] > spectrum[1])
		dAlpha *= -1;
	alpha = spectrum[0] + dAlpha * timeProgress; 
}