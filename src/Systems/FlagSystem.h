#ifndef FlagSystem_h__
#define FlagSystem_h__


#include "System.h"
#include "Components/Flag.h"
#include "Components/Player.h"
#include "Components/Team.h"
#include "Components/Trigger.h"
#include "Components/Transform.h"
#include "Components/JeepSteering.h"

#include "Events/FlagCaptured.h"
#include "Events/FlagDropped.h"
#include "Events/FlagPickup.h"
#include "Events/FlagReturned.h"


#include "Events/EnterTrigger.h"

namespace Systems
{
	class FlagSystem : public System
	{
	public:

		FlagSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
			: System(world, eventBroker, resourceManager) { }


		void Initialize() override;
		void RegisterComponents(ComponentFactory* cf) override;
		void Update(double dt) override;
		void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

	private:
		EventRelay<FlagSystem, Events::EnterTrigger> m_EEnterTrigger;
		bool OnEnterTrigger(const Events::EnterTrigger &event);

		void PickUpFlag(EntityID entity, EntityID trigger);
	};

}
#endif // DamageSystem_h__
