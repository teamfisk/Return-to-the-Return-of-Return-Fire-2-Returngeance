#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();

	ResourceManager->Preload("Model", "Models/Placeholders/PhysicsTest/Plane.obj");
	ResourceManager->Preload("Model", "Models/Placeholders/PhysicsTest/ArrowCube.obj");

	//Background Music
	{
		Events::PlayBGM e;
		e.Resource = "Sounds/SFX/WUB.mp3";
		EventBroker->Publish(e);
	}

	// Interface
	BindKey(GLFW_KEY_A, "interface_horizontal", -1.f);
	BindKey(GLFW_KEY_D, "interface_horizontal", 1.f);
	BindGamepadAxis(Gamepad::Axis::LeftX, "interface_horizontal", 1.f);
	BindGamepadButton(Gamepad::Button::Left, "interface_horizontal", -1.f);
	BindGamepadButton(Gamepad::Button::Right, "interface_horizontal", 1.f);
	BindKey(GLFW_KEY_W, "interface_vertical", 1.f);
	BindKey(GLFW_KEY_S, "interface_vertical", -1.f);
	BindGamepadAxis(Gamepad::Axis::LeftY, "interface_vertical", 1.f);
	BindGamepadButton(Gamepad::Button::Up, "interface_vertical", 1.f);
	BindGamepadButton(Gamepad::Button::Down, "interface_vertical", -1.f);
	BindKey(GLFW_KEY_SPACE, "interface_confirm", 1.f);
	BindGamepadButton(Gamepad::Button::A, "interface_confirm", 1.f);
	
	

	BindKey(GLFW_KEY_W, "vertical", 1.f);
	BindKey(GLFW_KEY_S, "vertical", -1.f);
	BindKey(GLFW_KEY_A, "horizontal", -1.f);
	BindKey(GLFW_KEY_D, "horizontal", 1.f);
	BindGamepadAxis(Gamepad::Axis::LeftX, "horizontal", 1.f);
	BindGamepadAxis(Gamepad::Axis::LeftY, "vertical", 1.f);

	BindKey(GLFW_KEY_UP, "barrel_rotation", 1.f);
	BindKey(GLFW_KEY_DOWN, "barrel_rotation", -1.f);
	BindKey(GLFW_KEY_LEFT, "tower_rotation", -1.f);
	BindKey(GLFW_KEY_RIGHT, "tower_rotation", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightX, "tower_rotation", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightY, "barrel_rotation", 1.f);

	BindKey(GLFW_KEY_SPACE, "handbrake", 1.f);
	BindGamepadButton(Gamepad::Button::A, "handbrake", 1.f);

	BindKey(GLFW_KEY_Z, "shoot", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightTrigger, "shoot", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightTrigger, "jeep_vertical", 1.f);
	BindGamepadAxis(Gamepad::Axis::LeftTrigger, "jeep_vertical", -1.f);

	BindKey(GLFW_KEY_X, "use", 1.f);
	BindGamepadButton(Gamepad::Button::X, "use", 1.f);

	BindMouseButton(GLFW_MOUSE_BUTTON_1, "cam_lock", 1.f);
	BindKey(GLFW_KEY_Y, "cam_vertical", 1.f);
	BindKey(GLFW_KEY_H, "cam_vertical", -1.f);
	BindKey(GLFW_KEY_G, "cam_horizontal", -1.f);
	BindKey(GLFW_KEY_J, "cam_horizontal", 1.f);
	BindKey(GLFW_KEY_U, "cam_normal", 1.f);
	BindKey(GLFW_KEY_T, "cam_normal", -1.f);
	BindKey(GLFW_KEY_B, "cam_speed", -1.f);
	BindKey(GLFW_KEY_N, "cam_speed", 1.f);
	
	//BindGamepadButton(Gamepad::Button::Up, "Gamepad::Button::Up", 1.f);
	//BindGamepadButton(Gamepad::Button::Down, "Gamepad::Button::Down", 1.f);
	//BindGamepadButton(Gamepad::Button::Left, "Gamepad::Button::Left", 1.f);
	//BindGamepadButton(Gamepad::Button::Right, "Gamepad::Button::Right", 1.f);
	//BindGamepadButton(Gamepad::Button::Start, "Gamepad::Button::Start", 1.f);
	//BindGamepadButton(Gamepad::Button::Back, "Gamepad::Button::Back", 1.f);
	//BindGamepadButton(Gamepad::Button::LeftThumb, "Gamepad::Button::LeftThumb", 1.f);
	//BindGamepadButton(Gamepad::Button::RightThumb, "Gamepad::Button::RightThumb", 1.f);
	//BindGamepadButton(Gamepad::Button::LeftShoulder, "Gamepad::Button::LeftShoulder", 1.f);
	//BindGamepadButton(Gamepad::Button::RightShoulder, "Gamepad::Button::RightShoulder", 1.f);
	//BindGamepadButton(Gamepad::Button::A, "Gamepad::Button::A", 1.f);
	//BindGamepadButton(Gamepad::Button::B, "Gamepad::Button::B", 1.f);
	//BindGamepadButton(Gamepad::Button::X, "Gamepad::Button::X", 1.f);
	//BindGamepadButton(Gamepad::Button::Y, "Gamepad::Button::Y", 1.f);

	RegisterComponents();

	auto camera = CreateEntity();
	{
		auto transform = AddComponent<Components::Transform>(camera);
		transform->Position.z = 150.f;
		transform->Position.y = 10.f;
		//transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
		auto cameraComp = AddComponent<Components::Camera>(camera);
		cameraComp->FarClip = 2000.f;
		auto freeSteering = AddComponent<Components::FreeSteering>(camera);
	}
	{
		Events::SetViewportCamera e;
		e.CameraEntity = camera;
		e.ViewportFrame = "ViewportFreeCam";
		EventBroker->Publish(e);
	}

#pragma region Wall_Debris_Template

	{
		auto debri = CreateEntity();
		AddComponent<Components::Template>(debri);
		auto transform = AddComponent<Components::Transform>(debri);
		transform->Position = glm::vec3(3.43f, 2.287f, 0.f);
		auto model = AddComponent<Components::Model>(debri);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris1.obj";
		auto physics = AddComponent<Components::Physics>(debri);
		physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
		physics->Mass = 200.f;
		physics->Restitution = .04f;
		physics->Friction = 0.9f;
	
		auto shape = CreateEntity(debri);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris1.obj";

		m_WallDebrisTemplates.push_back(debri);
	}

	{
		auto debri = CreateEntity();
		AddComponent<Components::Template>(debri);
		auto transform = AddComponent<Components::Transform>(debri);
		transform->Position = glm::vec3(-3.744f, 1.508f, 0.f);
		auto model = AddComponent<Components::Model>(debri);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris2.obj";
		auto physics = AddComponent<Components::Physics>(debri);
		physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
		physics->Mass = 200.f;
		physics->Restitution = .04f;
		physics->Friction = 0.9f;

		auto shape = CreateEntity(debri);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris2.obj";

		m_WallDebrisTemplates.push_back(debri);

	}

	{
		auto debri = CreateEntity();
		AddComponent<Components::Template>(debri);
		auto transform = AddComponent<Components::Transform>(debri);
		transform->Position = glm::vec3(-3.592f, 4.664f, 0.f);
		auto model = AddComponent<Components::Model>(debri);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris2.obj";
		auto physics = AddComponent<Components::Physics>(debri);
		physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
		physics->Mass = 200.f;
		physics->Restitution = .04f;
		physics->Friction = 0.9f;

		auto shape = CreateEntity(debri);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris2.obj";

		m_WallDebrisTemplates.push_back(debri);

	}

	{
		auto debri = CreateEntity();
		AddComponent<Components::Template>(debri);
		auto transform = AddComponent<Components::Transform>(debri);
		transform->Position = glm::vec3(-2.535f, 2.879f, 0.f);
		auto model = AddComponent<Components::Model>(debri);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris3.obj";
		auto physics = AddComponent<Components::Physics>(debri);
		physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
		physics->Mass = 200.f;
		physics->Restitution = .04f;
		physics->Friction = 0.9f;

		auto shape = CreateEntity(debri);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris3.obj";

		m_WallDebrisTemplates.push_back(debri);

	}

	{
		auto debri = CreateEntity();
		AddComponent<Components::Template>(debri);
		auto transform = AddComponent<Components::Transform>(debri);
		transform->Position = glm::vec3(0.333f, 3.618f, 0.f);
		auto model = AddComponent<Components::Model>(debri);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris4.obj";
		auto physics = AddComponent<Components::Physics>(debri);
		physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
		physics->Mass = 200.f;
		physics->Restitution = .04f;
		physics->Friction = 0.9f;

		auto shape = CreateEntity(debri);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris4.obj";

		m_WallDebrisTemplates.push_back(debri);

	}

	{
		auto debri = CreateEntity();
		AddComponent<Components::Template>(debri);
		auto transform = AddComponent<Components::Transform>(debri);
		transform->Position = glm::vec3(2.586f, 4.333f, 0.f);
		auto model = AddComponent<Components::Model>(debri);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris5.obj";
		auto physics = AddComponent<Components::Physics>(debri);
		physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
		physics->Mass = 200.f;
		physics->Restitution = .04f;
		physics->Friction = 0.9f;

		auto shape = CreateEntity(debri);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris5.obj";

		m_WallDebrisTemplates.push_back(debri);

	}

	{
		auto debri = CreateEntity();
		AddComponent<Components::Template>(debri);
		auto transform = AddComponent<Components::Transform>(debri);
		transform->Position = glm::vec3(0.537f, 1.344f, 0.f);
		auto model = AddComponent<Components::Model>(debri);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris6.obj";
		auto physics = AddComponent<Components::Physics>(debri);
		physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
		physics->Mass = 200.f;
		physics->Restitution = .04f;
		physics->Friction = 0.9f;

		auto shape = CreateEntity(debri);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris6.obj";

		m_WallDebrisTemplates.push_back(debri);
	}

	{
		auto debri = CreateEntity();
		AddComponent<Components::Template>(debri);
		auto transform = AddComponent<Components::Transform>(debri);
		transform->Position = glm::vec3(0.f, 0.f, 0.f);
		auto model = AddComponent<Components::Model>(debri);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDestroyed.obj";
		auto physics = AddComponent<Components::Physics>(debri);
		physics->MotionType = Components::Physics::MotionTypeEnum::Keyframed;
		physics->Mass = 200.f;
		physics->Restitution = .004f;
		physics->Friction = 0.9f;

		auto shape = CreateEntity(debri);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDestroyedColosion.obj";

		m_WallDebrisTemplates.push_back(debri);
	}
#pragma endregion Region for wall debris

	CreateTerrain();

	/*EntityID tank1 = CreateTank(1);
	{
		auto transform = GetComponent<Components::Transform>(tank1);
		transform->Position.z = 50;

		Events::SetViewportCamera e;
		e.CameraEntity = GetProperty<EntityID>(tank1, "Camera");
		e.ViewportFrame = "Viewport1";
		EventBroker->Publish(e);
	}

	EntityID tank2 = CreateTank(2);
	{
		auto transform = GetComponent<Components::Transform>(tank2);
		transform->Position.x = 10.f;
		transform->Position.z = 50.f;

		Events::SetViewportCamera e;
		e.CameraEntity = GetProperty<EntityID>(tank2, "Camera");
		e.ViewportFrame = "Viewport2";
		EventBroker->Publish(e);
	}*/

	

	//for (int i = 0; i < 500; i++)
	//{
	//	auto Light = CreateEntity();
	//	auto transform = AddComponent<Components::Transform>(Light);
	//	transform->Position = glm::vec3((5 + (i*2.f))*cos(i*2.f), 3.f, (5 + (i*2.f))*sin(i*2.f));
	//	auto light = AddComponent<Components::PointLight>(Light);
	//	light->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	//	light->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	//	light->Radius = 15.f;
	//	light->specularExponent = 100.f;
	//	CommitEntity(Light);
	//	//auto model = AddComponent<Components::Model>(Light, "Model");
	//	//model->ModelFile = "Models/Placeholders/PhysicsTest/PointLight.obj";
	//}

	
	{
		auto gateBase = CreateEntity();
		auto transform = AddComponent<Components::Transform>(gateBase);
		transform->Position = glm::vec3(0, -50, 150);
		auto model = AddComponent<Components::Model>(gateBase);
		model->ModelFile = "Models/Gate/Lift/Lift.obj";
		
		auto physics = AddComponent<Components::Physics>(gateBase);
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1; // Terrain-Layer
		{
			auto leftBaseShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(leftBaseShape);
			transform->Position = glm::vec3(8.5f, 3, 0);
			auto box = AddComponent<Components::BoxShape>(leftBaseShape);
			box->Width = 3.f/2;
			box->Height = 6.094f/2;
			box->Depth = 4.f/2;
			CommitEntity(leftBaseShape);
		}
		{
			auto RightBaseShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(RightBaseShape);
			transform->Position = glm::vec3(-8.5f, 3, 0);
			auto box = AddComponent<Components::BoxShape>(RightBaseShape);
			box->Width = 3.f/2;
			box->Height = 6.094f/2;
			box->Depth = 4.f/2;
			CommitEntity(RightBaseShape);
		}
		{
			auto LeftTopShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(LeftTopShape);
			transform->Position = glm::vec3(7.5485f, 9.385f, 0);
			auto box = AddComponent<Components::BoxShape>(LeftTopShape);
			box->Width = 1.747f/2;
			box->Height = 6.851f/2;
			box->Depth = 1.767f/2;
			CommitEntity(LeftTopShape);
		}
		{
			auto RightTopShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(RightTopShape);
			transform->Position = glm::vec3(-7.5485f, 9.385f, 0);
			auto box = AddComponent<Components::BoxShape>(RightTopShape);
			box->Width = 1.747f/2;
			box->Height = 6.851f/2;
			box->Depth = 1.767f/2;
			CommitEntity(RightTopShape);
		}
		{
			auto RampShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(RampShape);
			transform->Position = glm::vec3(0, 0.24f, 0);
			auto box = AddComponent<Components::BoxShape>(RampShape);
			box->Width = 14.9f/2;
			box->Height = 0.38f/2;
			box->Depth = 1.484f/2;
			CommitEntity(RampShape);
		}
		CommitEntity(gateBase);
	}
}

void GameWorld::Update(double dt)
{
	World::Update(dt);
}

void GameWorld::RegisterComponents()
{
	m_ComponentFactory.Register<Components::Transform>([]() { return new Components::Transform(); }); 
	m_ComponentFactory.Register<Components::Template>([]() { return new Components::Template(); });	
	m_ComponentFactory.Register<Components::Player>([]() { return new Components::Player(); });	
	m_ComponentFactory.Register<Components::Move>([]() { return new Components::Move(); });
	m_ComponentFactory.Register<Components::Rotate>([]() { return new Components::Rotate(); });
	m_ComponentFactory.Register<Components::Team>([]() { return new Components::Team(); });
}

void GameWorld::RegisterSystems()
{
	m_SystemFactory.Register<Systems::TimerSystem>([this]() { return new Systems::TimerSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::DamageSystem>([this]() { return new Systems::DamageSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::TransformSystem>([this]() { return new Systems::TransformSystem(this, EventBroker, ResourceManager); });
	//m_SystemFactory.Register<Systems::LevelGenerationSystem>([this]() { return new Systems::LevelGenerationSystem(this); });
	m_SystemFactory.Register<Systems::InputSystem>([this]() { return new Systems::InputSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::DebugSystem>([this]() { return new Systems::DebugSystem(this, EventBroker, ResourceManager); });
	//m_SystemFactory.Register<Systems::CollisionSystem>([this]() { return new Systems::CollisionSystem(this); });
	m_SystemFactory.Register<Systems::ParticleSystem>([this]() { return new Systems::ParticleSystem(this, EventBroker, ResourceManager); });
	//m_SystemFactory.Register<Systems::PlayerSystem>([this]() { return new Systems::PlayerSystem(this); });
	m_SystemFactory.Register<Systems::FreeSteeringSystem>([this]() { return new Systems::FreeSteeringSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::TankSteeringSystem>([this]() { return new Systems::TankSteeringSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::JeepSteeringSystem>([this]() { return new Systems::JeepSteeringSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::WheelPairSystem>([this]() { return new Systems::WheelPairSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::SoundSystem>([this]() { return new Systems::SoundSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::PhysicsSystem>([this]() { return new Systems::PhysicsSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::TriggerSystem>([this]() { return new Systems::TriggerSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::FollowSystem>([this]() { return new Systems::FollowSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::GarageSystem>([this]() { return new Systems::GarageSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::WallSystem>([this]() { return new Systems::WallSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::FlagSystem>([this]() { return new Systems::FlagSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::TowerSystem>([this]() { return new Systems::TowerSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::GameStateSystem>([this]() { return new Systems::GameStateSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::RenderSystem>([this]() { return new Systems::RenderSystem(this, EventBroker, ResourceManager); });
}

void GameWorld::AddSystems()
{
	AddSystem<Systems::TimerSystem>();
	AddSystem<Systems::DamageSystem>();
	AddSystem<Systems::TransformSystem>();
	//AddSystem<Systems::LevelGenerationSystem>();
	AddSystem<Systems::InputSystem>();
	AddSystem<Systems::DebugSystem>();
	//AddSystem<Systems::CollisionSystem>();
	AddSystem<Systems::ParticleSystem>();
	//AddSystem<Systems::PlayerSystem>();
	AddSystem<Systems::FreeSteeringSystem>();
	AddSystem<Systems::TankSteeringSystem>();
	AddSystem<Systems::JeepSteeringSystem>();
	AddSystem<Systems::WheelPairSystem>();
	AddSystem<Systems::SoundSystem>();
	AddSystem<Systems::PhysicsSystem>();
	AddSystem<Systems::TriggerSystem>();
	AddSystem<Systems::FollowSystem>();
	AddSystem<Systems::GarageSystem>();
	AddSystem<Systems::WallSystem>();
	AddSystem<Systems::FlagSystem>();
	AddSystem<Systems::TowerSystem>();
	AddSystem<Systems::GameStateSystem>();
	AddSystem<Systems::RenderSystem>();
}

void GameWorld::BindKey(int keyCode, std::string command, float value)
{
	Events::BindKey e;
	e.KeyCode = keyCode;
	e.Command = command;
	e.Value = value;
	EventBroker->Publish(e);
}

void GameWorld::BindMouseButton(int button, std::string command, float value)
{
	Events::BindMouseButton e;
	e.Button = button;
	e.Command = command;
	e.Value = value;
	EventBroker->Publish(e);
}

void GameWorld::BindGamepadAxis(Gamepad::Axis axis, std::string command, float value)
{
	Events::BindGamepadAxis e;
	e.Axis = axis;
	e.Command = command;
	e.Value = value;
	EventBroker->Publish(e);
}

void GameWorld::BindGamepadButton(Gamepad::Button button, std::string command, float value)
{
	Events::BindGamepadButton e;
	e.Button = button;
	e.Command = command;
	e.Value = value;
	EventBroker->Publish(e);
}

void GameWorld::CreateGate(EntityID parent, glm::vec3 position, glm::quat orientation, int teamID)
{
	auto gateParent = CreateEntity(parent);
	auto transform = AddComponent<Components::Transform>(gateParent);
	transform->Position = position;
	transform->Orientation = orientation;
	{
		auto gateBase = CreateEntity(gateParent);
		auto transform = AddComponent<Components::Transform>(gateBase);
		auto model = AddComponent<Components::Model>(gateBase);
		model->ModelFile = "Models/Gate/Lift/Lift.obj";
		auto physics = AddComponent<Components::Physics>(gateBase);
		physics->MotionType = Components::Physics::MotionTypeEnum::Keyframed;
		physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;

		{
			auto leftBaseShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(leftBaseShape);
			transform->Position = glm::vec3(8.5f, 3, 0);
			auto box = AddComponent<Components::BoxShape>(leftBaseShape);
			box->Width = 3.f/2;
			box->Height = 6.094f/2;
			box->Depth = 4.f/2;
			CommitEntity(leftBaseShape);
		}
		{
			auto RightBaseShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(RightBaseShape);
			transform->Position = glm::vec3(-8.5f, 3, 0);
			auto box = AddComponent<Components::BoxShape>(RightBaseShape);
			box->Width = 3.f/2;
			box->Height = 6.094f/2;
			box->Depth = 4.f/2;
			CommitEntity(RightBaseShape);
		}
		{
			auto LeftTopShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(LeftTopShape);
			transform->Position = glm::vec3(7.5485f, 9.385f, 0);
			auto box = AddComponent<Components::BoxShape>(LeftTopShape);
			box->Width = 1.747f/2;
			box->Height = 6.851f/2;
			box->Depth = 1.767f/2;
			CommitEntity(LeftTopShape);
		}

		{
			auto RightTopShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(RightTopShape);
			transform->Position = glm::vec3(-7.5485f, 9.385f, 0);
			auto box = AddComponent<Components::BoxShape>(RightTopShape);
			box->Width = 1.747f/2;
			box->Height = 6.851f/2;
			box->Depth = 1.767f/2;
			CommitEntity(RightTopShape);
		}
		{
			auto RampShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(RampShape);
			transform->Position = glm::vec3(0, 0.24f, 0);
			auto mesh = AddComponent<Components::MeshShape>(RampShape);
			mesh->ResourceName = "Models/Gate/Lift/RampCollision.obj";
			CommitEntity(RampShape);
		}
		CommitEntity(gateBase);
	}

	auto gate = CreateEntity(gateParent);
	{
		auto transform = AddComponent<Components::Transform>(gate);
		auto model = AddComponent<Components::Model>(gate);
		model->ModelFile = "Models/Gate/Gate/Gate.obj";		
		auto physics = AddComponent<Components::Physics>(gate);
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;
		auto move = AddComponent<Components::Move>(gate);
		move->Speed = 5.f;
		move->GoalPosition = glm::vec3(0, 9.02345f, 0);
		move->StartPosition = glm::vec3(0, 0, 0);
		{
			auto mainShape = CreateEntity(gate);
			auto transform = AddComponent<Components::Transform>(mainShape);
			transform->Position = glm::vec3(0.f, 3.09776f, 0.f);
			auto box = AddComponent<Components::BoxShape>(mainShape);
			box->Width = 6.963f;
			box->Height = 2.478f;
			box->Depth = 0.522f;
			CommitEntity(mainShape);
		}
		{
			auto lowerShape = CreateEntity(gate);
			auto transform = AddComponent<Components::Transform>(lowerShape);
			transform->Position = glm::vec3(0.f, 0.35f, 0.f);
			auto box = AddComponent<Components::BoxShape>(lowerShape);
			box->Width = 6.963f;
			box->Height = 0.308f;
			box->Depth = 0.222f;
			CommitEntity(lowerShape);
		}
		CommitEntity(gate);
	}

	{
		auto gateTrigger = CreateEntity(gateParent);
		auto transform = AddComponent<Components::Transform>(gateTrigger);
		transform->Position = glm::vec3(0, 2.8241, 0);
		auto trigger = AddComponent<Components::Trigger>(gateTrigger);
		trigger->TeamID = teamID;
		auto triggerMove = AddComponent<Components::TriggerMove>(gateTrigger);
		triggerMove->Entity = gate;
		auto teamComponent = AddComponent<Components::Team>(gateTrigger);
		teamComponent->TeamID = teamID;
		{
			auto shape = CreateEntity(gateTrigger);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(0.f, 2.8241f, 0.f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 6.963f;
			box->Height = 3.104f;
			box->Depth = 10.356f;
			CommitEntity(shape);
		}
		CommitEntity(gateTrigger);
	}
}

EntityID GameWorld::CreateTower(EntityID parent, glm::vec3 pos, int teamID)
{
	auto towerBase = CreateEntity(parent);
	auto transform = AddComponent<Components::Transform>(towerBase);
	transform->Position = pos;

	auto towerComponent = AddComponent<Components::Tower>(towerBase);

 	auto physics = AddComponent<Components::Physics>(towerBase);
 	physics->Mass = 100.f;
 	physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;

	auto model = AddComponent<Components::Model>(towerBase);
	model->ModelFile = "Models/Turret/Base/Base.obj";

	auto health = AddComponent<Components::Health>(towerBase);
	health->Amount = 50.f;

 	auto shape = CreateEntity(towerBase);
 	auto shapetransform = AddComponent<Components::Transform>(shape);
 	auto shapemesh = AddComponent<Components::MeshShape>(shape);
 	shapemesh->ResourceName = "Models/Turret/Base/Base.obj";

	CommitEntity(shape);
	{
		auto towerGunRotation = CreateEntity(towerBase);
		auto transform = AddComponent<Components::Transform>(towerGunRotation);
		transform->Position = glm::vec3(0.f, 5.889f-0.92791f, 0.f);


		auto towerGunBaseModel = CreateEntity(towerGunRotation);
		{
			auto transform = AddComponent<Components::Transform>(towerGunBaseModel);
			transform->Orientation = glm::quat(glm::vec3(0, -glm::pi<float>() / 2.f, 0));
			auto model = AddComponent<Components::Model>(towerGunBaseModel);
			model->ModelFile = "Models/Turret/GunRotation/GunRotation.obj";
		}
		CommitEntity(towerGunBaseModel);

		auto health = AddComponent<Components::Health>(towerGunRotation);
		health->Amount = 50.f;
		
		{
			auto towerGun = CreateEntity(towerGunRotation);
			auto transform = AddComponent<Components::Transform>(towerGun);
			transform->Position = glm::vec3(0.f, 4.12801f, 1.18125f);
			//transform->Orientation = glm::quat(glm::vec3(0, -glm::pi<float>() / 2.f, 0));

#pragma region Turret_Shot_Template

	{
		auto shot = CreateEntity(towerGun);
		auto transform = AddComponent<Components::Transform>(shot);
		transform->Position = glm::vec3(0.f, 0.f, -4.37612f);
		transform->Orientation = glm::angleAxis(-glm::pi<float>() / 2.f, glm::vec3(1, 0, 0));
		transform->Scale = glm::vec3(2.f);
		AddComponent<Components::Template>(shot);
		auto physics = AddComponent<Components::Physics>(shot);
		physics->Mass = 25.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
		physics->CollisionEvent = true;
		auto modelComponent = AddComponent<Components::Model>(shot);
		modelComponent->ModelFile = "Models/Placeholders/rocket/Rocket.obj";
		auto TurretShotComponent = AddComponent<Components::TurretShot>(shot);
		TurretShotComponent->Damage = 20.f;
		TurretShotComponent->ExplosionRadius = 30.f;
		TurretShotComponent->ExplosionStrength = 300000.f;
		{
			auto shape = CreateEntity(shot);
			auto transform = AddComponent<Components::Transform>(shape);
			auto boxShape = AddComponent<Components::BoxShape>(shape);
			boxShape->Width = 0.5f;
			boxShape->Height = 0.5f;
			boxShape->Depth = 0.5f;
			CommitEntity(shape);
		}
		CommitEntity(shot);
		m_ShotTemplate = shot;
	}

#pragma endregion

			auto towerGunModel = CreateEntity(towerGun);
			{
				auto transform = AddComponent<Components::Transform>(towerGunModel);
				transform->Orientation = glm::quat(glm::vec3(0, -glm::pi<float>() / 2.f, 0));
				auto model = AddComponent<Components::Model>(towerGunModel);
				model->ModelFile = "Models/Turret/Gun/Gun.obj";
			}
			CommitEntity(towerGunModel);

			auto health = AddComponent<Components::Health>(towerGun);
			health->Amount = 50.f; 

			CommitEntity(towerGun);
			towerComponent->Gun = towerGun;
		}

		CommitEntity(towerGunRotation);
		towerComponent->GunBase = towerGunRotation;
	}

	towerComponent->ID = teamID;

	auto turretComponent = AddComponent<Components::Turret>(towerBase);
	turretComponent->ShotTemplate = m_ShotTemplate;
	turretComponent->ShotSpeed = 40.f;

	CommitEntity(towerBase);

	return towerBase;

}

EntityID GameWorld::CreateWall(EntityID parent, glm::vec3 pos, glm::quat orientation)
{
	auto wall = CreateEntity(parent);
	auto transform = AddComponent<Components::Transform>(wall);
	transform->Position = pos;
	transform->Orientation = orientation;

	auto physics = AddComponent<Components::Physics>(wall);
	physics->Mass = 100.f;
	physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;

	auto walldebris = AddComponent<Components::Wall>(wall);
	walldebris->Walldebris = m_WallDebrisTemplates;

	auto model = AddComponent<Components::Model>(wall);
	model->ModelFile = "Models/Wall/WallWhole/Wall.obj";
	auto damageModel = AddComponent<Components::DamageModel>(wall);
	damageModel->ModelFile = "Models/Wall/WallWhole/WallCracks.obj";

	auto health = AddComponent<Components::Health>(wall);
	health->Amount = 50.f;
	health->VulnerableToSplash = false;

	auto shape = CreateEntity(wall);
	auto shapetransform = AddComponent<Components::Transform>(shape);
	auto shapemesh = AddComponent<Components::MeshShape>(shape);
	shapemesh->ResourceName = "Models/Wall/WallCollision/Collision.obj";

	CommitEntity(shape);
	CommitEntity(wall);

	return wall;

}

void GameWorld::CreateTerrain()
{
	{
		auto road_middle = CreateEntity();
		auto transform = AddComponent<Components::Transform>(road_middle);
		auto model = AddComponent<Components::Model>(road_middle);
		model->ModelFile = "Models/TerrainFiveIstles/Roads/MiddleRoad.obj";
		auto physics = AddComponent<Components::Physics>(road_middle);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		auto groundshape = CreateEntity(road_middle);
		auto transformshape = AddComponent<Components::Transform>(groundshape);
		auto meshShape = AddComponent<Components::MeshShape>(groundshape);
		meshShape->ResourceName = "Models/TerrainFiveIstles/Roads/MiddleRoad.obj";


		CommitEntity(groundshape);
		CommitEntity(road_middle);
	}

	

	{
		/*auto water = CreateEntity();
		auto transform = AddComponent<Components::Transform>(water);
		transform->Position = glm::vec3(0, -35, 0);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		transform->Scale = glm::vec3(5000.f, 10.f, 5000.f);
		auto model = AddComponent<Components::Model>(water);
		model->ModelFile = "Models/Placeholders/PhysicsTest/Cube.obj";
		auto blendmap = AddComponent<Components::BlendMap>(water);
		blendmap->TextureRed = "Textures/Skybox/Sky34/bottom.jpg";
		blendmap->TextureGreen = "Textures/Skybox/Sky34/bottom.jpg";
		blendmap->TextureBlue = "Textures/Skybox/Sky34/bottom.jpg";
		blendmap->TextureRepeats = 1.f;

		auto physics = AddComponent<Components::Physics>(water);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;
		{

		auto groundshape = CreateEntity(water);
		auto box = AddComponent<Components::BoxShape>(groundshape);
		box->Depth = 250.f;
		box->Height = 5.f;
		box->Width = 250.f;


		CommitEntity(groundshape);
		}
		CommitEntity(water);*/

		{
			auto water = CreateEntity();
			auto transform = AddComponent<Components::Transform>(water);

			{
				auto waves = CreateEntity(water);
				auto transform = AddComponent<Components::Transform>(water);
				auto model = AddComponent<Components::Model>(water);
				model->ModelFile = "Models/Water/Waves.obj";
				CommitEntity(waves);
			}

			CommitEntity(water);
		}
	}

	{
		auto ground_middle = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground_middle);
		auto model = AddComponent<Components::Model>(ground_middle);
		model->ModelFile = "Models/TerrainFiveIstles/Middle.obj";
		model->AverageNormals = true;
		auto blendmap = AddComponent<Components::BlendMap>(ground_middle);
		blendmap->TextureRed = "Textures/Ground/SoilBeach0087_11_S.jpg";
		blendmap->TextureRedNormal = "Textures/Ground/SoilBeach0087_11_SNM.png";
		blendmap->TextureGreen = "Textures/Ground/Grass0126_2_S.jpg";
		blendmap->TextureGreenNormal = "Textures/Ground/Grass0126_2_SNM.png";
		blendmap->TextureBlue = "Textures/Ground/Cliffs2.png";
		blendmap->TextureBlueNormal = "Textures/Ground/Cliffs2NM.png";
		blendmap->TextureRepeats = 30.f;

		auto physics = AddComponent<Components::Physics>(ground_middle);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		auto groundshape = CreateEntity(ground_middle);
		auto transformshape = AddComponent<Components::Transform>(groundshape);
		auto meshShape = AddComponent<Components::MeshShape>(groundshape);
		meshShape->ResourceName = "Models/TerrainFiveIstles/Middle.obj";


		CommitEntity(groundshape);
		CommitEntity(ground_middle);
	}

	{
		auto decoration_middle = CreateEntity();
		auto transform = AddComponent<Components::Transform>(decoration_middle);
		auto model = AddComponent<Components::Model>(decoration_middle);
		model->ModelFile = "Models/TerrainFiveIstles/Decoration/Middle.obj";
		CommitEntity(decoration_middle);
	}

	CreateBase(glm::quat(), 1);
	CreateBase(glm::quat(glm::vec3(0, glm::pi<float>(), 0)), 2);

	{
		auto tree = CreateEntity();
		auto transform = AddComponent<Components::Transform>(tree);
		transform->Position = glm::vec3(0, -10, 0);

		auto physics = AddComponent<Components::Physics>(tree);
		physics->Mass = 100.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Keyframed;
		physics->CollisionEvent = false;
		//physics->LinearDamping = 3.f;
		physics->CalculateCenterOfMass = true;

		{
			auto stem = CreateEntity(tree);
			auto transform = AddComponent<Components::Transform>(stem);
			transform->Position = glm::vec3(0, -2.40906f, 0);
			auto model = AddComponent<Components::Model>(stem);
			model->ModelFile = "Models/Tree/Stem/Stem.obj";
		}

		{
			auto leafs = CreateEntity(tree);
			auto transform = AddComponent<Components::Transform>(leafs);
			transform->Position = glm::vec3(0, -2.45511f, 0);
			auto model = AddComponent<Components::Model>(leafs);
			model->ModelFile = "Models/Tree/Leafs/Leafs.obj";
			model->Transparent = true;
			CommitEntity(leafs);
		}

		{
			auto shape = CreateEntity(tree);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(0.f, 0.f, 0.f);
			auto mesh = AddComponent<Components::MeshShape>(shape);
			mesh->ResourceName = "Models/Tree/Collision/Collision.obj";

			CommitEntity(shape);
		}


		/*{
		auto shape = CreateEntity(tree);
		auto transform = AddComponent<Components::Transform>(shape);
		auto box = AddComponent<Components::BoxShape>(shape);
		transform->Position = glm::vec3(0.f, 0, 0.f);
		box->Width = 0.296f;
		box->Height = 2.511f;
		box->Depth = 0.296f;

		CommitEntity(shape);
		}

		{
		auto shape = CreateEntity(tree);
		auto transform = AddComponent<Components::Transform>(shape);
		auto sphere = AddComponent<Components::SphereShape>(shape);
		transform->Position = glm::vec3(0, 2.04506f, 0.f);
		sphere->Radius = 1.389f;

		CommitEntity(shape);
		}*/

		CommitEntity(tree);
	}
}

void GameWorld::CreateBase(glm::quat orientation, int teamID)
{
	auto base = CreateEntity();
	auto transform = AddComponent<Components::Transform>(base);
	transform->Orientation = orientation;
#pragma region Terrain

	{
		auto ground_small = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(ground_small);
		auto model = AddComponent<Components::Model>(ground_small);
		model->ModelFile = "Models/TerrainFiveIstles/Small.obj";
		model->AverageNormals = true;
		auto blendmap = AddComponent<Components::BlendMap>(ground_small);
		blendmap->TextureRed = "Textures/Ground/SoilBeach0087_11_S.jpg";
		blendmap->TextureRedNormal = "Textures/Ground/SoilBeach0087_11_SNM.png";
		blendmap->TextureGreen = "Textures/Ground/Grass0126_2_S.jpg";
		blendmap->TextureGreenNormal = "Textures/Ground/Grass0126_2_SNM.png";
		blendmap->TextureBlue = "Textures/Ground/Cliffs2.png";
		blendmap->TextureBlueNormal = "Textures/Ground/Cliffs2NM.png";
		blendmap->TextureRepeats = 30.f;

		auto physics = AddComponent<Components::Physics>(ground_small);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		auto groundshape = CreateEntity(ground_small);
		auto transformshape = AddComponent<Components::Transform>(groundshape);
		auto meshShape = AddComponent<Components::MeshShape>(groundshape);
		meshShape->ResourceName = "Models/TerrainFiveIstles/Small.obj";
		CommitEntity(groundshape);

		CommitEntity(ground_small);
	}

	{
		auto decoration_small = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(decoration_small);
		auto model = AddComponent<Components::Model>(decoration_small);
		model->ModelFile = "Models/TerrainFiveIstles/Decoration/Small.obj";
		CommitEntity(decoration_small);
	}

	{
		auto road_small = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(road_small);
		auto model = AddComponent<Components::Model>(road_small);
		model->ModelFile = "Models/TerrainFiveIstles/Roads/SmallRoad.obj";
		auto physics = AddComponent<Components::Physics>(road_small);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		auto groundshape = CreateEntity(road_small);
		auto transformshape = AddComponent<Components::Transform>(groundshape);
		auto meshShape = AddComponent<Components::MeshShape>(groundshape);
		meshShape->ResourceName = "Models/TerrainFiveIstles/Roads/SmallRoad.obj";


		CommitEntity(groundshape);
		CommitEntity(road_small);
	}

	{
		auto bridge_middle = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(bridge_middle);
		auto model = AddComponent<Components::Model>(bridge_middle);
		model->ModelFile = "Models/TerrainFiveIstles/Bridges/MiddleBridge.obj";
		model->AverageNormals = false;
		auto physics = AddComponent<Components::Physics>(bridge_middle);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		{
			auto shape = CreateEntity(bridge_middle);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(70.31883f, 26.42735f, 0.50379f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 26.324f;
			box->Height = 2.727f;
			box->Depth = 6.798f;
			CommitEntity(shape);
		}
		{
			auto shape = CreateEntity(bridge_middle);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(70.43447f, 29.23594f, 7.37155f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 27.724f;
			box->Height = 1.334f;
			box->Depth = 0.571f;
			CommitEntity(shape);
		}
		{
			auto shape = CreateEntity(bridge_middle);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(70.43447f, 29.26906f, -6.68171f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 27.724f;
			box->Height = 1.334f;
			box->Depth = 0.571f;
			CommitEntity(shape);
		}
		{
			auto shape = CreateEntity(bridge_middle);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(70.36976f, 33.56011f, 9.68543f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 1.8f;
			box->Height = 15.984f;
			box->Depth = 1.456f;
			CommitEntity(shape);
		}
		{
			auto shape = CreateEntity(bridge_middle);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(70.36976f, 33.56011f, -8.67785f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 1.799f;
			box->Height = 15.983f;
			box->Depth = 1.456f;
			CommitEntity(shape);
		}
		{
			auto shape = CreateEntity(bridge_middle);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(99.16043f, 25.0318f, 0.54486f);
			transform->Orientation = glm::angleAxis(glm::radians(-9.087f), glm::vec3(0, 0, 1));
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 3.566f;
			box->Height = 3.566f;
			box->Depth = 7.632f;
			CommitEntity(shape);
		}
		CommitEntity(bridge_middle);
	}

	// Banners
	std::string bannerModel = "Models/Banner/Blue/Blue.obj";
	if (teamID == 2)
	{
		bannerModel = "Models/Banner/Red/Red.obj";
	}

	{
		auto banner = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(banner);
		transform->Position = glm::vec3(70.2f, 50.36f, -8.68f);
		transform->Orientation = glm::quat(glm::vec3(0, glm::pi<float>(), 0));
		auto model = AddComponent<Components::Model>(banner);
		model->ModelFile = bannerModel;
		model->Transparent = true;
		CommitEntity(banner);
	}
	{
		auto banner = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(banner);
		transform->Position = glm::vec3(70.2f, 50.36f, 9.86f);
		transform->Orientation = glm::quat(glm::vec3(0, glm::pi<float>(), 0));
		auto model = AddComponent<Components::Model>(banner);
		model->ModelFile = bannerModel;
		model->Transparent = true;
		CommitEntity(banner);
	}
	{
		auto banner = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(banner);
		transform->Position = glm::vec3(166.17f, 49.711f, -8.68f);
		transform->Orientation = glm::quat(glm::vec3(0, glm::pi<float>(), 0));
		auto model = AddComponent<Components::Model>(banner);
		model->ModelFile = bannerModel;
		model->Transparent = true;
		CommitEntity(banner);
	}
	{
		auto banner = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(banner);
		auto model = AddComponent<Components::Model>(banner);
		transform->Position = glm::vec3(166.17f, 49.711f, 9.86f);
		transform->Orientation = glm::quat(glm::vec3(0, glm::pi<float>(), 0));
		model->ModelFile = bannerModel;
		model->Transparent = true;
		CommitEntity(banner);
	}


	{
		auto terrain_base = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(terrain_base);
		auto model = AddComponent<Components::Model>(terrain_base);
		model->ModelFile = "Models/TerrainFiveIstles/Base.obj";
		model->AverageNormals = true;
		auto blendmap = AddComponent<Components::BlendMap>(terrain_base);
		blendmap->TextureRed = "Textures/Ground/SoilBeach0087_11_S.jpg";
		blendmap->TextureRedNormal = "Textures/Ground/SoilBeach0087_11_SNM.png";
		blendmap->TextureGreen = "Textures/Ground/Grass0126_2_S.jpg";
		blendmap->TextureGreenNormal = "Textures/Ground/Grass0126_2_SNM.png";
		blendmap->TextureBlue = "Textures/Ground/Cliffs2.png";
		blendmap->TextureBlueNormal = "Textures/Ground/Cliffs2NM.png";
		blendmap->TextureRepeats = 30.f;

		auto physics = AddComponent<Components::Physics>(terrain_base);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		auto shape = CreateEntity(terrain_base);
		auto transformshape = AddComponent<Components::Transform>(shape);
		auto meshShape = AddComponent<Components::MeshShape>(shape);
		meshShape->ResourceName = "Models/TerrainFiveIstles/Base.obj";


		CommitEntity(shape);
		CommitEntity(terrain_base);
	}

	{
		auto ground_base = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(ground_base);
		auto model = AddComponent<Components::Model>(ground_base);
		model->ModelFile = "Models/TerrainFiveIstles/BaseGround.obj";
		auto physics = AddComponent<Components::Physics>(ground_base);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		auto shape = CreateEntity(ground_base);
		auto transformshape = AddComponent<Components::Transform>(shape);
		auto meshShape = AddComponent<Components::MeshShape>(shape);
		meshShape->ResourceName = "Models/TerrainFiveIstles/BaseGround.obj";
		
		CommitEntity(shape);
		CommitEntity(ground_base);
	}

	{
		auto decoration_base = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(decoration_base);
		auto model = AddComponent<Components::Model>(decoration_base);
		model->ModelFile = "Models/TerrainFiveIstles/Decoration/Base.obj";
		CommitEntity(decoration_base);
	}

	{
		auto road_base = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(road_base);
		auto model = AddComponent<Components::Model>(road_base);
		model->ModelFile = "Models/TerrainFiveIstles/Roads/BaseRoad.obj";
		auto physics = AddComponent<Components::Physics>(road_base);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		auto groundshape = CreateEntity(road_base);
		auto transformshape = AddComponent<Components::Transform>(groundshape);
		auto meshShape = AddComponent<Components::MeshShape>(groundshape);
		meshShape->ResourceName = "Models/TerrainFiveIstles/Roads/BaseRoad.obj";


		CommitEntity(groundshape);
		CommitEntity(road_base);
	}

	{
		auto bridge_base = CreateEntity(base);
		auto transform = AddComponent<Components::Transform>(bridge_base);
		auto model = AddComponent<Components::Model>(bridge_base);
		model->ModelFile = "Models/TerrainFiveIstles/Bridges/BaseBridge.obj";
		model->AverageNormals = false;
		auto physics = AddComponent<Components::Physics>(bridge_base);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		{
			auto shape = CreateEntity(bridge_base);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(166.19089f, 25.99645f, -0.50379f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 27.723f;
			box->Height = 2.727f;
			box->Depth = 6.798f;
			CommitEntity(shape);
		}
		{
			auto shape = CreateEntity(bridge_base);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(166.19089f, 28.37571f, -6.68171f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 27.724f;
			box->Height = 1.334f;
			box->Depth = 0.571f;
			CommitEntity(shape);
		}
		{
			auto shape = CreateEntity(bridge_base);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(166.19089f, 28.37571f, 7.68929f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 27.724f;
			box->Height = 1.334f;
			box->Depth = 0.571f;
			CommitEntity(shape);
		}
		{
			auto shape = CreateEntity(bridge_base);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(166.12617f, 33.56011f, -8.67785f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 1.8;
			box->Height = 15.984f;
			box->Depth = 1.456f;
			CommitEntity(shape);
		}
		{
			auto shape = CreateEntity(bridge_base);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(166.12617f, 33.56011f, 9.68543f);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 1.8;
			box->Height = 15.984f;
			box->Depth = 1.456f;
			CommitEntity(shape);
		}

		CommitEntity(bridge_base);
	}
#pragma endregion Terrain

	// Small island
	CreateGate(base, glm::vec3(113.59f, 29.02f, 0.32f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)), teamID);
	// Z
	glm::quat yawRot = glm::angleAxis(glm::pi<float>() / 2.f, glm::vec3(0, 1, 0));
	CreateWall(base, glm::vec3(113.59f, 28.79635f, 15.26201f), yawRot); // glm::quat(glm::vec3(glm::radians<float>(-9.21f), 0, 0)));
	CreateWall(base, glm::vec3(113.60f, 28.05425f, 24.41458f), glm::angleAxis(glm::radians<float>(9.21f), glm::vec3(1, 0, 0)) * yawRot);
	CreateWall(base, glm::vec3(113.61f, 25.42271f, 32.34383f), glm::angleAxis(glm::radians<float>(27.f), glm::vec3(1, 0, 0)) * yawRot);
	CreateWall(base, glm::vec3(113.62f, 20.54767f, 38.68277f), glm::angleAxis(glm::radians<float>(47.9f), glm::vec3(1, 0, 0)) * yawRot);
	// -Z
	CreateWall(base, glm::vec3(113.59f, 28.734f, -13.85581f), glm::angleAxis(glm::radians<float>(-3.634f), glm::vec3(1, 0, 0)) * yawRot);
	CreateWall(base, glm::vec3(113.60f, 27.166f, -22.438f), glm::angleAxis(glm::radians<float>(-17.202f), glm::vec3(1, 0, 0)) * yawRot);
	CreateWall(base, glm::vec3(113.61f, 23.5952f, -30.50557f), glm::angleAxis(glm::radians<float>(-30.638f), glm::vec3(1, 0, 0)) * yawRot);
	CreateWall(base, glm::vec3(113.62f, 18.30851f, -36.56623f), glm::angleAxis(glm::radians<float>(-51.542f), glm::vec3(1, 0, 0)) * yawRot);

	// -X
	CreateGate(base, glm::vec3(273.f, 40.185f, 0.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)), teamID);
	CreateWall(base, glm::vec3(273.f, 40.3f, -55.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(273.f, 40.3f, -45.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(273.f, 40.3f, -35.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(273.f, 40.3f, -25.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(273.f, 40.3f, -15.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(273.f, 40.3f, 15.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(273.f, 40.3f, 25.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateTower(base, glm::vec3(290.f, 40.3f, 15.f), teamID);

	// -Z
	CreateWall(base, glm::vec3(280.f, 40.3f, -62.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(290.f, 40.3f, -62.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(300.f, 40.3f, -62.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(310.f, 40.3f, -62.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(320.f, 40.3f, -62.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(330.f, 40.3f, -62.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(340.f, 40.3f, -62.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(350.f, 40.3f, -62.f), glm::quat(glm::vec3(0, 0, 0)));

	// X
	CreateWall(base, glm::vec3(357.f, 40.3f, -55.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(357.f, 40.3f, -45.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(357.f, 40.3f, -35.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(357.f, 40.3f, -25.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(357.f, 40.3f, -15.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(357.f, 40.3f, -5.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(357.f, 40.3f, 5.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(357.f, 40.3f, 15.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));
	CreateWall(base, glm::vec3(357.f, 40.3f, 25.f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));

	// Z
	CreateWall(base, glm::vec3(280.f, 40.3f, 32.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(290.f, 40.3f, 32.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(300.f, 40.3f, 32.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(310.f, 40.3f, 32.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(320.f, 40.3f, 32.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(330.f, 40.3f, 32.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(340.f, 40.3f, 32.f), glm::quat(glm::vec3(0, 0, 0)));
	CreateWall(base, glm::vec3(350.f, 40.3f, 32.f), glm::quat(glm::vec3(0, 0, 0)));

	CreateGarage(base, glm::vec3(323.2f, 41.4f, -10.2f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)), teamID);

	CreateFlag(base, glm::vec3(291.f, 41.7412f, -36.2f), glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)), teamID);
}

EntityID GameWorld::CreateGarage(EntityID parent, glm::vec3 Position, glm::quat orientation, int teamID)
{
	auto garage = CreateEntity(parent);
	auto transform = AddComponent<Components::Transform>(garage);
	transform->Position = Position;
	transform->Orientation = orientation;

	auto ElevatorBase = CreateEntity(garage);
	{
		auto transform = AddComponent<Components::Transform>(ElevatorBase);
		auto model = AddComponent<Components::Model>(ElevatorBase);
		model->ModelFile = "Models/SpawnRamp/Main/Main.obj";
		auto physics = AddComponent<Components::Physics>(ElevatorBase);
		physics->MotionType = Components::Physics::MotionTypeEnum::Keyframed;
		physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;

		{
			auto mainShape = CreateEntity(ElevatorBase);
			auto transform = AddComponent<Components::Transform>(mainShape);
			transform->Position = glm::vec3(0.f, 0, 0.f);
			auto mesh = AddComponent<Components::MeshShape>(mainShape);
			mesh->ResourceName = "Models/SpawnRamp/Main/CollisionShape.obj";
			CommitEntity(mainShape);
		}
		{
			auto StairsShape = CreateEntity(ElevatorBase);
			auto transform = AddComponent<Components::Transform>(StairsShape);
			transform->Position = glm::vec3(-7.48639f, 0.4809f, 3.79166f);
			auto box = AddComponent<Components::BoxShape>(StairsShape);
			box->Width = 1.211f;
			box->Height = 1.798f;
			box->Depth = 3.229f;
			CommitEntity(StairsShape);
		}
	}
	CommitEntity(ElevatorBase);

	auto leftHatch = CreateEntity(garage);
	{
		auto transform = AddComponent<Components::Transform>(leftHatch);
		transform->Position = glm::vec3(5.83103f, 1.3553f, -2.40268f);
		auto model = AddComponent<Components::Model>(leftHatch);
		model->ModelFile = "Models/SpawnRamp/LeftHatch/LeftHatch.obj";
		auto physics = AddComponent<Components::Physics>(leftHatch);
		physics->MotionType = Components::Physics::MotionTypeEnum::Keyframed;
		physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;
		auto rotate = AddComponent<Components::Rotate>(leftHatch);
		rotate->StartRotation = transform->Orientation;
		rotate->GoalRotation = transform->Orientation * glm::angleAxis(-glm::radians(110.f), glm::vec3(0, 0, 1));
		rotate->Time = 5.f;
		{
			auto shape = CreateEntity(leftHatch);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(-2.79331, 0, 0);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 2.317f;
			box->Height = 0.183f;
			box->Depth = 6.987f;
			CommitEntity(shape);
		}
		CommitEntity(leftHatch);
	}

	auto rightHatch = CreateEntity(garage);
	{
		auto transform = AddComponent<Components::Transform>(rightHatch);
		transform->Position = glm::vec3(-4.52423f, 1.3553f, -2.40268f);
		auto model = AddComponent<Components::Model>(rightHatch);
		model->ModelFile = "Models/SpawnRamp/RightHatch/RightHatch.obj";
		auto physics = AddComponent<Components::Physics>(rightHatch);
		physics->MotionType = Components::Physics::MotionTypeEnum::Keyframed;
		physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;
		auto rotate = AddComponent<Components::Rotate>(rightHatch);
		rotate->StartRotation = transform->Orientation;
		rotate->GoalRotation =  transform->Orientation * glm::angleAxis(glm::radians(110.f), glm::vec3(0, 0, 1));
		rotate->Time = 5.f;
		{
			auto shape = CreateEntity(rightHatch);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(2.79331, 0, 0);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 2.317f;
			box->Height = 0.183f;
			box->Depth = 6.987f;
			CommitEntity(shape);
		}
		CommitEntity(rightHatch);
	}

	auto elevator = CreateEntity(garage);
	{
		auto transform = AddComponent<Components::Transform>(elevator);
		transform->Position = glm::vec3(0.69385f, 1.25222f, -2.39938f) + glm::vec3(0, -9.5f, 0);
		auto model = AddComponent<Components::Model>(elevator);
		model->ModelFile = "Models/SpawnRamp/Elevator/Elevator.obj";
		auto physics = AddComponent<Components::Physics>(elevator);
		physics->MotionType = Components::Physics::MotionTypeEnum::Keyframed;
		physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;
		auto move = AddComponent<Components::Move>(elevator);
		move->StartPosition = transform->Position;
		move->GoalPosition = transform->Position + glm::vec3(0, 9.5f, 0);
		move->Speed = 1.5f;
		{
			auto shape = CreateEntity(elevator);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(0, 0, 0);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 4.754f;
			box->Height = 0.141f;
			box->Depth = 6.86f;
			CommitEntity(shape);
		}

		auto spawnPoint = CreateEntity(elevator);
		{
			auto transform = AddComponent<Components::Transform>(spawnPoint);
			transform->Position.y = 1.f;
			auto spawnPointComponent = AddComponent<Components::SpawnPoint>(spawnPoint);
			auto teamComponent = AddComponent<Components::Team>(spawnPoint);
			teamComponent->TeamID = teamID;
		}
		CommitEntity(spawnPoint);
	}
	CommitEntity(elevator);

	auto garageComponent = AddComponent<Components::Garage>(garage);
	garageComponent->DoorLeft = leftHatch;
	garageComponent->DoorRight = rightHatch;
	garageComponent->Elevator = elevator;
	CommitEntity(garage);

	{
		auto trigger = CreateEntity(garage);
		SetProperty(trigger, "Name", "BoundsTrigger");
		auto transform = AddComponent<Components::Transform>(trigger);
		transform->Position = glm::vec3(0, 44.87521f, 0);
		auto triggerComponent = AddComponent<Components::Trigger>(trigger);
		triggerComponent->TeamID = teamID;
		auto teamComponent = AddComponent<Components::Team>(trigger);
		teamComponent->TeamID = teamID;
		{
			auto shape = CreateEntity(trigger);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(0, 0, 0);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 72.98f;
			box->Height = 54.53f;
			box->Depth = 72.98f;
			CommitEntity(shape);
		}
		CommitEntity(trigger);
	}

	{
		auto trigger = CreateEntity(garage);
		SetProperty(trigger, "Name", "ElevatorShaftTrigger");
		auto transform = AddComponent<Components::Transform>(trigger);
		transform->Position = glm::vec3(0.69385f, -6.7997f, -2.0f);
		auto triggerComponent = AddComponent<Components::Trigger>(trigger);
		triggerComponent->TeamID = teamID;
		auto teamComponent = AddComponent<Components::Team>(trigger);
		teamComponent->TeamID = teamID;
		{
			auto shape = CreateEntity(trigger);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(0, 0, 0);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 2.0f;
			box->Height = 12.f;
			box->Depth = 2.0f;
			CommitEntity(shape);
		}
		CommitEntity(trigger);
	}
	
	return garage;
}

void GameWorld::CreateFlag(EntityID parent, glm::vec3 position, glm::quat orientation, int TeamID)
{
	
	auto flag = CreateEntity(parent);
	SetProperty(flag, "Name", "Flag");
	auto transform = AddComponent<Components::Transform>(flag);
	transform->Position = position;
	transform->Orientation = orientation;
	auto flagComponent = AddComponent<Components::Flag>(flag);
	auto trigger = AddComponent<Components::Trigger>(flag);
	auto team = AddComponent<Components::Team>(flag);
	team->TeamID = TeamID;
	{
		auto fish = CreateEntity(flag);
		auto transform = AddComponent<Components::Transform>(fish);
		transform->Position = glm::vec3(-1.3f, 1.0, 0);
		auto model = AddComponent<Components::Model>(fish);
		model->ModelFile = "Models/Flag/LeFish/Salmon.obj";
		CommitEntity(fish);
	}

	{
		auto rod = CreateEntity(flag);
		auto transform = AddComponent<Components::Transform>(rod);
		transform->Position = glm::vec3(-1.3f, 1.0, 0);
		auto model = AddComponent<Components::Model>(rod);
		model->ModelFile = "Models/Flag/FishingRod/FishingRod.obj";
		CommitEntity(rod);
	}
	
	{
		auto shape = CreateEntity(flag);
		auto transform = AddComponent<Components::Transform>(shape);
		transform->Position = glm::vec3(-0.9f, 0.9f, 0);
		auto box = AddComponent<Components::BoxShape>(shape);
		box->Width = 1.1f;
		box->Depth = 0.7f;
		box->Height = 3.9f;
		CommitEntity(shape);
	}
	CommitEntity(flag);
	
}
