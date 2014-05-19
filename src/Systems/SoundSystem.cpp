#include "PrecompiledHeader.h"
#include "SoundSystem.h"
#include "World.h"

void Systems::SoundSystem::Initialize()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_FmodSystem);
	result = m_FmodSystem->init(32, FMOD_INIT_NORMAL, 0);


	FMOD::Sound *sound;
	FMOD::Channel *channel;
	FMOD_VECTOR pos;
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;
	m_FmodSystem->createSound("Sounds/WUB.mp3", FMOD_HARDWARE, 0, &sound);
	m_FmodSystem->playSound(FMOD_CHANNEL_FREE, sound, false, 0);
}

void Systems::SoundSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::SoundEmitter>([]() { return new Components::SoundEmitter(); });
	cf->Register<Components::Listener>([]() { return new Components::Listener(); });
}

void Systems::SoundSystem::RegisterResourceTypes(ResourceManager* rm)
{
	rm->RegisterType("Sound", [](std::string resourceName) { return new Sound(resourceName); });
}

void Systems::SoundSystem::Update(double dt)
{
	m_FmodSystem->update();
}

void Systems::SoundSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto listener = m_World->GetComponent<Components::Listener>(entity);
	if(listener)
	{
		
	}
}
