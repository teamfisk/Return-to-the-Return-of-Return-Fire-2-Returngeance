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
	//void OnComponentCreated(std::string type, std::shared_ptr<Component> component) override;

	FMOD::System *m_FmodSystem;

private:
	// Events
	EventRelay<Events::PlaySound> m_EPlaySound;
};

}
#endif // !SoundEmitter_h__

