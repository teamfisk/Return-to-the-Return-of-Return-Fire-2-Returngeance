#include "PrecompiledHeader.h"
#include "HelicopterSteeringSystem.h"
#include "World.h"

void Systems::HelicopterSteeringSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::HelicopterSteering>([]() { return new Components::HelicopterSteering(); });
}

void Systems::HelicopterSteeringSystem::Initialize()
{
	m_InputController = std::unique_ptr<HelicopterSteeringInputController>(new HelicopterSteeringInputController(EventBroker));
}

void Systems::HelicopterSteeringSystem::Update(double dt)
{

}

void Systems::HelicopterSteeringSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transform = m_World->GetComponent<Components::Transform>(entity);
	if (!transform)
		return;

	auto helicopterComponent = m_World->GetComponent<Components::HelicopterSteering>(entity);
	if (helicopterComponent)
	{
		glm::vec3 controllerRotationEuler = m_InputController->Rotation * (float)dt;
		transform->Orientation *= glm::quat(controllerRotationEuler);

		Events::ApplyForce e;
		e.Entity = entity;
		e.DeltaTime = dt;
		e.Force = glm::normalize(transform->Orientation * glm::vec3(0, 1, 0)) * (m_InputController->Power * 3000.f  * 9.82f * 8.f);
		EventBroker->Publish(e);
	}
}

bool Systems::HelicopterSteeringSystem::HelicopterSteeringInputController::OnCommand(const Events::InputCommand &event)
{
	if (event.Command == "horizontal")
	{
		Rotation.z = -event.Value;
	}
	else if (event.Command == "vertical")
	{
		Rotation.x = -event.Value;
	}

	else if (event.Command == "normal")
	{
		Power = event.Value;
	}

	return true;
}

bool Systems::HelicopterSteeringSystem::HelicopterSteeringInputController::OnMouseMove(const Events::MouseMove &event)
{
	return true;
}

void Systems::HelicopterSteeringSystem::HelicopterSteeringInputController::Update(double dt)
{
	
}
