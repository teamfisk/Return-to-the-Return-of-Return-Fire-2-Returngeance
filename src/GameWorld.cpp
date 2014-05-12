#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();

	m_ResourceManager.Preload("Model", "Models/Placeholders/PhysicsTest/Plane.obj");
	m_ResourceManager.Preload("Model", "Models/Placeholders/PhysicsTest/ArrowCube.obj");

	BindKey(GLFW_KEY_W, "+forward");
	BindKey(GLFW_KEY_S, "+backward");
	BindKey(GLFW_KEY_A, "+left");
	BindKey(GLFW_KEY_D, "+right");
	BindKey(GLFW_KEY_SPACE, "+up");
	BindKey(GLFW_KEY_LEFT_CONTROL, "+down");
	BindKey(GLFW_KEY_LEFT_ALT, "+slow");
	BindKey(GLFW_KEY_LEFT_SHIFT, "+fast");
	BindMouseButton(GLFW_MOUSE_BUTTON_1, "+attack");
	BindMouseButton(GLFW_MOUSE_BUTTON_2, "+attack2");
	BindMouseButton(GLFW_MOUSE_BUTTON_3, "+attack3");

	RegisterComponents();

	{
		auto camera = CreateEntity();
		auto transform = AddComponent<Components::Transform>(camera, "Transform");
		transform->Position.z = 20.f;
		transform->Position.y = 20.f;
		//transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
		auto cameraComp = AddComponent<Components::Camera>(camera, "Camera");
		cameraComp->FarClip = 2000.f;
		auto freeSteering = AddComponent<Components::FreeSteering>(camera, "FreeSteering");
		CommitEntity(camera);
	}


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

	{
		auto camera = CreateEntity();
		auto transform = AddComponent<Components::Transform>(camera, "Transform");
		transform->Position.z = 20.f;
		transform->Position.y = 10.f;
		transform->Orientation = glm::quat(glm::vec3(-glm::pi<float>() / 8.f, 0.f, 0.f));
		auto cameraComp = AddComponent<Components::Camera>(camera, "Camera");
		cameraComp->FarClip = 2000.f;
		AddComponent(camera, "Input");
		auto freeSteering = AddComponent<Components::FreeSteering>(camera, "FreeSteering");
		CommitEntity(camera);
	}

	/*{
		auto jeep = CreateEntity();
		auto transform = AddComponent<Components::Transform>(jeep, "Transform");
		transform->Position = glm::vec3(0, 5, 0);
		transform->Orientation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(0, 1, 0));
		auto physics = AddComponent<Components::Physics>(jeep, "Physics");
		physics->Mass = 1800;
		physics->Static = false;
		auto vehicle = AddComponent<Components::Vehicle>(jeep, "Vehicle");
		AddComponent<Components::Input>(jeep, "Input");

		{
			auto shape = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(shape, "Transform");
			auto meshShape = AddComponent<Components::MeshShape>(shape, "MeshShape");
			meshShape->ResourceName = "Models/Jeep/Chassi/ChassiCollision.obj";
			CommitEntity(shape);

			// 		auto box = AddComponent<Components::Box>(jeep, "Box");
			// 		box->Width = 1.487f;
			// 		box->Height = 0.727f;
			// 		box->Depth = 2.594f;

		}

		{
			auto chassis = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(chassis, "Transform");
			transform->Position = glm::vec3(0, 0, 0); // 0.6577f
			auto model = AddComponent<Components::Model>(chassis, "Model");
			model->ModelFile = "Models/Jeep/Chassi/chassi.obj";
		}

		{
			auto lightentity = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(lightentity, "Transform");
			transform->Position = glm::vec3(0, 15, 0);
			auto light = AddComponent<Components::PointLight>(lightentity, "PointLight");
			light->Diffuse = glm::vec3(128.f/255.f, 172.f/255.f, 242.f/255.f);
			light->Specular = glm::vec3(1.f);
			light->constantAttenuation = 0.3f;
			light->linearAttenuation = 0.003f;
			light->quadraticAttenuation = 0.002f;
		}


		//Create wheels
		float wheelOffset = 0.4f;
		float springLength = 0.3f;
		float suspensionStrength = 35.f;
		{
			auto wheel = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(1.9f, 0.5546f - wheelOffset, -0.9242f);
			transform->Scale = glm::vec3(1.0f);
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Jeep/WheelFront/wheelFront.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 50;
			Wheel->Radius = 0.837f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 4.f;
			Wheel->ConnectedToHandbrake = true;
			CommitEntity(wheel);
		}

		{
			auto wheel = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(-1.9f, 0.5546f - wheelOffset, -0.9242f);
			transform->Scale = glm::vec3(1.0f);
			transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1));
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Jeep/WheelFront/wheelFront.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 50;
			Wheel->Radius = 0.837f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 4.f;
			Wheel->ConnectedToHandbrake = true;
			CommitEntity(wheel);
		}

		{
			auto wheel = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(0.2726f, 0.2805f - wheelOffset, 1.9307f);
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Jeep/WheelBack/wheelBack.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 50;
			Wheel->Radius = 0.737f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 4.f;
			Wheel->ConnectedToHandbrake = true;
			CommitEntity(wheel);
		}

		{
			auto wheel = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(-0.2726f, 0.2805f - wheelOffset, 1.9307f);
			transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1));
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Jeep/WheelBack/wheelBack.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 50;
			Wheel->Radius = 0.737f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 4.f;
			Wheel->ConnectedToHandbrake = true;
			CommitEntity(wheel);
		}

		CommitEntity(jeep);
	}*/


	{
		auto tank = CreateEntity();
		auto transform = AddComponent<Components::Transform>(tank, "Transform");
		transform->Position = glm::vec3(0, 5, 0);
		transform->Orientation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(0, 1, 0));
		auto physics = AddComponent<Components::Physics>(tank, "Physics");
		physics->Mass = 45000;
		physics->Static = false;
		auto vehicle = AddComponent<Components::Vehicle>(tank, "Vehicle");
		vehicle->MaxTorque = 5200.f;

		AddComponent<Components::Input>(tank, "Input");

		{
			auto shape = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(shape, "Transform");
			auto meshShape = AddComponent<Components::MeshShape>(shape, "MeshShape");
			meshShape->ResourceName = "Models/Tank/Fix/ChassiCollision.obj";
			CommitEntity(shape);

			// 		auto box = AddComponent<Components::Box>(jeep, "Box");
			// 		box->Width = 1.487f;
			// 		box->Height = 0.727f;
			// 		box->Depth = 2.594f;

		}

		{
			auto chassis = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(chassis, "Transform");
			transform->Position = glm::vec3(0, 0, 0); // 0.6577f
			auto model = AddComponent<Components::Model>(chassis, "Model");
			model->ModelFile = "Models/Tank/Fix/Chassi.obj";
		}
		{
			auto top = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(top, "Transform");
			transform->Position = glm::vec3(0, 1.2, 1.95); // 0.6577f
			auto model = AddComponent<Components::Model>(top, "Model");
			model->ModelFile = "Models/Tank/Fix/Top.obj";

			{
				auto top = CreateEntity(tank);
				auto transform = AddComponent<Components::Transform>(top, "Transform");
				transform->Position = glm::vec3(0, 1, 0.5); // 0.6577f
				auto model = AddComponent<Components::Model>(top, "Model");
				model->ModelFile = "Models/Tank/Fix/Barrel.obj";
			}
		}

		{
			auto lightentity = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(lightentity, "Transform");
			transform->Position = glm::vec3(0, 15, 0);
			auto light = AddComponent<Components::PointLight>(lightentity, "PointLight");
			light->Diffuse = glm::vec3(128.f/255.f, 172.f/255.f, 242.f/255.f);
			light->Specular = glm::vec3(1.f);
			light->constantAttenuation = 0.3f;
			light->linearAttenuation = 0.003f;
			light->quadraticAttenuation = 0.002f;
		}

// 		auto wheelpair = CreateEntity(tank);
// 		SetProperty(wheelpair, "Name", "WheelPair");
// 		AddComponent(wheelpair, "WheelPairThingy");

		//Create wheels
		float wheelOffset = 0.4f;
		float springLength = 0.3f;
		float suspensionStrength = 25.f;

		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(1.88f, -0.83f - wheelOffset, -2.6f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(1.88f, -0.83f - wheelOffset, -0.83f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			CommitEntity(wheel);
		}

		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(-1.88f, -0.83f - wheelOffset, -2.6f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(-1.88f, -0.83f - wheelOffset, -0.83f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			CommitEntity(wheel);
		}


		//Back
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(1.88f, -0.83f - wheelOffset, 1.f);
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(1.88f, -0.83f - wheelOffset, 2.95f);
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			CommitEntity(wheel);
		}

		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(-1.88f, -0.83f - wheelOffset, 1.f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(-1.88f, -0.83f - wheelOffset, 2.95f);
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			CommitEntity(wheel);
		}

		CommitEntity(tank);
	}

	/*
		for(int i = 0; i < 10; i++)
		{
			auto entity = CreateEntity();
			auto transform = AddComponent<Components::Transform>(entity, "Transform");
			transform->Position = glm::vec3(30 + i*0.1f, 0 + i*0.1f, 10 + i*0.1f);
			transform->Scale = glm::vec3(0);
			transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
			
			std::stringstream ss;
			ss << "Models/Placeholders/ShatterTest/" << i+1 << ".obj";

			auto model = AddComponent<Components::Model>(entity, "Model");
			model->ModelFile = ss.str();

			auto physics = AddComponent<Components::Physics>(entity, "Physics");
			physics->Mass = 100;
			physics->Static = true;
			auto meshShape = AddComponent<Components::MeshShape>(entity, "MeshShape");
			meshShape->ResourceName = ss.str();

			CommitEntity(entity);
		}*/

	for(int i = 0; i < 1; i++)
	{
		for (int y = 0; y < 15; y++)
		{
			for (int x = -5; x < 5; x++)
			{
				auto brick = CreateEntity();
				auto transform = AddComponent<Components::Transform>(brick, "Transform");
				transform->Position = glm::vec3(x + 0.01f, y * 0.3f + 0.01f, -20);
				transform->Position.x += (y % 2)*0.5f;
				transform->Scale = glm::vec3(1, 0.3f, 0.4f);
				transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
				auto model = AddComponent<Components::Model>(brick, "Model");
				model->ModelFile = "Models/Placeholders/PhysicsTest/Cube2.obj";

				auto physics = AddComponent<Components::Physics>(brick, "Physics");
				physics->Mass = 3;
				


				auto shape = CreateEntity(brick);
				auto transformshape = AddComponent<Components::Transform>(shape, "Transform");
				auto box = AddComponent<Components::BoxShape>(shape, "BoxShape");
				box->Width = 0.5f;
				box->Height = 0.15f;
				box->Depth = 0.3f;
				CommitEntity(shape);
				CommitEntity(brick);
			}
		}
	}

	/*for (int x = 0; x < 5; x++)
		for (int y = 0; y < 5; y++)
		{
			auto cube = CreateEntity();
			auto transform = AddComponent<Components::Transform>(cube, "Transform");
			transform->Position = glm::vec3(3 * x + 0.1f + -20.f, 3 * y + 0.1f + 1.f, 0);
			transform->Scale = glm::vec3(3);
			transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
			auto model = AddComponent<Components::Model>(cube, "Model");
			model->ModelFile = "Models/Placeholders/PhysicsTest/Cube2.obj";

			auto physics = AddComponent<Components::Physics>(cube, "Physics");
			physics->Mass = 100;
			auto box = AddComponent<Components::BoxShape>(cube, "BoxShape");
			box->Width = 1.5f;
			box->Height = 1.5f;
			box->Depth = 1.5f;
			CommitEntity(cube);
		}
*/


	/*{
		auto entity = CreateEntity();
		AddComponent(entity, "Transform");
		auto emitter = AddComponent<Components::SoundEmitter>(entity, "SoundEmitter");
		emitter->Path = "Sounds/korvring.wav";
		emitter->Loop = true;
		GetSystem<Systems::SoundSystem>("SoundSystem")->PlaySound(emitter);
		CommitEntity(entity);
	}*/
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
	////m_SystemFactory.Register("ParticleSystem", [this]() { return new Systems::ParticleSystem(this); });
	//m_SystemFactory.Register("PlayerSystem", [this]() { return new Systems::PlayerSystem(this); });
	m_SystemFactory.Register("FreeSteeringSystem", [this]() { return new Systems::FreeSteeringSystem(this); });
	m_SystemFactory.Register("SoundSystem", [this]() { return new Systems::SoundSystem(this); });
	m_SystemFactory.Register("PhysicsSystem", [this]() { return new Systems::PhysicsSystem(this); });
	m_SystemFactory.Register("RenderSystem", [this]() { return new Systems::RenderSystem(this, m_Renderer); });
	m_SystemFactory.Register("FreeSteeringSystem", [this]() { return new Systems::FreeSteeringSystem(this, m_EventBroker); });
}

void GameWorld::AddSystems()
{
	AddSystem("TransformSystem");
	//AddSystem("LevelGenerationSystem");
	AddSystem("InputSystem");
	AddSystem("DebugSystem");
	//AddSystem("CollisionSystem");
	////AddSystem("ParticleSystem");
	//AddSystem("PlayerSystem");
	AddSystem("FreeSteeringSystem");
	AddSystem("SoundSystem");
	AddSystem("PhysicsSystem");
	AddSystem("RenderSystem");
}

void GameWorld::BindKey(int keyCode, std::string command)
{
	Events::BindKey e;
	e.KeyCode = keyCode;
	e.Command = command;
	m_EventBroker->Publish(e);
}

void GameWorld::BindMouseButton(int button, std::string command)
{
	Events::BindMouseButton e;
	e.Button = button;
	e.Command = command;
	m_EventBroker->Publish(e);
}
