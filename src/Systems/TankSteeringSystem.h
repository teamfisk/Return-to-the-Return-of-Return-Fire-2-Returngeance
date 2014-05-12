#include <array>

#include "System.h"
#include "Events/TankSteer.h"
#include "Components/Transform.h"
#include "Components/TankSteering.h"
#include "Components/Vehicle.h"
#include "InputController.h"

namespace Systems
{

	class TankSteeringSystem : public System
	{
	public:
		TankSteeringSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
			: System(world, eventBroker) { }

		void RegisterComponents(ComponentFactory* cf) override;
		void Initialize() override;

		void Update(double dt) override;
		void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

	private:
		class TankSteeringInputController;
		std::unique_ptr<TankSteeringInputController> m_InputController;
	};

	class TankSteeringSystem::TankSteeringInputController : InputController
	{
	public:
		TankSteeringInputController(std::shared_ptr<::EventBroker> eventBroker)
			: InputController(eventBroker) { }

		float PositionY;
		float PositionX;
		bool Handbrake;

	protected:
		virtual bool OnCommand(const Events::InputCommand &event);
		virtual bool OnMouseMove(const Events::MouseMove &event);
	};

}