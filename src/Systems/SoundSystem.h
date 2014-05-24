#ifndef SoundEmitter_h__
#define SoundEmitter_h__
#include <fmod.hpp>
#include <fmod_errors.h>
#include <vector>

#include "System.h"
#include "Components/Transform.h"
#include "Components/SoundEmitter.h"
#include "Components/Listener.h"
#include "Events/PlaySound.h"
#include "Sound.h"

namespace Systems
{

class SoundSystem : public System
{
public:
	SoundSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
		: System(world, eventBroker) { }

	void RegisterComponents(ComponentFactory* cf) override;
	void RegisterResourceTypes(ResourceManager* rm) override;
	void Initialize() override;
	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	void OnComponentCreated(std::string type, std::shared_ptr<Component> component) override;
	void OnEntityCommit(EntityID entity) override;

	

private:
	// Events
	EventRelay<SoundSystem, Events::PlaySound> m_EPlaySound;

	FMOD_RESULT LoadSound(FMOD_SOUND*&, std::string, float, float, bool, float, float);
	void PlaySound(FMOD_CHANNEL*, FMOD_SOUND*, float volume, bool loop);
	void RemoveEmitter(EntityID ent);
	std::vector<EntityID> m_Listeners;
	FMOD_SYSTEM* m_System;
	FMOD_CHANNELGROUP* m_ChannelGruoup;
	//std::map<EntityID, FMOD_CHANNEL*> m_Channels;
	std::map<EntityID, FMOD_CHANNEL*> m_Channels;
	std::map<EntityID, FMOD_SOUND*> m_Sounds;
	FMOD::System *m_FmodSystem;
	
	FMOD_SOUND* m_Sound; //Temp
	bool m_Playing; //TEMP
};

}
#endif // !SoundEmitter_h__

