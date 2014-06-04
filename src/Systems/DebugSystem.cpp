#include "PrecompiledHeader.h"
#include "DebugSystem.h"
#include "World.h"


void Systems::DebugSystem::Initialize()
{
	// Subscribe to events
	m_EKeyDown = decltype(m_EKeyDown)(std::bind(&Systems::DebugSystem::OnKeyDown, this, std::placeholders::_1));
	EventBroker->Subscribe(m_EKeyDown);
}

void Systems::DebugSystem::Update(double dt)
{

}

bool Systems::DebugSystem::OnKeyDown(const Events::KeyDown &event)
{
	if (event.KeyCode == GLFW_KEY_ENTER)
	{
		Events::PlaySFX e;
		e.Position = glm::vec3(0);
		e.Resource = "Sounds/korvring.wav";
		EventBroker->Publish<Events::PlaySFX>(e);

		return true;
	}

	return false;
}
