#include "PrecompiledHeader.h"
#include "WheelPairSystem.h"
#include "World.h"

void Systems::WheelPairSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::WheelPair>([]() { return new Components::WheelPair(); });
}

void Systems::WheelPairSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto wheelPair = m_World->GetComponent<Components::WheelPair>(entity);
	if (!wheelPair)
		return;

	auto transform = m_World->GetComponent<Components::Transform>(entity);
	if (!transform)
		return;
	auto transformFakeFront = m_World->GetComponent<Components::Transform>(wheelPair->FakeWheelFront);
	if (!transformFakeFront)
		return;
	auto transformFakeBack = m_World->GetComponent<Components::Transform>(wheelPair->FakeWheelBack);
	if (!transformFakeBack)
		return;

	glm::vec3 thing = transformFakeBack->Position - transformFakeFront->Position;

	float height = ((transformFakeFront->Position + transformFakeBack->Position) / 2.f).y;
	float angle = std::atan2f(thing.y, thing.z);

	transform->Position.y = height;
	transform->Orientation = glm::quat(glm::eulerAngles(transform->Orientation) * glm::vec3(0, 1, 1) + glm::vec3(-angle, 0, 0));
}