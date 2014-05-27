#ifndef TimerSystem_h__
#define TimerSystem_h__


#include "System.h"
#include "Components/Transform.h"
#include "Components/Timer.h"
#include "Components/FrameTimer.h"

namespace Systems
{
	class TimerSystem : public System
	{
	public:

		TimerSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
			: System(world, eventBroker) { }

		void RegisterComponents(ComponentFactory* cf) override;
		void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

	private:


	};

}
#endif // TimerSystem_h__
