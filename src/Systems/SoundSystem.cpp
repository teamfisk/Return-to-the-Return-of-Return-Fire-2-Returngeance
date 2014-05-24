#include "PrecompiledHeader.h"
#include "SoundSystem.h"
#include "World.h"

void Systems::SoundSystem::Initialize()
{
	/*FMOD_RESULT result;
	result = FMOD::System_Create(&m_FmodSystem);
	result = m_FmodSystem->init(32, FMOD_INIT_NORMAL, 0);


	FMOD::Sound *sound;
	FMOD::Channel *channel;
	FMOD_VECTOR pos;
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;
	m_FmodSystem->createSound("Sounds/WUB.mp3", FMOD_HARDWARE, 0, &sound);
	m_FmodSystem->playSound(FMOD_CHANNEL_FREE, sound, false, 0);*/

	FMOD_System_Create(&m_System);
	FMOD_System_Init(m_System, 1024, FMOD_INIT_NORMAL, nullptr);
	FMOD_System_Set3DSettings(m_System, 10.0f, 1.f, 100.f); //dopplerScale, distancefactor, rolloffscale
	//FMOD_System_GetMasterChannelGroup(m_System, &m_ChannelGruoup);
	m_Playing = false;
	
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
	FMOD_System_Update(m_System);
}

void Systems::SoundSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto listener = m_World->GetComponent<Components::Listener>(entity);
	if(listener)
	{
		int lID = std::find(m_Listeners.begin(), m_Listeners.end(), entity) - m_Listeners.begin();
		auto lTransform = m_World->GetComponent<Components::Transform>(entity);
		
		glm::vec3 tPos = lTransform->Position;
		FMOD_VECTOR lPos = {tPos.x, tPos.y, tPos.z}; 

		glm::vec3 tVel = lTransform->Velocity;
		FMOD_VECTOR lVel = {tVel.x, tVel.y, tVel.z};

		glm::mat3 tOri = glm::toMat3(lTransform->Orientation);
		glm::vec3 tUp = tOri[1];
		FMOD_VECTOR lUp = {tUp.x, tUp.y, tUp.z}; 
		glm::vec3 tForward = tOri[2];
		FMOD_VECTOR lForward = {tForward.x, tForward.y, tForward.z};

		FMOD_System_Set3DListenerAttributes(m_System, lID, (const FMOD_VECTOR*)&lPos, (const FMOD_VECTOR*)&lVel, (const FMOD_VECTOR*)&lForward, (const FMOD_VECTOR*)&lUp);
	}

	auto emitter = m_World->GetComponent<Components::SoundEmitter>(entity);
	if(emitter)
	{
		FMOD_CHANNEL* channel = m_Channels[entity];
		FMOD_SOUND* sound = m_Sounds[entity];
		auto eTransform = m_World->GetComponent<Components::Transform>(entity);

		glm::vec3 tPos = eTransform->Position;
		FMOD_VECTOR ePos = {tPos.x, tPos.y, tPos.z};

		glm::vec3 tVel = eTransform->Velocity;
		FMOD_VECTOR eVel = {tVel.x, tVel.y, tVel.z};

		FMOD_Channel_Set3DAttributes(channel, (const FMOD_VECTOR*)&ePos, (const FMOD_VECTOR*)&eVel);

		if(!m_Playing)
		{
			PlaySound(channel, sound, 1, emitter->Loop);
			m_Playing = true;
		}
	}
}

void Systems::SoundSystem::OnComponentCreated(std::string type, std::shared_ptr<Component> component)
{

// 	if(type ==  typeid(Components::Listener).name())
// 	{
// 		m_Listeners.push_back(component->Entity);
// 	}
// 
// 	if (type == typeid(Components::SoundEmitter).name())
// 	{
// 		//std::shared_ptr<Components::SoundEmitter> eComponent = std::dynamic_pointer_cast<Components::SoundEmitter>(component);
// 		
// 		FMOD_CHANNEL* channel;
// 		m_Channels.insert(std::pair<EntityID, FMOD_CHANNEL*>(component->Entity, channel));
// 		
// 		LoadSound("Sounds/DarkHorse.mp3", 100, 30, false, 1, 0);
// 	}
}

void Systems::SoundSystem::OnEntityCommit(EntityID entity)
{
	//TEMP ska slängas in i OnComponentCreated
	auto lComponent = m_World->GetComponent<Components::Listener>(entity);
	if(lComponent)
	{
		m_Listeners.push_back(entity);
	}

	auto eCompoonent = m_World->GetComponent<Components::SoundEmitter>(entity);
	if(eCompoonent)
	{
		FMOD_CHANNEL* channel;
		FMOD_SOUND* sound;

		std::string path = eCompoonent->Path;
		float volume = eCompoonent->Gain;
		bool loop = eCompoonent->Loop;
		float maxDist = eCompoonent->MaxDistance;
		float minDist = eCompoonent->MinDistance;
		float pitch = eCompoonent->Pitch;

		if(LoadSound(sound, path, maxDist, minDist, loop, volume, pitch) != FMOD_OK)
			LOG_ERROR("FMOD did not load sound file");
		m_Channels.insert(std::make_pair(entity, channel));
		m_Sounds.insert(std::make_pair(entity, sound));
	}
}

FMOD_RESULT Systems::SoundSystem::LoadSound(FMOD_SOUND* &sound, std::string filePath, float maxDist, float minDist, bool loop, float volume, float pitch)
{
	FMOD_RESULT result = FMOD_System_CreateSound(m_System, filePath.c_str(), FMOD_3D , 0, &sound);
	FMOD_Sound_Set3DMinMaxDistance(m_Sound, minDist, maxDist);

	return result;
}

void Systems::SoundSystem::PlaySound(FMOD_CHANNEL* channel, FMOD_SOUND* sound, float volume, bool loop)
{
	FMOD_System_PlaySound(m_System, FMOD_CHANNEL_FREE, sound, false, &channel);
	FMOD_Channel_SetVolume(channel, volume);
}