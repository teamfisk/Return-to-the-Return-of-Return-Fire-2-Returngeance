#include "PrecompiledHeader.h"
#include "FreeSteeringSystem.h"
#include "World.h"

void Systems::FreeSteeringSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("FreeSteering", []() { return new Components::FreeSteering(); });
}

void Systems::FreeSteeringSystem::Initialize()
{
	m_InputController = std::unique_ptr<FreeSteeringInputController>(new FreeSteeringInputController(EventBroker));
}

void Systems::FreeSteeringSystem::Update(double dt)
{
	
}

void Systems::FreeSteeringSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto steering = m_World->GetComponent<Components::FreeSteering>(entity, "FreeSteering");
	if (steering)
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity, "Transform");

		glm::vec3 cameraRight = glm::vec3(glm::vec4(1, 0, 0, 0) * m_InputController->Orientation);
		glm::vec3 cameraForward = glm::vec3(glm::vec4(0, 0, -1, 0) * m_InputController->Orientation);
		glm::vec3 movement;
		movement += cameraRight * m_InputController->Movement.x;
		movement.y += m_InputController->Movement.y;
		movement += cameraForward * -m_InputController->Movement.z;
		transform->Position += movement * steering->Speed * m_InputController->SpeedMultiplier * (float)dt;
		transform->Orientation = m_InputController->Orientation;
	}
}

bool Systems::FreeSteeringSystem::FreeSteeringInputController::OnCommand(const Events::InputCommand &event)
{
	// Movement
	if (event.Command == "+forward")
	{
		Movement.z += -1.f;
	}
	else if (event.Command == "-forward")
	{
		Movement.z -= -1.f;
	}
	else if (event.Command == "+backward")
	{
		Movement.z += 1.f;
	}
	else if (event.Command == "-backward")
	{
		Movement.z -= 1.f;
	}
	else if (event.Command == "+right")
	{
		Movement.x += 1.f;
	}
	else if (event.Command == "-right")
	{
		Movement.x -= 1.f;
	}
	else if (event.Command == "+left")
	{
		Movement.x += -1.f;
	}
	else if (event.Command == "-left")
	{
		Movement.x -= -1.f;
	}
	else if (event.Command == "+up")
	{
		Movement.y += 1.f;
	}
	else if (event.Command == "-up")
	{
		Movement.y -= 1.f;
	}
	else if (event.Command == "+down")
	{
		Movement.y += -1.f;
	}
	else if (event.Command == "-down")
	{
		Movement.y -= -1.f;
	}

	// Speed
	else if (event.Command == "+fast")
	{
		SpeedMultiplier *= 4.f;
	}
	else if (event.Command == "-fast")
	{
		SpeedMultiplier /= 4.f;
	}
	else if (event.Command == "+slow")
	{
		SpeedMultiplier /= 4.f;
	}
	else if (event.Command == "-slow")
	{
		SpeedMultiplier *= 4.f;
	}

	// Mouse click
	else if (event.Command == "+attack")
	{
		OrientationActive = true;
	}
	else if (event.Command == "-attack")
	{
		OrientationActive = false;
	}

	return true;
}

bool Systems::FreeSteeringSystem::FreeSteeringInputController::OnMouseMove(const Events::MouseMove &event)
{
	if (OrientationActive)
	{
		// TOUCHING THIS CODE MIGHT CAUSE THE UNIVERSE TO IMPLODE, ALSO DRAGONS
		//---------------------------------------------------------------------
		Orientation = glm::angleAxis<float>(event.DeltaY / 300.f, glm::vec3(1, 0, 0)) * Orientation * glm::angleAxis<float>(event.DeltaX / 300.f, glm::vec3(0, 1, 0));
		//---------------------------------------------------------------------
		// TOUCHING THIS CODE MIGHT CAUSE THE UNIVERSE TO IMPLODE, ALSO DRAGONS
	}

	return true;
}
