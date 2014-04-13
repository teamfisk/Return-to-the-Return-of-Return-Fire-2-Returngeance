#include "PrecompiledHeader.h"
#include "FreeSteeringSystem.h"
#include "World.h"

void Systems::FreeSteeringSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("FreeSteering", []() { return new Components::FreeSteering(); });
}

void Systems::FreeSteeringSystem::Update(double dt)
{

}

void Systems::FreeSteeringSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto steering = m_World->GetComponent<Components::FreeSteering>(entity, "FreeSteering");
	auto input = m_World->GetComponent<Components::Input>(entity, "Input");
	if (steering && input)
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity, "Transform");

		glm::vec3 Camera_Right = glm::vec3(glm::vec4(1, 0, 0, 0) * transform->Orientation);
		glm::vec3 Camera_Forward = glm::vec3(glm::vec4(0, 0, 1, 0) * transform->Orientation);

		float speed = steering->Speed;
		if (input->KeyState[GLFW_KEY_LEFT_SHIFT])
		{
			speed *= 4.0f;
		}
		if (input->KeyState[GLFW_KEY_LEFT_ALT])
		{
			speed /= 4.0f;
		}
		if (input->KeyState[GLFW_KEY_A])
		{
			transform->Position -= Camera_Right * (float)dt * speed;
		}
		else if (input->KeyState[GLFW_KEY_D])
		{
			transform->Position += Camera_Right * (float)dt * speed;
		}
		if (input->KeyState[GLFW_KEY_W])
		{
			transform->Position -= Camera_Forward * (float)dt * speed;
		}
		if (input->KeyState[GLFW_KEY_S])
		{
			transform->Position += Camera_Forward * (float)dt * speed;
		}
		if (input->KeyState[GLFW_KEY_SPACE])
		{
			transform->Position += glm::vec3(0, 1, 0) * (float)dt * speed;
		}
		if (input->KeyState[GLFW_KEY_LEFT_CONTROL])
		{
			transform->Position -= glm::vec3(0, 1, 0) * (float)dt * speed;
		}

		if (input->MouseState[GLFW_MOUSE_BUTTON_LEFT])
		{
			// TOUCHING THIS CODE MIGHT CAUSE THE UNIVERSE TO IMPLODE, ALSO DRAGONS // spelling tobias :3
			//---------------------------------------------------------------------
			transform->Orientation = glm::angleAxis<float>(input->dY / 300.f, glm::vec3(1, 0, 0)) * transform->Orientation;

			transform->Orientation = transform->Orientation * glm::angleAxis<float>(input->dX / 300.f, glm::vec3(0, 1, 0));
			//---------------------------------------------------------------------
			// TOUCHING THIS CODE MIGHT CAUSE THE UNIVERSE TO IMPLODE, ALSO DRAGONS
		}
	}
}
