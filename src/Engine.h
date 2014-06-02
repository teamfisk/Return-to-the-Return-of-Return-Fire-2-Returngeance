#include <string>
#include <sstream>

#include "ResourceManager.h"
#include "OBJ.h"
#include "Model.h"
#include "Texture.h"
#include "EventBroker.h"
#include "RenderQueue.h"
#include "Renderer.h"
#include "InputManager.h"
#include "GUI/Frame.h"
#include "GUI/GameFrame.h"

class Engine
{
public:
	Engine(int argc, char* argv[])
	{
		m_EventBroker = std::make_shared<EventBroker>();

		m_ResourceManager = std::make_shared<ResourceManager>();
		m_ResourceManager->RegisterType("OBJ", [](std::string resourceName) { return new OBJ(resourceName); });
		auto rm = m_ResourceManager;
		m_ResourceManager->RegisterType("Model", [rm](std::string resourceName) { return new Model(rm, *rm->Load<OBJ>("OBJ", resourceName)); });
		m_ResourceManager->RegisterType("Texture", [](std::string resourceName) { return new Texture(resourceName); });

		m_FrameStack = new GUI::Frame(m_EventBroker, m_ResourceManager); 
		m_FrameStack->Width = 1920;
		m_FrameStack->Height = 1080;
		
		m_Renderer = std::make_shared<Renderer>(m_ResourceManager);
		m_Renderer->SetFullscreen(false);
		m_Renderer->SetResolution(*static_cast<Rectangle*>(m_FrameStack));
		m_Renderer->Initialize();

		m_InputManager = std::make_shared<InputManager>(m_Renderer->GetWindow(), m_EventBroker);

		new GUI::GameFrame(m_FrameStack, "GameFrame");
		//m_World = std::make_shared<GameWorld>(m_EventBroker, m_ResourceManager);
		//m_World->Initialize();

		m_LastTime = glfwGetTime();
	}

	bool Running() const { return !glfwWindowShouldClose(m_Renderer->GetWindow()); }

	void Tick()
	{
		double currentTime = glfwGetTime();
		double dt = currentTime - m_LastTime;
		m_LastTime = currentTime;

		// Update input
		m_InputManager->Update(dt);

		// Update frame stack
		m_EventBroker->Process<GUI::Frame>();
		m_FrameStack->UpdateLayered(dt);

		// Render scene
		m_FrameStack->DrawLayered(m_Renderer);
		m_Renderer->Swap();
		
		// Swap event queues
		m_EventBroker->Clear();

		glfwPollEvents();
	}

private:
	std::shared_ptr<ResourceManager> m_ResourceManager;
	std::shared_ptr<EventBroker> m_EventBroker;
	std::shared_ptr<Renderer> m_Renderer;
	std::shared_ptr<InputManager> m_InputManager;
	GUI::Frame* m_FrameStack;
	// TODO: This should ultimately live in GameFrame
	//std::shared_ptr<GameWorld> m_World;

	double m_LastTime;
};