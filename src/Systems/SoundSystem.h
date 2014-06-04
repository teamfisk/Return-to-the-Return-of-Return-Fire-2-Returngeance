#ifndef SoundEmitter_h__
#define SoundEmitter_h__
#include <fmod.hpp>
#include <fmod_errors.h>
#include <vector>

#include "System.h"
#include "Systems/TransformSystem.h"
#include "Components/Transform.h"
#include "Components/SoundEmitter.h"
#include "Components/Listener.h"
#include "Events/ComponentCreated.h"
#include "Events/PlaySFX.h"
#include "Events/PlayBGM.h"
#include "Events/StopSound.h"
#include "Sound.h"

namespace Systems
{

class SoundSystem : public System
{
public:
	SoundSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: System(world, eventBroker, resourceManager)
	{ }
	~SoundSystem();
	void RegisterComponents(ComponentFactory* cf) override;
	void RegisterResourceTypes(std::shared_ptr<::ResourceManager> rm) override;
	void Initialize() override;
	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	void OnComponentRemoved(EntityID entity, std::string type, Component* component) override;
	FMOD_SYSTEM* GetFMODSystem() const { return m_System; }
private:
	// Events
	EventRelay<SoundSystem, Events::ComponentCreated> m_EComponentCreated;
	bool OnComponentCreated(const Events::ComponentCreated &event);
	EventRelay<SoundSystem, Events::PlaySFX> m_EPlaySFX;
	bool PlaySFX(const Events::PlaySFX &event);
	EventRelay<SoundSystem, Events::PlayBGM> m_EPlayBGM;
	bool PlayBGM(const Events::PlayBGM &event);
	EventRelay<SoundSystem, Events::StopSound> m_EStopSound;
	bool StopSound(const Events::StopSound &event);

	void LoadSound(FMOD_SOUND*&, std::string, float, float);
	void PlaySound(FMOD_CHANNEL**, FMOD_SOUND*, float volume, bool loop);
	
	FMOD_SYSTEM* m_System;
	FMOD_BOOL m_isPlaying;
	FMOD_CHANNEL* m_BGMChannel;
	std::vector<EntityID> m_Listeners;
	std::map<EntityID, FMOD_CHANNEL*> m_Channels;
	std::map<EntityID, FMOD_CHANNEL*> m_DeleteChannels;
	std::map<EntityID, FMOD_SOUND*> m_DeleteSounds;	
	std::shared_ptr<Systems::TransformSystem> m_TransformSystem;
	
};

}
#endif // !SoundEmitter_h__

