#include "PrecompiledHeader.h"
#include "FollowSystem.h"
#include "World.h"

void Systems::FollowSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Follow>([]() { return new Components::Follow(); });
}

void Systems::FollowSystem::Update( double dt )
{

}

void Systems::FollowSystem::UpdateEntity( double dt, EntityID entity, EntityID parent )
{
	auto followComponent = m_World->GetComponent<Components::Follow>(entity);
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity);
	if(!followComponent || !transformComponent)
		return;
	auto absoluteTransformTarget = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(followComponent->Entity);

	glm::vec3 Offset = (absoluteTransformTarget.Orientation * glm::normalize(absoluteTransformTarget.Position) * followComponent->Distance);
	transformComponent->Position = (absoluteTransformTarget.Position + Offset);
	transformComponent->Orientation = -glm::quat(glm::eulerAngles(absoluteTransformTarget.Orientation) * followComponent->FollowAxis);
}
