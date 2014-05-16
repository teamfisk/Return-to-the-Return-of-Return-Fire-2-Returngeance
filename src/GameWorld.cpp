#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();

	m_ResourceManager.Preload("Model", "Models/Placeholders/PhysicsTest/Plane.obj");
	m_ResourceManager.Preload("Model", "Models/Placeholders/PhysicsTest/ArrowCube.obj");

	BindKey(GLFW_KEY_W, "cam_vertical", 1.f);
	BindKey(GLFW_KEY_S, "cam_vertical", -1.f);
	BindKey(GLFW_KEY_A, "cam_horizontal", -1.f);
	BindKey(GLFW_KEY_D, "cam_horizontal", 1.f);
	BindGamepadAxis(Gamepad::Axis::LeftX, "horizontal", 1.f);
	BindGamepadAxis(Gamepad::Axis::LeftY, "vertical", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightX, "horizontal", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightY, "vertical", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightTrigger, "normal", 1.f);
	BindGamepadAxis(Gamepad::Axis::LeftTrigger, "normal", -1.f);
	BindKey(GLFW_KEY_SPACE, "cam_normal", 1.f);
	BindKey(GLFW_KEY_LEFT_CONTROL, "cam_normal", -1.f);

	BindKey(GLFW_KEY_LEFT_SHIFT, "cam_speed", 1.f);
	BindKey(GLFW_KEY_LEFT_ALT, "cam_speed", -1.f);

	BindMouseButton(GLFW_MOUSE_BUTTON_1, "cam_attack", 1.f);
	
	/*BindKey(GLFW_KEY_UP, "barrel_rotation", 1.f);
	BindKey(GLFW_KEY_DOWN, "barrel_rotation", -1.f);
	BindKey(GLFW_KEY_LEFT, "tower_rotation", -1.f);
	BindKey(GLFW_KEY_RIGHT, "tower_rotation", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightX, "tower_rotation", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightY, "barrel_rotation", 1.f);

	BindKey(GLFW_KEY_SPACE, "handbrake", 1.f);
	BindGamepadButton(Gamepad::Button::A, "handbrake", 1.f);

	BindKey(GLFW_KEY_Z, "shoot", 1.f);*/
	
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

	


	{
		auto ground = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground, "Transform");
		transform->Position = glm::vec3(0, 0, 0);
		//transform->Scale = glm::vec3(400.0f, 10.0f, 400.0f);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ground, "Model");
		//model->ModelFile = "Models/TestScene/testScene.obj";
		model->ModelFile = "Models/Placeholders/Terrain/Terrain2.obj";
		
		auto physics = AddComponent<Components::Physics>(ground, "Physics");
		physics->Mass = 10;
		physics->Static = true;


		auto groundshape = CreateEntity(ground);
		auto transformshape = AddComponent<Components::Transform>(groundshape, "Transform");
		auto meshShape = AddComponent<Components::MeshShape>(groundshape, "MeshShape");
		meshShape->ResourceName = "Models/Placeholders/Terrain/Terrain2.obj";
		//meshShape->ResourceName = "Models/TestScene/testScene.obj";
		
		CommitEntity(groundshape);
		CommitEntity(ground);
	}

	auto heli = CreateEntity();
	{
		{
		auto transform = AddComponent<Components::Transform>(heli, "Transform");
		transform->Position = glm::vec3(0.f, 3.f, 10.f);
		auto physics = AddComponent<Components::Physics>(heli, "Physics");
		physics->Mass = 3000;
		auto heliComponent = AddComponent<Components::HelicopterSteering>(heli, "HelicopterSteering");
		}

		auto model = CreateEntity(heli);
		{
		auto transform = AddComponent<Components::Transform>(model, "Transform");
		transform->Orientation = glm::quat(glm::vec3(0, -glm::pi<float>()/2.f, 0.f));
		auto modelComponent = AddComponent<Components::Model>(model, "Model");
		modelComponent->ModelFile = "Models/Heli/Chassi/Chassi.obj";
		}
		CommitEntity(model);

		auto shape = CreateEntity(heli);
		{
		auto transform = AddComponent<Components::Transform>(shape, "Transform");
		auto box = AddComponent<Components::BoxShape>(shape, "BoxShape");
		box->Width = 5.f;
		box->Height = 1.8f;
		box->Depth = 7.f;
		}
		CommitEntity(shape);

		
	}
	CommitEntity(heli);

	auto camera = CreateEntity(heli);
	{
		auto transform = AddComponent<Components::Transform>(camera, "Transform");
		transform->Position.z = 10.f;
		transform->Position.y = 5.f;
		//transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
		auto cameraComp = AddComponent<Components::Camera>(camera, "Camera");
		cameraComp->FarClip = 2000.f;
		auto freeSteering = AddComponent<Components::FreeSteering>(camera, "FreeSteering");
		CommitEntity(camera);
	}
}

void GameWorld::Update(double dt)
{
	World::Update(dt);
}

void GameWorld::RegisterComponents()
{
	m_ComponentFactory.Register("Transform", []() { return new Components::Transform(); }); 
	m_ComponentFactory.Register("Template", []() { return new Components::Template(); });	
}

void GameWorld::RegisterSystems()
{
	m_SystemFactory.Register("TransformSystem", [this]() { return new Systems::TransformSystem(this, m_EventBroker); });
	//m_SystemFactory.Register("LevelGenerationSystem", [this]() { return new Systems::LevelGenerationSystem(this); });
	m_SystemFactory.Register("InputSystem", [this]() { return new Systems::InputSystem(this, m_EventBroker); });
	m_SystemFactory.Register("DebugSystem", [this]() { return new Systems::DebugSystem(this, m_EventBroker); });
	//m_SystemFactory.Register("CollisionSystem", [this]() { return new Systems::CollisionSystem(this); });
	m_SystemFactory.Register("ParticleSystem", [this]() { return new Systems::ParticleSystem(this, m_EventBroker); });
	//m_SystemFactory.Register("PlayerSystem", [this]() { return new Systems::PlayerSystem(this); });
	m_SystemFactory.Register("FreeSteeringSystem", [this]() { return new Systems::FreeSteeringSystem(this, m_EventBroker); });
	m_SystemFactory.Register("TankSteeringSystem", [this]() { return new Systems::TankSteeringSystem(this, m_EventBroker); });
	m_SystemFactory.Register("HelicopterSteeringSystem", [this]() { return new Systems::HelicopterSteeringSystem(this, m_EventBroker); });
	m_SystemFactory.Register("SoundSystem", [this]() { return new Systems::SoundSystem(this, m_EventBroker); });
	m_SystemFactory.Register("PhysicsSystem", [this]() { return new Systems::PhysicsSystem(this, m_EventBroker); });
	m_SystemFactory.Register("RenderSystem", [this]() { return new Systems::RenderSystem(this, m_EventBroker, m_Renderer); });
}

void GameWorld::AddSystems()
{
	AddSystem("TransformSystem");
	//AddSystem("LevelGenerationSystem");
	AddSystem("InputSystem");
	AddSystem("DebugSystem");
	//AddSystem("CollisionSystem");
	AddSystem("ParticleSystem");
	//AddSystem("PlayerSystem");
	AddSystem("FreeSteeringSystem");
	AddSystem("TankSteeringSystem");
	AddSystem("HelicopterSteeringSystem");
	AddSystem("SoundSystem");
	AddSystem("PhysicsSystem");
	AddSystem("RenderSystem");
}

void GameWorld::BindKey(int keyCode, std::string command, float value)
{
	Events::BindKey e;
	e.KeyCode = keyCode;
	e.Command = command;
	e.Value = value;
	m_EventBroker->Publish(e);
}

void GameWorld::BindMouseButton(int button, std::string command, float value)
{
	Events::BindMouseButton e;
	e.Button = button;
	e.Command = command;
	e.Value = value;
	m_EventBroker->Publish(e);
}

void GameWorld::BindGamepadAxis(Gamepad::Axis axis, std::string command, float value)
{
	Events::BindGamepadAxis e;
	e.Axis = axis;
	e.Command = command;
	e.Value = value;
	m_EventBroker->Publish(e);
}

void GameWorld::BindGamepadButton(Gamepad::Button button, std::string command, float value)
{
	Events::BindGamepadButton e;
	e.Button = button;
	e.Command = command;
	e.Value = value;
	m_EventBroker->Publish(e);
}
