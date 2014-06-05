#include <array>

#include "System.h"
#include "Components/Transform.h"
#include "Components/Physics.h"
#include "Components/Vehicle.h"
#include "Components/Player.h"
#include "Components/Model.h"
#include "Systems/TransformSystem.h"
#include "Systems/ParticleSystem.h"
#include "InputController.h"

#include "Components/Health.h"

#include "Components/Trigger.h"

#include "Components/Vehicle.h"
#include "Components/Player.h"

#include "Events/SpawnVehicle.h"
#include "Components/SpawnPoint.h"

#include "Components/Wheel.h"
#include "Components/MeshShape.h"
#include "Components/Camera.h"
#include "Components/BoxShape.h"
#include "Components/WheelPair.h"
#include "Components/Input.h"
#include "Components/JeepSteering.h"
#include "Events/JeepSteer.h"
#include "Components/Team.h"

#include "Events/SetViewportCamera.h"

namespace Systems
{

	class JeepSteeringSystem : public System
	{
	public:
		JeepSteeringSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
			: System(world, eventBroker, resourceManager)
		{ }

		void RegisterComponents(ComponentFactory* cf) override;
		void Initialize() override;

		void Update(double dt) override;
		void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

	private:
		std::map<EntityID, double> m_TimeSinceLastShot;
		EventRelay<JeepSteeringSystem, Events::SpawnVehicle> m_ESpawnVehicle;
		bool OnSpawnVehicle(const Events::SpawnVehicle &e);

		class JeepSteeringInputController;
		std::array<std::shared_ptr<JeepSteeringInputController>, 4> m_JeepInputControllers;

		EntityID CreateJeep(int playerID);
		void AddJeepWheels(EntityID jeepEntity);
	};

	class JeepSteeringSystem::JeepSteeringInputController : InputController<JeepSteeringSystem>
	{
	public:
		JeepSteeringInputController(std::shared_ptr<::EventBroker> eventBroker, int playerID)
			: InputController(eventBroker)
		{
			PlayerID = playerID;

			m_Horizontal = 0.f;
			m_Vertical = 0.f;
			PositionX = 0;
			PositionY = 0;
			Handbrake = false;
		}

		int PlayerID;

		float PositionY;
		float PositionX;
		bool Handbrake;

		void Update(double dt);
	protected:
		virtual bool OnCommand(const Events::InputCommand &event);
		//virtual bool OnMouseMove(const Events::MouseMove &event);

	private:
		float m_Horizontal;
		float m_Vertical;
	};
}