#ifndef TriggerSystem_h__
#define TriggerSystem_h__


#include "System.h"
#include "Components/Transform.h"
#include "Events/SetVelocity.h"
#include "Events/ApplyForce.h"
#include "Events/ApplyPointImpulse.h"

#include <unordered_map>

#include "Components/Trigger.h"
#include "Components/TriggerExplosion.h"
#include "Events/EnterTrigger.h"
#include <math.h>
namespace Systems
{
	class TriggerSystem : public System
	{
	public:
		
		TriggerSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
			: System(world, eventBroker) { }

		void Initialize() override;
		void RegisterComponents(ComponentFactory* cf) override;
		void Update(double dt) override;
		void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
		void OnComponentCreated(std::string type, std::shared_ptr<Component> component) override;
		void OnComponentRemoved(EntityID entity, std::string type, Component* component) override;
		void OnEntityCommit(EntityID entity) override;
		void OnEntityRemoved(EntityID entity) override;

		EventRelay<TriggerSystem, Events::EnterTrigger> m_EEnterTrigger;
		bool OnEnterTrigger(const Events::EnterTrigger &event);
	private:
		void Explosion(EntityID entity, EntityID phantomEntity);

	};

}
#endif // TriggerSystem_h__
