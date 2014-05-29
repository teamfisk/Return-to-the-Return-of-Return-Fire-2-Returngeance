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
	auto transformFake1 = m_World->GetComponent<Components::Transform>(wheelPair->FakeWheel1);
	if (!transformFake1)
		return;
	auto transformFake2 = m_World->GetComponent<Components::Transform>(wheelPair->FakeWheel2);
	if (!transformFake2)
		return;

	glm::vec3 thing = transformFake2->Position - transformFake1->Position;
	glm::vec3 forward = glm::normalize(glm::vec3(thing.x, 0, thing.z));
	glm::vec3 up = glm::vec3(0, 1, 0);

	float angle = glm::acos(glm::dot(glm::normalize(thing), forward));
	float height = (thing / 2.f).y;

	transform->Position.y = height;
	//transform->Orientation = glm::quat(glm::vec3())
}