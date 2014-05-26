#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();

	ResourceManager->Preload("Model", "Models/Placeholders/PhysicsTest/Plane.obj");
	ResourceManager->Preload("Model", "Models/Placeholders/PhysicsTest/ArrowCube.obj");

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

	//auto camera = CreateEntity();
	//{
	//	auto transform = AddComponent<Components::Transform>(camera);
	//	transform->Position.z = 20.f;
	//	transform->Position.y = 20.f;
	//	//transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
	//	auto cameraComp = AddComponent<Components::Camera>(camera);
	//	cameraComp->FarClip = 2000.f;
	//	auto freeSteering = AddComponent<Components::FreeSteering>(camera);
	//}
	

	auto player1 = CreateEntity();
	{
		auto player = AddComponent<Components::Player>(player1);
		player->ID = 1;
	}
	
	auto player2 = CreateEntity();
	{
		auto player = AddComponent<Components::Player>(player2);
		player->ID = 2;
	}

	
	{
		auto ground = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground);
		transform->Position = glm::vec3(0, -50, 0);
		//transform->Scale = glm::vec3(400.0f, 10.0f, 400.0f);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ground);
		model->ModelFile = "Models/TestScene3/testScene.obj";
		//model->ModelFile = "Models/Placeholders/Terrain/Terrain2.obj";
		
		auto physics = AddComponent<Components::Physics>(ground);
		physics->Mass = 10;
		physics->Static = true;


		auto groundshape = CreateEntity(ground);
		auto transformshape = AddComponent<Components::Transform>(groundshape);
		auto meshShape = AddComponent<Components::MeshShape>(groundshape);
		//meshShape->ResourceName = "Models/Placeholders/Terrain/Terrain2.obj";
		meshShape->ResourceName = "Models/TestScene3/testScene.obj";

		
		CommitEntity(groundshape);
		CommitEntity(ground);
	}

	

	/*{
		auto jeep = CreateEntity();
		auto transform = AddComponent<Components::Transform>(jeep);
		transform->Position = glm::vec3(0, 5, 0);
		transform->Orientation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(0, 1, 0));
		auto physics = AddComponent<Components::Physics>(jeep);
		physics->Mass = 1800;
		physics->Static = false;
		auto vehicle = AddComponent<Components::Vehicle>(jeep);
		AddComponent<Components::Input>(jeep);

		{
			auto shape = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(shape);
			auto meshShape = AddComponent<Components::MeshShape>(shape);
			meshShape->ResourceName = "Models/Jeep/Chassi/ChassiCollision.obj";
			CommitEntity(shape);

			// 		auto box = AddComponent<Components::Box>(jeep);
			// 		box->Width = 1.487f;
			// 		box->Height = 0.727f;
			// 		box->Depth = 2.594f;

		}

		{
			auto chassis = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(chassis);
			transform->Position = glm::vec3(0, 0, 0); // 0.6577f
			auto model = AddComponent<Components::Model>(chassis);
			model->ModelFile = "Models/Jeep/Chassi/chassi.obj";
		}

		{
			auto lightentity = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(lightentity);
			transform->Position = glm::vec3(0, 15, 0);
			auto light = AddComponent<Components::PointLight>(lightentity);
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
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(1.9f, 0.5546f - wheelOffset, -0.9242f);
			transform->Scale = glm::vec3(1.0f);
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Jeep/WheelFront/wheelFront.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
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
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(-1.9f, 0.5546f - wheelOffset, -0.9242f);
			transform->Scale = glm::vec3(1.0f);
			transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Jeep/WheelFront/wheelFront.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
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
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(0.2726f, 0.2805f - wheelOffset, 1.9307f);
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Jeep/WheelBack/wheelBack.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
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
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(-0.2726f, 0.2805f - wheelOffset, 1.9307f);
			transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Jeep/WheelBack/wheelBack.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
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
		auto transform = AddComponent<Components::Transform>(tank);
		transform->Position = glm::vec3(0, 5, 0);
		//transform->Orientation = glm::angleAxis(0.f, glm::vec3(0, 1, 0));
		auto physics = AddComponent<Components::Physics>(tank);
		physics->Mass = 63000 - 16000;
		physics->Static = false;
		auto vehicle = AddComponent<Components::Vehicle>(tank);
		vehicle->MaxTorque = 36000.f;
		vehicle->MaxSteeringAngle = 90.f;
		vehicle->MaxSpeedFullSteeringAngle = 4.f;
		auto tankSteering = AddComponent<Components::TankSteering>(tank);
		tankSteering->Player = player1;
		AddComponent<Components::Input>(tank);

		{
			auto shape = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(shape);
			auto meshShape = AddComponent<Components::MeshShape>(shape);
			meshShape->ResourceName = "Models/Tank/Fix/ChassiCollision.obj";
			CommitEntity(shape);

			// 		auto box = AddComponent<Components::Box>(jeep);
			// 		box->Width = 1.487f;
			// 		box->Height = 0.727f;
			// 		box->Depth = 2.594f;

		}

		{
			auto chassis = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(chassis);
			transform->Position = glm::vec3(0, 0, 0);
			auto model = AddComponent<Components::Model>(chassis);
			model->ModelFile = "Models/Tank/tankBody.obj";
		}
		{
			auto tower = CreateEntity(tank);
			SetProperty(tower, "Name", "tower");
			auto transform = AddComponent<Components::Transform>(tower);
			transform->Position = glm::vec3(0.f, 0.68f, 0.9f);
			auto model = AddComponent<Components::Model>(tower);
			model->ModelFile = "Models/Tank/tankTop.obj";
			auto towerSteering = AddComponent<Components::TowerSteering>(tower);
			towerSteering->Axis = glm::vec3(0.f, 1.f, 0.f);
			towerSteering->TurnSpeed = glm::pi<float>()/4.f;
			{
				auto barrel = CreateEntity(tower);
				auto transform = AddComponent<Components::Transform>(barrel);
				transform->Position = glm::vec3(-0.012f, 0.4f, -0.75);
				auto model = AddComponent<Components::Model>(barrel);
				model->ModelFile = "Models/Tank/tankBarrel.obj";
				auto barrelSteering = AddComponent<Components::BarrelSteering>(barrel);
				barrelSteering->Axis = glm::vec3(1.f, 0.f, 0.f);
				barrelSteering->TurnSpeed = glm::pi<float>()/4.f;
				barrelSteering->ShotSpeed = 70.f;
				{
					auto shot = CreateEntity(barrel);
					auto transform = AddComponent<Components::Transform>(shot);
					transform->Position = glm::vec3(0.35f, 0.f, -2.f);
					transform->Orientation = glm::angleAxis(-glm::pi<float>()/2.f, glm::vec3(1, 0, 0));
					transform->Scale = glm::vec3(3.f);
					AddComponent<Components::Template>(shot);
					auto physics = AddComponent<Components::Physics>(shot);
					physics->Mass = 25.f;
					physics->Static = false;
					auto modelComponent = AddComponent<Components::Model>(shot);
					modelComponent->ModelFile = "Models/Placeholders/rocket/Rocket.obj";

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
					barrelSteering->ShotTemplate = shot;
				}
				CommitEntity(barrel);
				tankSteering->Barrel = barrel;
			}
			CommitEntity(tower);
			tankSteering->Turret = tower;
			

			auto cameraTower = CreateEntity(tower);
			{
				auto transform = AddComponent<Components::Transform>(cameraTower);
				transform->Position.z = 11.f;
				transform->Position.y = 4.f;
				//transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
				auto cameraComp = AddComponent<Components::Camera>(cameraTower);
				cameraComp->FarClip = 2000.f;
				//auto freeSteering = AddComponent<Components::FreeSteering>(cameraTower);
			}
			CommitEntity(cameraTower);
			{
				Events::SetViewportCamera e;
				e.CameraEntity = cameraTower;
				e.ViewportFrame = "Viewport1";
				EventBroker->Publish(e);
			}
		}

		{
			auto lightentity = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(lightentity);
			transform->Position = glm::vec3(0, 0, 0);
			auto light = AddComponent<Components::PointLight>(lightentity);
			//light->Diffuse = glm::vec3(128.f/255.f, 172.f/255.f, 242.f/255.f);
			//light->Specular = glm::vec3(1.f);
			/*light->ConstantAttenuation = 0.3f;
			light->LinearAttenuation = 0.003f;
			light->QuadraticAttenuation = 0.002f;*/
		}

// 		auto wheelpair = CreateEntity(tank);
// 		SetProperty(wheelpair, "Name", "WheelPair");
// 		AddComponent(wheelpair, "WheelPairThingy");

		//Create wheels
		float wheelOffset = 0.4f;
		float springLength = 0.3f;
		float suspensionStrength = 15.f;

		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(1.68f, -0.83f - wheelOffset, -2.6f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(1.68f, -0.83f - wheelOffset, -0.83f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}

		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(-1.68f, -0.83f - wheelOffset, -2.6f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(-1.68f, -0.83f - wheelOffset, -0.83f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}


		//Back
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(1.68f, -0.83f - wheelOffset, 1.f);
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(1.68f, -0.83f - wheelOffset, 2.95f);
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);

			auto entity = CreateEntity(tank);
			auto transformComponent = AddComponent<Components::Transform>(entity);
			transformComponent->Position = glm::vec3(2,-1.7,2.0);
			transformComponent->Scale = glm::vec3(3,3,3);
			transformComponent->Orientation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(1,0,0));
			auto emitterComponent = AddComponent<Components::ParticleEmitter>(entity);
			emitterComponent->SpawnCount = 2;
			emitterComponent->SpawnFrequency = 0.005;
			emitterComponent->SpreadAngle = glm::pi<float>();
			emitterComponent->UseGoalVelocity = false;
			emitterComponent->LifeTime = 0.5;
			//emitterComponent->AngularVelocitySpectrum.push_back(glm::pi<float>() / 100);
			emitterComponent->ScaleSpectrum.push_back(glm::vec3(0.05));
			CommitEntity(entity);

			auto particleEntity = CreateEntity(entity);
			auto TEMP = AddComponent<Components::Transform>(particleEntity);
			TEMP->Scale = glm::vec3(0);
			auto spriteComponent = AddComponent<Components::Sprite>(particleEntity);
			spriteComponent->SpriteFile = "Models/Textures/Sprites/Dust.png";
			emitterComponent->ParticleTemplate = particleEntity;

			CommitEntity(particleEntity);
		}

		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(-1.68f, -0.83f - wheelOffset, 1.f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(-1.68f, -0.83f - wheelOffset, 2.95f);
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);

			auto entity = CreateEntity(tank);
			auto transformComponent = AddComponent<Components::Transform>(entity);
			transformComponent->Position = glm::vec3(-2,-1.7,2.0);
			transformComponent->Scale = glm::vec3(3,3,3);
			transformComponent->Orientation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(1,0,0));
			auto emitterComponent = AddComponent<Components::ParticleEmitter>(entity);
			emitterComponent->SpawnCount = 2;
			emitterComponent->SpawnFrequency = 0.005;
			emitterComponent->SpreadAngle = glm::pi<float>();
			emitterComponent->UseGoalVelocity = false;
			emitterComponent->LifeTime = 0.5;
			//emitterComponent->AngularVelocitySpectrum.push_back(glm::pi<float>() / 100);
			emitterComponent->ScaleSpectrum.push_back(glm::vec3(0.05));
			CommitEntity(entity);

			auto particleEntity = CreateEntity(entity);
			auto TEMP = AddComponent<Components::Transform>(particleEntity);
			TEMP->Scale = glm::vec3(0);
			auto spriteComponent = AddComponent<Components::Sprite>(particleEntity);
			spriteComponent->SpriteFile = "Models/Textures/Sprites/Dust.png";
			emitterComponent->ParticleTemplate = particleEntity;

			CommitEntity(particleEntity);
		}

		CommitEntity(tank);
	}

	{
		auto tank = CreateEntity();
		auto transform = AddComponent<Components::Transform>(tank);
		transform->Position = glm::vec3(20, 5, 0);
		//transform->Orientation = glm::angleAxis(0.f, glm::vec3(0, 1, 0));
		auto physics = AddComponent<Components::Physics>(tank);
		physics->Mass = 63000 - 16000;
		physics->Static = false;
		auto vehicle = AddComponent<Components::Vehicle>(tank);
		vehicle->MaxTorque = 36000.f;
		vehicle->MaxSteeringAngle = 90.f;
		vehicle->MaxSpeedFullSteeringAngle = 4.f;
		auto tankSteering = AddComponent<Components::TankSteering>(tank);
		tankSteering->Player = player2;
		AddComponent<Components::Input>(tank);

		{
			auto shape = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(shape);
			auto meshShape = AddComponent<Components::MeshShape>(shape);
			meshShape->ResourceName = "Models/Tank/Fix/ChassiCollision.obj";
			CommitEntity(shape);

			// 		auto box = AddComponent<Components::Box>(jeep);
			// 		box->Width = 1.487f;
			// 		box->Height = 0.727f;
			// 		box->Depth = 2.594f;

		}

		{
			auto chassis = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(chassis);
			transform->Position = glm::vec3(0, 0, 0);
			auto model = AddComponent<Components::Model>(chassis);
			model->ModelFile = "Models/Tank/tankBody.obj";
		}
		{
			auto tower = CreateEntity(tank);
			SetProperty(tower, "Name", "tower");
			auto transform = AddComponent<Components::Transform>(tower);
			transform->Position = glm::vec3(0.f, 0.68f, 0.9f);
			auto model = AddComponent<Components::Model>(tower);
			model->ModelFile = "Models/Tank/tankTop.obj";
			auto towerSteering = AddComponent<Components::TowerSteering>(tower);
			towerSteering->Axis = glm::vec3(0.f, 1.f, 0.f);
			towerSteering->TurnSpeed = glm::pi<float>()/4.f;
			{
				auto barrel = CreateEntity(tower);
				auto transform = AddComponent<Components::Transform>(barrel);
				transform->Position = glm::vec3(-0.012f, 0.4f, -0.75);
				auto model = AddComponent<Components::Model>(barrel);
				model->ModelFile = "Models/Tank/tankBarrel.obj";
				auto barrelSteering = AddComponent<Components::BarrelSteering>(barrel);
				barrelSteering->Axis = glm::vec3(1.f, 0.f, 0.f);
				barrelSteering->TurnSpeed = glm::pi<float>()/4.f;
				barrelSteering->ShotSpeed = 70.f;
				{
					auto shot = CreateEntity(barrel);
					auto transform = AddComponent<Components::Transform>(shot);
					transform->Position = glm::vec3(0.35f, 0.f, -2.f);
					transform->Orientation = glm::angleAxis(-glm::pi<float>()/2.f, glm::vec3(1, 0, 0));
					transform->Scale = glm::vec3(3.f);
					AddComponent<Components::Template>(shot);
					auto physics = AddComponent<Components::Physics>(shot);
					physics->Mass = 25.f;
					physics->Static = false;
					auto modelComponent = AddComponent<Components::Model>(shot);
					modelComponent->ModelFile = "Models/Placeholders/rocket/Rocket.obj";

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
					barrelSteering->ShotTemplate = shot;
				}
				CommitEntity(barrel);
				tankSteering->Barrel = barrel;
			}
			CommitEntity(tower);
			tankSteering->Turret = tower;
			

			auto cameraTower = CreateEntity(tower);
			{
				auto transform = AddComponent<Components::Transform>(cameraTower);
				transform->Position.z = 11.f;
				transform->Position.y = 4.f;
				//transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
				auto cameraComp = AddComponent<Components::Camera>(cameraTower);
				cameraComp->FarClip = 2000.f;
				//auto freeSteering = AddComponent<Components::FreeSteering>(cameraTower);
			}
			CommitEntity(cameraTower);
			{
				Events::SetViewportCamera e;
				e.CameraEntity = cameraTower;
				e.ViewportFrame = "Viewport2";
				EventBroker->Publish(e);
			}
		}

		{
			auto lightentity = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(lightentity);
			transform->Position = glm::vec3(0, 0, 0);
			auto light = AddComponent<Components::PointLight>(lightentity);
			//light->Diffuse = glm::vec3(128.f/255.f, 172.f/255.f, 242.f/255.f);
			//light->Specular = glm::vec3(1.f);
			/*light->ConstantAttenuation = 0.3f;
			light->LinearAttenuation = 0.003f;
			light->QuadraticAttenuation = 0.002f;*/
		}

// 		auto wheelpair = CreateEntity(tank);
// 		SetProperty(wheelpair, "Name", "WheelPair");
// 		AddComponent(wheelpair, "WheelPairThingy");

		//Create wheels
		float wheelOffset = 0.4f;
		float springLength = 0.3f;
		float suspensionStrength = 15.f;

		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(1.68f, -0.83f - wheelOffset, -2.6f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(1.68f, -0.83f - wheelOffset, -0.83f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}

		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(-1.68f, -0.83f - wheelOffset, -2.6f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(-1.68f, -0.83f - wheelOffset, -0.83f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}


		//Back
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(1.68f, -0.83f - wheelOffset, 1.f);
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(1.68f, -0.83f - wheelOffset, 2.95f);
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);

			auto entity = CreateEntity(tank);
			auto transformComponent = AddComponent<Components::Transform>(entity);
			transformComponent->Position = glm::vec3(2,-1.7,2.0);
			transformComponent->Scale = glm::vec3(3,3,3);
			transformComponent->Orientation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(1,0,0));
			auto emitterComponent = AddComponent<Components::ParticleEmitter>(entity);
			emitterComponent->SpawnCount = 2;
			emitterComponent->SpawnFrequency = 0.005;
			emitterComponent->SpreadAngle = glm::pi<float>();
			emitterComponent->UseGoalVelocity = false;
			emitterComponent->LifeTime = 0.5;
			//emitterComponent->AngularVelocitySpectrum.push_back(glm::pi<float>() / 100);
			emitterComponent->ScaleSpectrum.push_back(glm::vec3(0.05));
			CommitEntity(entity);

			auto particleEntity = CreateEntity(entity);
			auto TEMP = AddComponent<Components::Transform>(particleEntity);
			TEMP->Scale = glm::vec3(0);
			auto spriteComponent = AddComponent<Components::Sprite>(particleEntity);
			spriteComponent->SpriteFile = "Models/Textures/Sprites/Dust.png";
			emitterComponent->ParticleTemplate = particleEntity;

			CommitEntity(particleEntity);
		}

		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(-1.68f, -0.83f - wheelOffset, 1.f);
			transform->Orientation =  glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);
		}
		{
			auto wheel = CreateEntity(tank);
			auto transform = AddComponent<Components::Transform>(wheel);
			transform->Position = glm::vec3(-1.68f, -0.83f - wheelOffset, 2.95f);
			auto model = AddComponent<Components::Model>(wheel);
			model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel);
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 2000;
			Wheel->Radius = 0.6f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = suspensionStrength;
			Wheel->Friction = 3.f;
			Wheel->ConnectedToHandbrake = true;
			Wheel->TorqueRatio = 0.125f;
			Wheel->Width = 0.6f;
			{
				auto shape = CreateEntity(wheel);
				auto shapetransform = AddComponent<Components::Transform>(shape);
				shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
				auto boxShape = AddComponent<Components::BoxShape>(shape);
				boxShape->Width = 0.7f;
				boxShape->Height = 0.34f;
				boxShape->Depth = 0.7f;
				CommitEntity(shape);
			}
			CommitEntity(wheel);

			auto entity = CreateEntity(tank);
			auto transformComponent = AddComponent<Components::Transform>(entity);
			transformComponent->Position = glm::vec3(-2,-1.7,2.0);
			transformComponent->Scale = glm::vec3(3,3,3);
			transformComponent->Orientation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(1,0,0));
			auto emitterComponent = AddComponent<Components::ParticleEmitter>(entity);
			emitterComponent->SpawnCount = 2;
			emitterComponent->SpawnFrequency = 0.005;
			emitterComponent->SpreadAngle = glm::pi<float>();
			emitterComponent->UseGoalVelocity = false;
			emitterComponent->LifeTime = 0.5;
			//emitterComponent->AngularVelocitySpectrum.push_back(glm::pi<float>() / 100);
			emitterComponent->ScaleSpectrum.push_back(glm::vec3(0.05));
			CommitEntity(entity);

			auto particleEntity = CreateEntity(entity);
			auto TEMP = AddComponent<Components::Transform>(particleEntity);
			TEMP->Scale = glm::vec3(0);
			auto spriteComponent = AddComponent<Components::Sprite>(particleEntity);
			spriteComponent->SpriteFile = "Models/Textures/Sprites/Dust.png";
			emitterComponent->ParticleTemplate = particleEntity;

			CommitEntity(particleEntity);
		}

		CommitEntity(tank);
	}

	/*
		for(int i = 0; i < 10; i++)
		{
			auto entity = CreateEntity();
			auto transform = AddComponent<Components::Transform>(entity);
			transform->Position = glm::vec3(30 + i*0.1f, 0 + i*0.1f, 10 + i*0.1f);
			transform->Scale = glm::vec3(0);
			transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
			
			std::stringstream ss;
			ss << "Models/Placeholders/ShatterTest/" << i+1 << ".obj";

			auto model = AddComponent<Components::Model>(entity);
			model->ModelFile = ss.str();

			auto physics = AddComponent<Components::Physics>(entity);
			physics->Mass = 100;
			physics->Static = true;
			auto meshShape = AddComponent<Components::MeshShape>(entity);
			meshShape->ResourceName = ss.str();

			CommitEntity(entity);
		}*/

	/*for(int i = 0; i < 1; i++)
	{
		for (int y = 0; y < 15; y++)
		{
			for (int x = -5; x < 5; x++)
			{
				auto brick = CreateEntity();
				auto transform = AddComponent<Components::Transform>(brick);
				transform->Position = glm::vec3(x + 0.01f, y * 0.3f + 0.01f, -20);
				transform->Position.x += (y % 2)*0.5f;
				transform->Scale = glm::vec3(1, 0.3f, 0.4f);
				transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
				auto model = AddComponent<Components::Model>(brick);
				model->ModelFile = "Models/Placeholders/PhysicsTest/Cube2.obj";

				auto physics = AddComponent<Components::Physics>(brick);
				physics->Mass = 3;
				


				auto shape = CreateEntity(brick);
				auto transformshape = AddComponent<Components::Transform>(shape);
				auto box = AddComponent<Components::BoxShape>(shape);
				box->Width = 0.5f;
				box->Height = 0.15f;
				box->Depth = 0.3f;
				CommitEntity(shape);
				CommitEntity(brick);
			}
		}
	}*/

	/*for (int x = 0; x < 5; x++)
		for (int y = 0; y < 5; y++)
		{
			auto cube = CreateEntity();
			auto transform = AddComponent<Components::Transform>(cube);
			transform->Position = glm::vec3(3 * x + 0.1f + -20.f, 3 * y + 0.1f + 1.f, 0);
			transform->Scale = glm::vec3(3);
			transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
			auto model = AddComponent<Components::Model>(cube);
			model->ModelFile = "Models/Placeholders/PhysicsTest/Cube2.obj";

			auto physics = AddComponent<Components::Physics>(cube);
			physics->Mass = 100;
			auto box = AddComponent<Components::BoxShape>(cube);
			box->Width = 1.5f;
			box->Height = 1.5f;
			box->Depth = 1.5f;
			CommitEntity(cube);
		}
*/


	/*{
		auto entity = CreateEntity();
		AddComponent(entity, "Transform");
		auto emitter = AddComponent<Components::SoundEmitter>(entity);
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
	m_ComponentFactory.Register<Components::Transform>([]() { return new Components::Transform(); }); 
	m_ComponentFactory.Register<Components::Template>([]() { return new Components::Template(); });	
	m_ComponentFactory.Register<Components::Player>([]() { return new Components::Player(); });	
}

void GameWorld::RegisterSystems()
{
	m_SystemFactory.Register<Systems::TransformSystem>([this]() { return new Systems::TransformSystem(this, EventBroker, ResourceManager); });
	//m_SystemFactory.Register<Systems::LevelGenerationSystem>([this]() { return new Systems::LevelGenerationSystem(this); });
	m_SystemFactory.Register<Systems::InputSystem>([this]() { return new Systems::InputSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::DebugSystem>([this]() { return new Systems::DebugSystem(this, EventBroker, ResourceManager); });
	//m_SystemFactory.Register<Systems::CollisionSystem>([this]() { return new Systems::CollisionSystem(this); });
	m_SystemFactory.Register<Systems::ParticleSystem>([this]() { return new Systems::ParticleSystem(this, EventBroker, ResourceManager); });
	//m_SystemFactory.Register<Systems::PlayerSystem>([this]() { return new Systems::PlayerSystem(this); });
	m_SystemFactory.Register<Systems::FreeSteeringSystem>([this]() { return new Systems::FreeSteeringSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::TankSteeringSystem>([this]() { return new Systems::TankSteeringSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::SoundSystem>([this]() { return new Systems::SoundSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::PhysicsSystem>([this]() { return new Systems::PhysicsSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::RenderSystem>([this]() { return new Systems::RenderSystem(this, EventBroker, ResourceManager); });
}

void GameWorld::AddSystems()
{
	AddSystem<Systems::TransformSystem>();
	//AddSystem<Systems::LevelGenerationSystem>();
	AddSystem<Systems::InputSystem>();
	AddSystem<Systems::DebugSystem>();
	//AddSystem<Systems::CollisionSystem>();
	AddSystem<Systems::ParticleSystem>();
	//AddSystem<Systems::PlayerSystem>();
	AddSystem<Systems::FreeSteeringSystem>();
	AddSystem<Systems::TankSteeringSystem>();
	AddSystem<Systems::SoundSystem>();
	AddSystem<Systems::PhysicsSystem>();
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
