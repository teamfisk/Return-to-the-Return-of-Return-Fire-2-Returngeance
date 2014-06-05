#ifndef GameStateSystem_h__
#define GameStateSystem_h__

#include <set>

#include "System.h"
#include "Components/Transform.h"
#include "Components/Player.h"
#include "Events/FlagCaptured.h"
#include "Events/GameOver.h"
#include "Events/PlayBGM.h"

namespace Systems
{

class GameStateSystem : public System
{
public:

	GameStateSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: System(world, eventBroker, resourceManager) 
		, m_InGame(true)
	{ }

	void Initialize() override;

	bool InGame() const { return m_InGame; }

private:
	bool m_InGame;

	EventRelay<GameStateSystem, Events::FlagCaptured> m_EFlagCaptured;
	bool OnFlagCaptured(const Events::FlagCaptured &event);
};

}

#endif // GameStateSystem_h__
