#include "PrecompiledHeader.h"
#include "TimerSystem.h"
#include "World.h"

void Systems::TimerSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Timer>([]() { return new Components::Timer(); });
	cf->Register<Components::FrameTimer>([]() { return new Components::FrameTimer(); });
}

void Systems::TimerSystem::UpdateEntity( double dt, EntityID entity, EntityID parent )
{
	auto timer = m_World->GetComponent<Components::Timer>(entity);
	if(timer)
	{
		timer->Time -= dt;
		
		if(timer->Time <= 0)
		{
			m_World->RemoveEntity(entity);
		}
	}

	auto frameTimer = m_World->GetComponent<Components::FrameTimer>(entity);
	if(frameTimer)
	{
		frameTimer->Frames -= 1;

		if(frameTimer->Frames <= 0)
		{
			m_World->RemoveEntity(entity);
		}
	}

	
}


