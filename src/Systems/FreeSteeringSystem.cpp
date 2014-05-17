#include "PrecompiledHeader.h"
#include "FreeSteeringSystem.h"
#include "World.h"

void Systems::FreeSteeringSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::FreeSteering>([]() { return new Components::FreeSteering(); });
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
	auto steering = m_World->GetComponent<Components::FreeSteering>(entity);
	if (steering)
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity);

		glm::vec3 cameraRight = glm::vec3(transform->Orientation * glm::vec4(1, 0, 0, 0));
		glm::vec3 cameraForward = glm::vec3(transform->Orientation * glm::vec4(0, 0, -1, 0));
		glm::vec3 movement;
		movement += cameraRight * m_InputController->Movement.x;
		movement.y += m_InputController->Movement.y;
		movement += cameraForward * -m_InputController->Movement.z;
		float speedMultiplier = 1.f;
		if (m_InputController->SpeedMultiplier > 0)
			speedMultiplier *= 4;
		else if (m_InputController->SpeedMultiplier < 0)
			speedMultiplier /= 4;

		transform->Position += movement * steering->Speed * speedMultiplier * (float)dt;

		glm::quat mouseOrientationPitch = glm::quat(m_InputController->MouseOrientation * glm::vec3(1, 0, 0));
		glm::quat mouseOrientationYaw = glm::quat(m_InputController->MouseOrientation * glm::vec3(0, 1, 0));

		glm::vec3 controllerOrientationEuler = m_InputController->ControllerOrientation * (float)dt;
		glm::quat controllerOrientationPitch = glm::quat(controllerOrientationEuler * glm::vec3(1, 0, 0));
		glm::quat controllerOrientationYaw = glm::quat(controllerOrientationEuler * glm::vec3(0, 1, 0));

		// TOUCHING THIS CODE MIGHT CAUSE THE UNIVERSE TO IMPLODE, ALSO DRAGONS
		//---------------------------------------------------------------------
		transform->Orientation = (mouseOrientationYaw * controllerOrientationYaw)
			* transform->Orientation 
			* (mouseOrientationPitch * controllerOrientationPitch);
		//---------------------------------------------------------------------
		// TOUCHING THIS CODE MIGHT CAUSE THE UNIVERSE TO IMPLODE, ALSO DRAGONS
	}

	m_InputController->MouseOrientation = glm::vec3(0);
}

bool Systems::FreeSteeringSystem::FreeSteeringInputController::OnCommand(const Events::InputCommand &event)
{
	// Movement
	if (event.Command == "vertical")
	{
		Movement.z = -event.Value;
	}
	else if (event.Command == "horizontal")
	{
		Movement.x = event.Value;
	}
	else if (event.Command == "normal")
	{
		Movement.y = event.Value;
	}

	// Speed
	else if (event.Command == "speed")
	{
		SpeedMultiplier = event.Value;
	}

	// Mouse click
	else if (event.Command == "attack")
	{
		OrientationActive = event.Value > 0;

		if (OrientationActive)
		{
			Events::LockMouse e;
			EventBroker->Publish(e);
		}
		else
		{
			Events::UnlockMouse e;
			EventBroker->Publish(e);
		}
	}

	else if (event.Command == "vertical2")
	{
		ControllerOrientation.x = event.Value;
	}
	else if (event.Command == "horizontal2")
	{
		ControllerOrientation.y = -event.Value;
	}

	return true;
}

bool Systems::FreeSteeringSystem::FreeSteeringInputController::OnMouseMove(const Events::MouseMove &event)
{
	if (OrientationActive)
	{
		MouseOrientation = -glm::vec3(event.DeltaY / 300.f, event.DeltaX / 300.f, 0.f);
	}

	return true;
}
