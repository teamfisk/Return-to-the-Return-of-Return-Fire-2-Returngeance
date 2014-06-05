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

	Events::GameOver e;
	e.Player = event.Player;
	EventBroker->Publish(e);

	return true;
}