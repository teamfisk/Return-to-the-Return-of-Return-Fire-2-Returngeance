#include "PrecompiledHeader.h"
#include "ParticleSystem.h"


#include "World.h"

void Systems::ParticleSystem::Initialize()
{
	m_TransformSystem = m_World->GetSystem<Systems::TransformSystem>();
	tempSpawnedExplosions = false;
	EVENT_SUBSCRIBE_MEMBER(m_EExplosion, &ParticleSystem::CreateExplosion);
}

void Systems::ParticleSystem::Update(double dt)
{
	std::map<EntityID, double>::iterator it;
	for(it = m_ExplosionEmitters.begin(); it != m_ExplosionEmitters.end();)
	{
		EntityID explosionID = it->first;
		double spawnTime = it->second;
		
		double timeLived = glfwGetTime() - spawnTime;
		auto eComp = m_World->GetComponent<Components::ParticleEmitter>(explosionID);

		if(timeLived > eComp->LifeTime && m_ParticlesToEmitter[explosionID] == NULL)
		{
			auto e = m_World->GetComponent<Components::ParticleEmitter>(explosionID);
			m_World->RemoveEntity(e->ParticleTemplate);
			m_World->RemoveEntity(explosionID);
			it = m_ExplosionEmitters.erase(it);
			//LOG_INFO("Deleted explosion emitter successfully");
			//LOG_INFO("Deleted explosion emitter successfully. nr of emitters%i", m_ExplosionEmitters.size());
		}
		else 
		{
			it++;
		}
	}
}

void Systems::ParticleSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity);
	if(!transformComponent)
		return;

	auto emitterComponent = m_World->GetComponent<Components::ParticleEmitter>(entity);
	if(emitterComponent)
	{
		emitterComponent->TimeSinceLastSpawn += dt;
		auto emitterTransformComponent = m_World->GetComponent<Components::Transform>(entity);
		if(emitterComponent->Emitting)
		{
			if(emitterComponent->TimeSinceLastSpawn > emitterComponent->SpawnFrequency)
			{
				SpawnParticles(entity);
				emitterComponent->TimeSinceLastSpawn = 0;
			}
		}
		
	}

	auto particleComponent = m_World->GetComponent<Components::Particle>(entity);
	if(particleComponent)
	{
		

		EntityID particleID = entity;
		double timeLived = glfwGetTime() - particleComponent->SpawnTime; 
		
		if(timeLived > particleComponent->LifeTime)
		{
			m_World->RemoveEntity(particleID);
			m_ParticlesToEmitter.erase(particleID);
			
			return;
		}
		else
		{
			auto transformComponent = m_World->GetComponent<Components::Transform>(particleID);
			auto eComponent = m_World->GetComponent<Components::ParticleEmitter>(m_ParticlesToEmitter[particleID]);
			auto sprite = m_World->GetComponent<Components::Sprite>(entity);
			// FIX: calculate once
			float timeProgress = timeLived / particleComponent->LifeTime; 
			// ColorInterpolation(timeProgress, particleComponent->ColorSpectrum, color);
			// Scale interpolation
			if(particleComponent->ScaleSpectrum.size() > 1)
				VectorInterpolation(timeProgress, particleComponent->ScaleSpectrum, transformComponent->Scale);
			// Velocity interpolation
			if(particleComponent->VelocitySpectrum.size() > 1)
				VectorInterpolation(timeProgress, particleComponent->VelocitySpectrum, transformComponent->Velocity);
			
			if(particleComponent->Fade == true)
			{
				std::vector<float> spectrum;
				spectrum.push_back(1);
				spectrum.push_back(0);
				float alpha;
				ScalarInterpolation(timeProgress, spectrum, alpha);
				sprite->Color.w = alpha;
			}



			/*// Angular velocity interpolation
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
			}*/
			transformComponent->Position += transformComponent->Velocity * (float)dt;
		}
	}
}

void Systems::ParticleSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::ParticleEmitter>([]() { return new Components::ParticleEmitter(); });
	cf->Register<Components::Particle>([]() { return new Components::Particle(); });
}


void Systems::ParticleSystem::SpawnParticles(EntityID emitterID)
{
	auto eComponent = m_World->GetComponent<Components::ParticleEmitter>(emitterID);
	auto eTransform = m_World->GetComponent<Components::Transform>(emitterID);
	glm::vec3 ePosition = m_TransformSystem->AbsolutePosition(emitterID);
	glm::quat eOrientation = eTransform->Orientation;
	glm::vec3 paticleSpeed = glm::vec3(eComponent->Speed);
	for(int i = 0; i < eComponent->SpawnCount; i++)
	{
		auto ent = m_World->CloneEntity(eComponent->ParticleTemplate);
		m_ParticlesToEmitter.insert(std::make_pair(ent, emitterID));
		auto particleTransform = m_World->GetComponent<Components::Transform>(ent);
		particleTransform->Position = ePosition;
		
		particleTransform->Orientation = eOrientation;
		//The emitter's orientation as "start value" times the default direction for emitter. Times the speed, and then rotate on x and y axis with the randomized spread angle. 
		float spreadAngle = eComponent->SpreadAngle;
		particleTransform->Velocity = eOrientation * glm::vec3(0, 0, -1) * paticleSpeed * 
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(1, 0, 0))) * 
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(0, 1, 0))) *
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(0, 0, 1)));

		auto particle = m_World->AddComponent<Components::Particle>(ent);
		particle->LifeTime = eComponent->LifeTime;
		particle->ScaleSpectrum = eComponent->ScaleSpectrum; 
		particle->VelocitySpectrum.push_back(particleTransform->Velocity);
		particle->Fade = eComponent->Fade;
		
		auto sprite = m_World->GetComponent<Components::Sprite>(ent);
		if(eComponent->Color != glm::vec4(0))
			sprite->Color = eComponent->Color;
		
		if (eComponent->ScaleSpectrum.size() > 0)
		{
			if (eComponent->ScaleSpectrum.size() > 1)
			{
				particle->ScaleSpectrum = eComponent->ScaleSpectrum;
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
			particle->VelocitySpectrum.push_back(eComponent->GoalVelocity);
		particle->OrientationSpectrum = eComponent->OrientationSpectrum;
		if(particle->OrientationSpectrum.size() != 0)
			particleTransform->Orientation = glm::angleAxis(0.f, particle->OrientationSpectrum[0]);
		particle->AngularVelocitySpectrum = eComponent->AngularVelocitySpectrum;
		

		particle->SpawnTime = glfwGetTime();
// 		if (particle->AngularVelocitySpectrum.size() != 0)
// 			data.AngularVelocity = particle->AngularVelocitySpectrum[0];
// 		if (particle->OrientationSpectrum.size() != 0)
// 			data.Orientation = particle->OrientationSpectrum[0];
// 		else data.Orientation = eOrientation * glm::vec3(0,0,-1);
		//m_ParticleEmitter[emitterID].push_back(data);
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

void Systems::ParticleSystem::ScalarInterpolation(double timeProgress, std::vector<float> spectrum, float &alpha)
{
	float dAlpha = glm::abs(spectrum[0] - spectrum[1]);
	if(spectrum[0] > spectrum[1])
		dAlpha *= -1;
	alpha = spectrum[0] + dAlpha * timeProgress; 
}

bool Systems::ParticleSystem::CreateExplosion(const Events::CreateExplosion &e)
{
	auto explosion = m_World->CreateEntity();
	auto emitter = m_World->AddComponent<Components::ParticleEmitter>(explosion);
	emitter->LifeTime = e.LifeTime;
	emitter->SpawnCount = e.ParticlesToSpawn;
	emitter->Speed = e.Speed;
	emitter->SpreadAngle = e.SpreadAngle; 
	emitter->Emitting = e.Emitting;
	emitter->SpawnFrequency = e.SpawnFrequency;
	//emitter->ScaleSpectrum.push_back(glm::vec3(e.ParticleScale));
	emitter->SpawnFrequency = e.LifeTime + 20; //temp
	emitter->Fade = true;
	emitter->Color = e.Color;
	if(e.UseGoalVector)
	{
		emitter->UseGoalVelocity = e.UseGoalVector;
		emitter->GoalVelocity = e.GoalVelocity;
	}
	std::vector<glm::vec3> scale;
	scale.push_back(glm::vec3(e.ParticleScale[0]));
	if(e.ParticleScale.size() >= 2)
		scale.push_back(glm::vec3(e.ParticleScale[1]));
	emitter->ScaleSpectrum = scale;
	// 	emitter->GoalVelocity = glm::vec3(0,-_speed, 0);
	m_World->CommitEntity(explosion);

	auto particleEnt = m_World->CreateEntity();
	auto templateComponent = m_World->AddComponent<Components::Template>(particleEnt);
	auto TEMP = m_World->AddComponent<Components::Transform>(particleEnt);
	auto spriteComponent = m_World->AddComponent<Components::Sprite>(particleEnt);
	if(e.usePointLight)
	{
		auto light = m_World->AddComponent<Components::PointLight>(particleEnt);
		light->Diffuse = glm::vec3(231.f/255.f, 181.f/255.f, 48.f/255.f);
		light->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
		light->Radius = 20.f;
	}
	spriteComponent->SpriteFile = e.spritePath;
	m_World->CommitEntity(particleEnt);
	emitter->ParticleTemplate = particleEnt;

	auto transform = m_World->AddComponent<Components::Transform>(explosion);
	transform->Position = e.Position;
	transform->Orientation = e.RelativeUpOrientation;

	SpawnParticles(explosion);
	m_ExplosionEmitters[explosion] = glfwGetTime();

	return true;
}