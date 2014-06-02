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

	BindMouseButton(GLFW_MOUSE_BUTTON_1, "cam_lock", 1.f);
	BindKey(GLFW_KEY_Y, "cam_vertical", 1.f);
	BindKey(GLFW_KEY_H, "cam_vertical", -1.f);
	BindKey(GLFW_KEY_G, "cam_horizontal", -1.f);
	BindKey(GLFW_KEY_J, "cam_horizontal", 1.f);
	BindKey(GLFW_KEY_U, "cam_normal", 1.f);
	BindKey(GLFW_KEY_T, "cam_normal", -1.f);
	
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

 	{
 		auto road_base = CreateEntity();
 		auto transform = AddComponent<Components::Transform>(road_base);
 		transform->Position = glm::vec3(0, -50, 0);
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
		auto road_middle = CreateEntity();
		auto transform = AddComponent<Components::Transform>(road_middle);
		transform->Position = glm::vec3(0, -50, 0);
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
		auto road_small = CreateEntity();
		auto transform = AddComponent<Components::Transform>(road_small);
		transform->Position = glm::vec3(0, -50, 0);
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
		auto water = CreateEntity();
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
		CommitEntity(water);
	}

	{
		auto ground_middle = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground_middle);
		transform->Position = glm::vec3(0, -50, 0);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ground_middle);
		model->ModelFile = "Models/TerrainFiveIstles/Middle.obj";
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
		auto ground_small = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground_small);
		transform->Position = glm::vec3(0, -50, 0);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ground_small);
		model->ModelFile = "Models/TerrainFiveIstles/Small.obj";
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
		auto ground_small_mirrored = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground_small_mirrored);
		transform->Position = glm::vec3(0, -50, 0);
		transform->Orientation = glm::angleAxis(glm::radians(180.f), glm::vec3(0, 1, 0));
		auto model = AddComponent<Components::Model>(ground_small_mirrored);
		model->ModelFile = "Models/TerrainFiveIstles/Small.obj";
		auto blendmap = AddComponent<Components::BlendMap>(ground_small_mirrored);
		blendmap->TextureRed = "Textures/Ground/SoilBeach0087_11_S.jpg";
		blendmap->TextureRedNormal = "Textures/Ground/SoilBeach0087_11_SNM.png";
		blendmap->TextureGreen = "Textures/Ground/Grass0126_2_S.jpg";
		blendmap->TextureGreenNormal = "Textures/Ground/Grass0126_2_SNM.png";
		blendmap->TextureBlue = "Textures/Ground/Cliffs2.png";
		blendmap->TextureBlueNormal = "Textures/Ground/Cliffs2NM.png";
		blendmap->TextureRepeats = 30.f;

		auto physics = AddComponent<Components::Physics>(ground_small_mirrored);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		auto groundshape = CreateEntity(ground_small_mirrored);
		auto transformshape = AddComponent<Components::Transform>(groundshape);
		auto meshShape = AddComponent<Components::MeshShape>(groundshape);
		meshShape->ResourceName = "Models/TerrainFiveIstles/Small.obj";


		CommitEntity(groundshape);
		CommitEntity(ground_small_mirrored);
	}

	{
		auto ground_base = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground_base);
		transform->Position = glm::vec3(0, -50, 0);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ground_base);
		model->ModelFile = "Models/TerrainFiveIstles/Base.obj";
		auto blendmap = AddComponent<Components::BlendMap>(ground_base);
		blendmap->TextureRed = "Textures/Ground/SoilBeach0087_11_S.jpg";
		blendmap->TextureRedNormal = "Textures/Ground/SoilBeach0087_11_SNM.png";
		blendmap->TextureGreen = "Textures/Ground/Grass0126_2_S.jpg";
		blendmap->TextureGreenNormal = "Textures/Ground/Grass0126_2_SNM.png";
		blendmap->TextureBlue = "Textures/Ground/Cliffs2.png";
		blendmap->TextureBlueNormal = "Textures/Ground/Cliffs2NM.png";
		blendmap->TextureRepeats = 30.f;

		auto physics = AddComponent<Components::Physics>(ground_base);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		auto groundshape = CreateEntity(ground_base);
		auto transformshape = AddComponent<Components::Transform>(groundshape);
		auto meshShape = AddComponent<Components::MeshShape>(groundshape);
		meshShape->ResourceName = "Models/TerrainFiveIstles/Base.obj";


		CommitEntity(groundshape);
		CommitEntity(ground_base);
	}

	{
		auto ground_base_mirrored = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground_base_mirrored);
		transform->Position = glm::vec3(0, -50, 0);
		transform->Orientation = glm::angleAxis(glm::radians(180.f), glm::vec3(0, 1, 0));
		auto model = AddComponent<Components::Model>(ground_base_mirrored);
		model->ModelFile = "Models/TerrainFiveIstles/Base.obj";
		auto blendmap = AddComponent<Components::BlendMap>(ground_base_mirrored);
		blendmap->TextureRed = "Textures/Ground/SoilBeach0087_11_S.jpg";
		blendmap->TextureRedNormal = "Textures/Ground/SoilBeach0087_11_SNM.png";
		blendmap->TextureGreen = "Textures/Ground/Grass0126_2_S.jpg";
		blendmap->TextureGreenNormal = "Textures/Ground/Grass0126_2_SNM.png";
		blendmap->TextureBlue = "Textures/Ground/Cliffs2.png";
		blendmap->TextureBlueNormal = "Textures/Ground/Cliffs2NM.png";
		blendmap->TextureRepeats = 30.f;

		auto physics = AddComponent<Components::Physics>(ground_base_mirrored);
		physics->Mass = 10;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;
		physics->CollisionLayer = 1;

		auto groundshape = CreateEntity(ground_base_mirrored);
		auto transformshape = AddComponent<Components::Transform>(groundshape);
		auto meshShape = AddComponent<Components::MeshShape>(groundshape);
		meshShape->ResourceName = "Models/TerrainFiveIstles/Base.obj";


		CommitEntity(groundshape);
		CommitEntity(ground_base_mirrored);
	}

	{
		auto tree = CreateEntity();
		auto transform = AddComponent<Components::Transform>(tree);
		transform->Position = glm::vec3(0, 10, 0);
		
		auto physics = AddComponent<Components::Physics>(tree);
		physics->Mass = 100.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
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
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDestroyed.obj";

		m_WallDebrisTemplates.push_back(debri);
	}
#pragma endregion Region for wall debris

#pragma region Walls

#pragma region Wall1
	{
		auto wall = CreateEntity();
		auto transform = AddComponent<Components::Transform>(wall);
		transform->Position = glm::vec3(0, -16, 0);

		auto physics = AddComponent<Components::Physics>(wall);
		physics->Mass = 100.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;

		auto walldebris = AddComponent<Components::Wall>(wall);
		walldebris->Walldebris = m_WallDebrisTemplates;
		
		auto model = AddComponent<Components::Model>(wall);
		model->ModelFile = "Models/Wall/WallWhole/Wall.obj";

		auto health = AddComponent<Components::Health>(wall);
		health->Amount = 50.f;

		auto shape = CreateEntity(wall);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallCollision/Collision.obj";

	CommitEntity(shape);
	CommitEntity(wall);
	}
#pragma endregion Region for wall1

#pragma endregion Region for all walls

	{
		auto wall = CreateEntity();
		auto transform = AddComponent<Components::Transform>(wall);
		transform->Position = glm::vec3(20, 10, 30);

		auto physics = AddComponent<Components::Physics>(wall);
		physics->Mass = 100.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;

		auto model = AddComponent<Components::Model>(wall);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDestroyed.obj";

		auto shape = CreateEntity(wall);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDestroyed.obj";

		CommitEntity(shape);
		CommitEntity(wall);
	}

	{
		auto wall_trash = CreateEntity();
		auto transform = AddComponent<Components::Transform>(wall_trash);
		transform->Position = glm::vec3(20, 12, 30);

		auto physics = AddComponent<Components::Physics>(wall_trash);
		physics->Mass = 100.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;

		auto model = AddComponent<Components::Model>(wall_trash);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris1.obj";

		auto shape = CreateEntity(wall_trash);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris1.obj";

		CommitEntity(shape);
		CommitEntity(wall_trash);
	}

	{
		auto wall_trash = CreateEntity();
		auto transform = AddComponent<Components::Transform>(wall_trash);
		transform->Position = glm::vec3(20, 10, 45);

		auto physics = AddComponent<Components::Physics>(wall_trash);
		physics->Mass = 100.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Keyframed;

		auto model = AddComponent<Components::Model>(wall_trash);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris2.obj";

		auto shape = CreateEntity(wall_trash);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris2.obj";

		CommitEntity(shape);
		CommitEntity(wall_trash);
	}

	{
		auto wall_trash = CreateEntity();
		auto transform = AddComponent<Components::Transform>(wall_trash);
		transform->Position = glm::vec3(23, 10, 45);

		auto physics = AddComponent<Components::Physics>(wall_trash);
		physics->Mass = 100.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Keyframed;

		auto model = AddComponent<Components::Model>(wall_trash);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris3.obj";

		auto shape = CreateEntity(wall_trash);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris3.obj";

		CommitEntity(shape);
		CommitEntity(wall_trash);
	}

	{
		auto wall_trash = CreateEntity();
		auto transform = AddComponent<Components::Transform>(wall_trash);
		transform->Position = glm::vec3(20, 12, 30);

		auto physics = AddComponent<Components::Physics>(wall_trash);
		physics->Mass = 100.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;

		auto model = AddComponent<Components::Model>(wall_trash);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris4.obj";

		auto shape = CreateEntity(wall_trash);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris4.obj";

		CommitEntity(shape);
		CommitEntity(wall_trash);
	}

	{
		auto wall_trash = CreateEntity();
		auto transform = AddComponent<Components::Transform>(wall_trash);
		transform->Position = glm::vec3(29, 10, 45);

		auto physics = AddComponent<Components::Physics>(wall_trash);
		physics->Mass = 100.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;

		auto model = AddComponent<Components::Model>(wall_trash);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris5.obj";

		auto shape = CreateEntity(wall_trash);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris5.obj";

		CommitEntity(shape);
		CommitEntity(wall_trash);
	}

	{
		auto wall_trash = CreateEntity();
		auto transform = AddComponent<Components::Transform>(wall_trash);
		transform->Position = glm::vec3(32, 10, 45);

		auto physics = AddComponent<Components::Physics>(wall_trash);
		physics->Mass = 100.f;
		physics->MotionType = Components::Physics::MotionTypeEnum::Fixed;

		auto model = AddComponent<Components::Model>(wall_trash);
		model->ModelFile = "Models/Wall/WallDestroyed/WallDebris6.obj";

		auto shape = CreateEntity(wall_trash);
		auto shapetransform = AddComponent<Components::Transform>(shape);
		auto shapemesh = AddComponent<Components::MeshShape>(shape);
		shapemesh->ResourceName = "Models/Wall/WallDestroyed/WallDebris6.obj";

		CommitEntity(shape);
		CommitEntity(wall_trash);
	}

	EntityID tank1 = CreateTank(1);
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
	}

	{
		auto flag = CreateEntity();
		auto transform = AddComponent<Components::Transform>(flag);
		transform->Position = glm::vec3(0, -50, 100);
		auto model = AddComponent<Components::Model>(flag);
		model->ModelFile = "Models/Flag/FishingRod/FishingRod.obj";
		{
			auto fish = CreateEntity(flag);
			auto transform = AddComponent<Components::Transform>(fish);
			transform->Position = glm::vec3(-1.3f, 1.0, 0);
			auto model = AddComponent<Components::Model>(fish);
			model->ModelFile = "Models/Flag/LeFish/Salmon.obj";
			CommitEntity(fish);
		}

		auto trigger = AddComponent<Components::Trigger>(flag);
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

		auto flagComponent = AddComponent<Components::Flag>(flag);

		CommitEntity(flag);
	}

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
	m_ComponentFactory.Register<Components::Flag>([]() { return new Components::Flag(); });
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

EntityID GameWorld::CreateTank(int playerID)
{
	auto tank = CreateEntity();
	auto transform = AddComponent<Components::Transform>(tank);
	transform->Position = glm::vec3(0, 5, 0);
	//transform->Orientation = glm::angleAxis(0.f, glm::vec3(0, 1, 0));
	auto physics = AddComponent<Components::Physics>(tank);
	physics->Mass = 63000 - 16000;
	physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
	auto vehicle = AddComponent<Components::Vehicle>(tank);
	vehicle->MaxTorque = 36000.f;
	vehicle->MaxSteeringAngle = 90.f;
	vehicle->MaxSpeedFullSteeringAngle = 4.f;
	auto player = AddComponent<Components::Player>(tank);
	player->ID = playerID;
	auto tankSteering = AddComponent<Components::TankSteering>(tank);
	AddComponent<Components::Input>(tank);
	auto health = AddComponent<Components::Health>(tank);
	health->Amount = 100.f;

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
		towerSteering->TurnSpeed = glm::pi<float>() / 4.f;
		{
			auto barrel = CreateEntity(tower);
			auto transform = AddComponent<Components::Transform>(barrel);
			transform->Position = glm::vec3(-0.012f, 0.4f, -0.75);
			auto model = AddComponent<Components::Model>(barrel);
			model->ModelFile = "Models/Tank/tankBarrel.obj";
			auto barrelSteering = AddComponent<Components::BarrelSteering>(barrel);
			barrelSteering->Axis = glm::vec3(1.f, 0.f, 0.f);
			barrelSteering->TurnSpeed = glm::pi<float>() / 4.f;
			barrelSteering->ShotSpeed = 70.f;
			{
				auto shot = CreateEntity(barrel);
				auto transform = AddComponent<Components::Transform>(shot);
				transform->Position = glm::vec3(0.35f, 0.f, -2.f);
				transform->Orientation = glm::angleAxis(-glm::pi<float>() / 2.f, glm::vec3(1, 0, 0));
				transform->Scale = glm::vec3(3.f);
				AddComponent<Components::Template>(shot);
				auto physics = AddComponent<Components::Physics>(shot);
				physics->Mass = 25.f;
				physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
				physics->CollisionEvent = true;
				auto modelComponent = AddComponent<Components::Model>(shot);
				modelComponent->ModelFile = "Models/Placeholders/rocket/Rocket.obj";
				auto tankShellComponent = AddComponent<Components::TankShell>(shot);
				tankShellComponent->Damage = 20.f;
				tankShellComponent->ExplosionRadius = 30.f;
				tankShellComponent->ExplosionStrength = 300000.f;
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


				auto cameraTower = CreateEntity(barrel);
				{
					auto transform = AddComponent<Components::Transform>(cameraTower);
					transform->Position.z = 16.f;
					transform->Position.y = 4.f;
					transform->Orientation = glm::quat(glm::vec3(-glm::radians(5.f), 0.f, 0.f));
					auto cameraComp = AddComponent<Components::Camera>(cameraTower);
					cameraComp->FarClip = 2000.f;
					//auto freeSteering = AddComponent<Components::FreeSteering>(cameraTower);
				}

				SetProperty(tank, "Camera", cameraTower);
			}
			CommitEntity(barrel);
			tankSteering->Barrel = barrel;
		}
		CommitEntity(tower);
		tankSteering->Turret = tower;


		
	}

	//{
	//	auto lightentity = CreateEntity(tank);
	//	auto transform = AddComponent<Components::Transform>(lightentity);
	//	transform->Position = glm::vec3(0, 0, 0);
	//	auto light = AddComponent<Components::PointLight>(lightentity);
	//	//light->Diffuse = glm::vec3(128.f/255.f, 172.f/255.f, 242.f/255.f);
	//	//light->Specular = glm::vec3(1.f);
	//	/*light->ConstantAttenuation = 0.3f;
	//	light->LinearAttenuation = 0.003f;
	//	light->QuadraticAttenuation = 0.002f;*/
	//}

	// 		auto wheelpair = CreateEntity(tank);
	// 		SetProperty(wheelpair, "Name", "WheelPair");
	// 		AddComponent(wheelpair, "WheelPairThingy");

#pragma region Wheels
	//Create wheels
	float wheelOffset = -0.83f;
	const float suspensionStrength = 15.f;
	const float springLength = 0.3f;
	
	AddTankWheelPair(tank, glm::vec3(1.68f, wheelOffset, -1.715f), 0, true);
	AddTankWheelPair(tank, glm::vec3(-1.68f, wheelOffset, -1.715f), 0, true);
	AddTankWheelPair(tank, glm::vec3(1.68f, wheelOffset, 2.375), 1, false);
	AddTankWheelPair(tank, glm::vec3(-1.68f, wheelOffset, 2.375), 1, false);
#pragma endregion

	{
		auto entity = CreateEntity(tank);
		auto transformComponent = AddComponent<Components::Transform>(entity);
		transformComponent->Position = glm::vec3(-2, -1.7, 2.0);
		transformComponent->Scale = glm::vec3(3, 3, 3);
		transformComponent->Orientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1, 0, 0));
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

	return tank;
}

void GameWorld::AddTankWheelPair(EntityID tankEntity, glm::vec3 position, int axleID, bool front)
{
	const float separation = 1.77f;
	const float suspensionStrength = 15.f;
	const float springLength = 0.3f;

	bool steering = front;

	auto wheelFront = CreateEntity(tankEntity);
	{
		auto transform = AddComponent<Components::Transform>(wheelFront);
		transform->Position = position - glm::vec3(0, 0, separation / 2.f);  // glm::vec3(1.68f, -0.83f - wheelOffset, -0.83f);
		//transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
#ifdef DEBUG
		auto model = AddComponent<Components::Model>(wheelFront);
		model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
#endif
		auto Wheel = AddComponent<Components::Wheel>(wheelFront);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = axleID;
		Wheel->Mass = 2000;
		Wheel->Radius = 0.6f;
		Wheel->Steering = steering;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 3.f;
		Wheel->ConnectedToHandbrake = true;
		Wheel->TorqueRatio = 0.125f;
		Wheel->Width = 0.6f;
		{
			auto shape = CreateEntity(wheelFront);
			auto shapetransform = AddComponent<Components::Transform>(shape);
			shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
			auto boxShape = AddComponent<Components::BoxShape>(shape);
			boxShape->Width = 0.7f;
			boxShape->Height = 0.34f;
			boxShape->Depth = 0.7f;
			CommitEntity(shape);
		}
		CommitEntity(wheelFront);
	}

	auto wheelBack = CreateEntity(tankEntity);
	{
		auto transform = AddComponent<Components::Transform>(wheelBack);
		transform->Position = position + glm::vec3(0, 0, separation / 2.f); // glm::vec3(1.68f, -0.83f - wheelOffset, -2.6f);
		//transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
#ifdef DEBUG
		auto model = AddComponent<Components::Model>(wheelBack);
		model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
#endif
		auto Wheel = AddComponent<Components::Wheel>(wheelBack);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = axleID;
		Wheel->Mass = 2000;
		Wheel->Radius = 0.6f;
		Wheel->Steering = steering;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 3.f;
		Wheel->ConnectedToHandbrake = true;
		Wheel->TorqueRatio = 0.125f;
		Wheel->Width = 0.6f;
		{
			auto shape = CreateEntity(wheelBack);
			auto shapetransform = AddComponent<Components::Transform>(shape);
			shapetransform->Position = glm::vec3(0.f, 0.32f, 0.f) + transform->Position;
			auto boxShape = AddComponent<Components::BoxShape>(shape);
			boxShape->Width = 0.7f;
			boxShape->Height = 0.34f;
			boxShape->Depth = 0.7f;
			CommitEntity(shape);
		}
		CommitEntity(wheelBack);
	}

	auto wheelPair = CreateEntity(tankEntity);
	{
		{
			auto transform = AddComponent<Components::Transform>(wheelPair);
			transform->Position = position;
			//transform->Orientation = glm::quat(glm::vec3(0, glm::pi<float>() / 4.f, 0));

			auto pair = AddComponent<Components::WheelPair>(wheelPair);
			pair->FakeWheelFront = wheelFront;
			pair->FakeWheelBack = wheelBack;
		}

		auto modelEntity = CreateEntity(wheelPair);
		{
			auto transform = AddComponent<Components::Transform>(modelEntity);
			//transform->Position = glm::vec3(0.f, 0.35f, 0.f);
			if (front)
			{
				transform->Position = glm::vec3(0.f, 0.03f, 0.f);
				transform->Orientation = glm::quat(glm::vec3(0, glm::pi<float>(), 0));
			}
			else
			{
				transform->Position = glm::vec3(0.f, 0.17f, 0.f);
				transform->Scale = glm::vec3(1.f, 1.2f, 1.2f);
			}

			auto model = AddComponent<Components::Model>(modelEntity);
			model->ModelFile = "Models/Tank/tankWheel.obj";
		}
		CommitEntity(modelEntity);
	}
	CommitEntity(wheelPair);
}

EntityID GameWorld::CreateJeep(int playerID)
{
	auto jeep = CreateEntity();
	auto transform = AddComponent<Components::Transform>(jeep);
	transform->Position = glm::vec3(0, 5, 0);
	transform->Orientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(0, 1, 0));
	auto physics = AddComponent<Components::Physics>(jeep);
	physics->Mass = 1800;
	physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
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
		light->Diffuse = glm::vec3(128.f / 255.f, 172.f / 255.f, 242.f / 255.f);
		light->Specular = glm::vec3(1.f);
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

	return jeep;	
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
	m_SystemFactory.Register<Systems::WheelPairSystem>([this]() { return new Systems::WheelPairSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::SoundSystem>([this]() { return new Systems::SoundSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::PhysicsSystem>([this]() { return new Systems::PhysicsSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::TriggerSystem>([this]() { return new Systems::TriggerSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::RenderSystem>([this]() { return new Systems::RenderSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::WallSystem>([this]() { return new Systems::WallSystem(this, EventBroker, ResourceManager); });
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
	AddSystem<Systems::WheelPairSystem>();
	AddSystem<Systems::SoundSystem>();
	AddSystem<Systems::PhysicsSystem>();
	AddSystem<Systems::TriggerSystem>();
	AddSystem<Systems::RenderSystem>();
	AddSystem<Systems::WallSystem>();
}
