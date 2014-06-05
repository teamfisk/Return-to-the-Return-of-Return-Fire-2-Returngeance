#ifndef TowerSystem_h__
#define TowerSystem_h__


#include "System.h"
#include "Systems/TransformSystem.h"
#include "Components/Player.h"
#include "Components/Tower.h"
#include "Components/Turret.h"
#include "Components/TurretShot.h"
#include "Components/Health.h"
#include "Components/TowerDebris.h"
#include "Components/Model.h"
#include "Events/SetVelocity.h"
#include "Events/CreateExplosion.h"
#include "Events/Collision.h"
#include "Events/Damage.h"
#include "Events/ApplyPointImpulse.h"






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


		EventRelay<TowerSystem, Events::Collision> m_ECollision;
		bool OnCollision(const Events::Collision &e);
		EventRelay<TowerSystem, Events::Damage> m_eDamage;
		bool Damage(const Events::Damage &event);


	private:
		std::map<EntityID, double> m_TimeSinceLastShot;
		float Temp_m_TimeSinceLastShot;

	};

}
#endif // TowerSystem_h__
