#ifndef WallSystem_h__
#define WallSystem_h__


#include "System.h"
#include "Systems/TransformSystem.h"
#include "Events/OnDead.h"
#include "Events/Damage.h"
#include "Events/ApplyPointImpulse.h"
#include "Components/Model.h"
#include "Components/DamageModel.h"
#include "Components/Wall.h"
#include "Components/Transform.h"
#include "Components/Physics.h"
#include "Components/MeshShape.h"
#include "Components/Tankshell.h"
#include "Components/Health.h"



namespace Systems
{
	class WallSystem : public System
	{
	public:

		WallSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
			: System(world, eventBroker, resourceManager) { }


		void Initialize() override;
		void RegisterComponents(ComponentFactory* cf) override;



		EventRelay<WallSystem, Events::Damage> m_eDamage;
		bool Damage(const Events::Damage &event);

	private:

	};

}
#endif // WallSystem_h__
