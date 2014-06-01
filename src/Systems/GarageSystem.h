#ifndef GarageSystem_h__
#define GarageSystem_h__


#include "System.h"
#include "Components/Transform.h"
#include "Components/Trigger.h"
#include "Components/Garage.h"
#include "Events/EnterTrigger.h"
#include "Events/LeaveTrigger.h"

#include "Components/Player.h"
#include "Events/Move.h"
#include "Events/Rotate.h"
#include "Components/Move.h"
#include "Components/Rotate.h"

namespace Systems
{
	class GarageSystem : public System
	{
	public:

		GarageSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
			: System(world, eventBroker, resourceManager) { }

		void Initialize() override;
		void RegisterComponents(ComponentFactory* cf) override;
		void Update(double dt) override;
		void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

	private:
		EventRelay<GarageSystem, Events::EnterTrigger> m_EEnterTrigger;
		bool OnEnterTrigger(const Events::EnterTrigger &event);
		EventRelay<GarageSystem, Events::LeaveTrigger> m_ELeaveTrigger;
		bool OnLeaveTrigger(const Events::LeaveTrigger &event);

		void ToggleGarage(Components::Garage* garageComponent);
	};

}
#endif // GarageSystem_h__
