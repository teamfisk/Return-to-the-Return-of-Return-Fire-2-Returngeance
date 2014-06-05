#ifndef DamageSystem_h__
#define DamageSystem_h__


#include "System.h"
#include "Components/Health.h"
#include "Components/DamageModel.h"
#include "Events/Damage.h"
#include "Events/OnDead.h"

namespace Systems
{
	class DamageSystem : public System
	{
	public:

		DamageSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
			: System(world, eventBroker, resourceManager) { }


		void Initialize() override;
		void RegisterComponents(ComponentFactory* cf) override;



		EventRelay<DamageSystem, Events::Damage> m_EDamage;
		bool OnDamage(const Events::Damage &event);

	private:

	};

}
#endif // DamageSystem_h__
