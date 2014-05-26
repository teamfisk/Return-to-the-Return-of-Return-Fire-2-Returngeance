#include <string>
#include <sstream>

#include "ResourceManager.h"
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

		m_Renderer = std::make_shared<Renderer>();
		m_Renderer->Initialize();

		m_InputManager = std::make_shared<InputManager>(m_Renderer->GetWindow(), m_EventBroker);

		m_FrameStack = new GUI::Frame(m_EventBroker, m_ResourceManager); 
		new GUI::GameFrame(m_FrameStack, "GameFrame");

		//m_World = std::make_shared<GameWorld>(m_EventBroker, m_ResourceManager);
		//m_World->Initialize();

		m_LastTime = glfwGetTime();
	}

	bool Running() const { return !glfwWindowShouldClose(m_Renderer->GetWindow()); }

	void Tick()
	{
		double currentTime = glfwGetTime();
		double dt =  currentTime - m_LastTime;
		m_LastTime = currentTime;

		m_InputManager->Update(dt);
		m_FrameStack->DrawLayered(m_Renderer);
		
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