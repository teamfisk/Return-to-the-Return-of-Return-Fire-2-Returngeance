#ifndef FollowSystem_h__
#define FollowSystem_h__


#include "System.h"
#include "Systems/TransformSystem.h"
#include "Components/Transform.h"
#include "Components/Follow.h"

#include "Events/Move.h"
#include "Events/Rotate.h"


namespace Systems
{
	class FollowSystem : public System
	{
	public:

		FollowSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
			: System(world, eventBroker, resourceManager) { }

		//void Initialize() override;
		void RegisterComponents(ComponentFactory* cf) override;
		void Update(double dt) override;
		void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
		/*void OnComponentCreated(std::string type, std::shared_ptr<Component> component) override;
		void OnComponentRemoved(EntityID entity, std::string type, Component* component) override;
		void OnEntityCommit(EntityID entity) override;
		void OnEntityRemoved(EntityID entity) override;*/

	private:

	};

}
#endif // FollowSystem_h__
