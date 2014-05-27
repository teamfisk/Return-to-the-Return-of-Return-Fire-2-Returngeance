#ifndef DamageSystem_h__
#define DamageSystem_h__


#include "System.h"
#include "Components/Health.h"
#include "Events/Damage.h"

namespace Systems
{
	class DamageSystem : public System
	{
	public:

		DamageSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
			: System(world, eventBroker) { }


		void Initialize() override;
		void RegisterComponents(ComponentFactory* cf) override;



		EventRelay<DamageSystem, Events::Damage> m_EDamage;
		bool OnDamage(const Events::Damage &event);

	private:

	};

}
#endif // DamageSystem_h__
