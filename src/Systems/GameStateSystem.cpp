#include "PrecompiledHeader.h"
#include "GameStateSystem.h"
#include "World.h"

void Systems::GameStateSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_EFlagCaptured, &Systems::GameStateSystem::OnFlagCaptured);
}

bool Systems::GameStateSystem::OnFlagCaptured(const Events::FlagCaptured &event)
{
	m_InGame = false;

	Events::GameOver e1;
	e.Player = event.Player;
	EventBroker->Publish(e1);
	Events::PlayBGM e2;
	e2.Resource = "Sounds/BGM/WilliamTellOverture.mp3";
	EventBroker->Publish(e2);

	return true;
}