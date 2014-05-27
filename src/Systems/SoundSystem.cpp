#include "PrecompiledHeader.h"
#include "SoundSystem.h"
#include "World.h"

void Systems::SoundSystem::Initialize()
{
	//initialize OpenAL
	ALCdevice* Device = alcOpenDevice(NULL);
	ALCcontext* context;
	if(Device)
	{
		context = alcCreateContext(Device, NULL);
		alcMakeContextCurrent(context);
	}
	else
	{
		LOG_ERROR("OMG OPEN AL FAIL");
	}

	alGetError();

	alSpeedOfSound(340.29f); // Speed of sound
	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

	// Subscribe to events
	m_EPlaySound = decltype(m_EPlaySound)(std::bind(&Systems::SoundSystem::OnPlaySound, this, std::placeholders::_1));
	EventBroker->Subscribe(m_EPlaySound);
}

void Systems::SoundSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::SoundEmitter>([]() { return new Components::SoundEmitter(); });
}

void Systems::SoundSystem::RegisterResourceTypes(ResourceManager* rm)
{
	rm->RegisterType("Sound", [](std::string resourceName) { return new Sound(resourceName); });
}

void Systems::SoundSystem::Update(double dt)
{

}

void Systems::SoundSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity);
	if (transformComponent == nullptr)
		return;

	auto entityName = m_World->GetProperty<std::string>(entity, "Name");
	if (entityName == "Camera")
	{
		glm::vec3 playerPos = transformComponent->Position;
		ALfloat listenerPos[3] = { playerPos.x, playerPos.y, -playerPos.z };

		glm::vec3 playerVel = transformComponent->Velocity;
		ALfloat listenerVel[3] = { playerVel.x, playerVel.y, -playerVel.z };

		glm::fquat playerQuatOri = transformComponent->Orientation;
		glm::vec3 playerOriFW = glm::rotate(playerQuatOri, glm::vec3(0, 0, -1));
		glm::vec3 playerOriUP = glm::rotate(playerQuatOri, glm::vec3(0, 1, 0));
		ALfloat listenerOri[6] = { playerOriFW.x, playerOriFW.y, playerOriFW.z, playerOriUP.x, playerOriUP.y, playerOriUP.z };

		//Listener
		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVel);
		alListenerfv(AL_ORIENTATION, listenerOri);
	}

	auto soundEmitter = m_World->GetComponent<Components::SoundEmitter>(entity);
	if(soundEmitter != nullptr)
	{
		ALuint source = m_Sources[soundEmitter];
		alSourcef(source, AL_GAIN, soundEmitter->Gain);
		//alSourcef(source, AL_MAX_DISTANCE, soundEmitter->MaxDistance);
		alSourcef(source, AL_REFERENCE_DISTANCE, soundEmitter->ReferenceDistance);
		alSourcef(source, AL_PITCH, soundEmitter->Pitch);
		alSourcei(source, AL_LOOPING, soundEmitter->Loop);

		glm::vec3 emitterPos = transformComponent->Position;
		ALfloat sourcePos[3] = { emitterPos.x, emitterPos.y, -emitterPos.z };

		glm::vec3 emitterVel= transformComponent->Velocity;
		ALfloat sourceVel[3] = { emitterVel.x, emitterVel.y, -emitterVel.z };

		alSourcefv(source, AL_POSITION, sourcePos);
		alSourcefv(source, AL_VELOCITY, sourceVel);
	}
}

void Systems::SoundSystem::PlaySound(Components::SoundEmitter* emitter, std::string fileName)
{
	if (m_Sources.find(emitter) == m_Sources.end())
		return;

	ALuint buffer = *m_World->GetResourceManager()->Load<Sound>("Sound", fileName);
	if (buffer == 0)
		return;
	ALuint source = m_Sources[emitter];
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(m_Sources[emitter]);
}

void Systems::SoundSystem::PlaySound(std::shared_ptr<Components::SoundEmitter> emitter)
{
	ALuint buffer = *m_World->GetResourceManager()->Load<Sound>("Sound", emitter->Path);
	ALuint source = m_Sources[emitter.get()];
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(m_Sources[emitter.get()]);
}

void Systems::SoundSystem::StopSound(std::shared_ptr<Components::SoundEmitter> emitter)
{
	alSourceStop(m_Sources[emitter.get()]);
}

void Systems::SoundSystem::OnComponentCreated(std::string type, std::shared_ptr<Component> component)
{
	if(type == "SoundEmitter")
	{
		ALuint source = CreateSource();
		m_Sources[component.get()] = source;
	}
}

void Systems::SoundSystem::OnComponentRemoved(EntityID entity, std::string type, Component* component)
{
	if(type == "SoundEmitter")
	{
		if (m_Sources.find(component) != m_Sources.end())
		{
			ALuint source = m_Sources[component];
			alDeleteSources(1, &source);
		}
	}
}

ALuint Systems::SoundSystem::CreateSource()
{
	ALuint source;
	alGenSources((ALuint)1, &source);

	alDopplerFactor(1); // Numbers greater than 1 will increase Doppler effect, numbers lower than 1 will decrease the Doppler effect
	alDopplerVelocity(350.f); // Defines the velocity of the sound

	return source;
}

bool Systems::SoundSystem::OnPlaySound(const Events::PlaySound &event)
{
	LOG_DEBUG("Events::PlaySound.Resource = %s", event.Resource.c_str());

	ALuint buffer = *m_World->GetResourceManager()->Load<Sound>("Sound", event.Resource);
	ALuint source = m_Sources.begin()->second;
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);

	return true;
}
