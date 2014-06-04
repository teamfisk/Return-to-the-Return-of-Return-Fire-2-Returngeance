#ifndef TowerSystem_h__
#define TowerSystem_h__


#include "System.h"
#include "Systems/TransformSystem.h"
#include "Components/Player.h"
#include "Components/Tower.h"



namespace Systems
{
	class TowerSystem : public System
	{
	public:

		TowerSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
			: System(world, eventBroker, resourceManager) { }


		void Initialize() override;
		void RegisterComponents(ComponentFactory* cf) override;

		virtual void Update(double dt);
		virtual void UpdateEntity(double dt, EntityID entity, EntityID parent);

		//EventRelay<TowerSystem, Events::Damage> m_eDamage;
		//bool Damage(const Events::Damage &event);

	private:

	};

}
#endif // TowerSystem_h__
