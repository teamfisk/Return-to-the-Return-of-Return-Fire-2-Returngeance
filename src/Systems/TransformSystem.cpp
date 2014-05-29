#include "PrecompiledHeader.h"
#include "TransformSystem.h"
#include "World.h"

//void Systems::TransformSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
//{
//	if (parent == 0)
//		return;
//
//	auto transform = m_World->GetComponent<Components::Transform>(entity);
//	auto parentTransform = m_World->GetComponent<Components::Transform>(parent);
//
//	transform->Position = parentTransform->Position + transform->RelativePosition;
//}

void Systems::TransformSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_EMove, &Systems::TransformSystem::OnMove);
}

void Systems::TransformSystem::UpdateEntity( double dt, EntityID entity, EntityID parent )
{
	if(m_MoveItems.find(entity) != m_MoveItems.end())
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity);
		glm::vec3 move = m_MoveItems[entity].GoalPosition - transform->Position;
		move *= m_MoveItems[entity].Time;
		m_MoveItems[entity].Time -= dt;

		if(m_MoveItems[entity].Time <= 0)
		{
			float movetime = dt + m_MoveItems[entity].Time;
			transform->Position += move * movetime;

			m_MoveItems.erase(entity);

			if(m_QueuedMoveItems.find(entity) != m_QueuedMoveItems.end())
			{
				auto it = m_QueuedMoveItems.find(entity);
				m_MoveItems[entity] = it->second;
				m_QueuedMoveItems.erase(it->first);
			}
		}
		else
		{
			transform->Position += move * (float)dt;
		}
	}
}


bool Systems::TransformSystem::OnMove( const Events::Move &event )
{
	MoveItems item;
	item.Entity = event.Entity;
	item.GoalPosition = event.GoalPosition;
	item.Time = event.Time;
	m_MoveItems[event.Entity] = item;

	if(event.Queue)
	{
		m_QueuedMoveItems.insert(std::make_pair(event.Entity, item));
	}
	return true;
}


glm::vec3 Systems::TransformSystem::AbsolutePosition(EntityID entity)
{
	glm::vec3 absPosition;
	glm::quat accumulativeOrientation;

	do
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity);
		//absPosition += transform->Position;
		entity = m_World->GetEntityParent(entity);
		auto transform2 = m_World->GetComponent<Components::Transform>(entity);
		if (entity == 0)
			absPosition += transform->Position;
		else
			absPosition = transform2->Orientation * (absPosition + transform->Position);
	} while (entity != 0);

	return absPosition * accumulativeOrientation;
}

glm::quat Systems::TransformSystem::AbsoluteOrientation(EntityID entity)
{
	glm::quat absOrientation;

	do
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity);
		absOrientation = transform->Orientation * absOrientation;
		entity = m_World->GetEntityParent(entity);
	} while (entity != 0);

	return absOrientation;
}

glm::vec3 Systems::TransformSystem::AbsoluteScale(EntityID entity)
{
	glm::vec3 absScale(1);

	do
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity);
		absScale *= transform->Scale;
		entity = m_World->GetEntityParent(entity);
	} while (entity != 0);

	return absScale;
}

Components::Transform Systems::TransformSystem::AbsoluteTransform(EntityID entity)
{
	glm::vec3 absPosition;
	glm::quat absOrientation;
	glm::vec3 absScale(1);

	do
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity);
		entity = m_World->GetEntityParent(entity);
		auto transform2 = m_World->GetComponent<Components::Transform>(entity);

		// Position
		if (entity == 0)
			absPosition += transform->Position;
		else
			absPosition = transform2->Orientation * (absPosition + transform->Position);
		// Orientation
		absOrientation = transform->Orientation * absOrientation;
		// Scale
		absScale *= transform->Scale;
	} while (entity != 0);

	Components::Transform transform;
	transform.Position = absPosition;
	transform.Orientation = absOrientation;
	transform.Scale = absScale;

	return transform;
}



