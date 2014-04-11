#include <string>
#include <sstream>

#include "Renderer.h"
#include "GameWorld.h"

class Engine
{
public:
	Engine(int argc, char* argv[])
	{	m_Renderer = std::make_shared<Renderer>();
		m_Renderer->Initialize();

		m_World = std::make_shared<GameWorld>(m_Renderer);
		m_World->Initialize();

		m_LastTime = glfwGetTime();
	}

	bool Running() const { return !glfwWindowShouldClose(m_Renderer->GetWindow()); }

	void Tick()
	{	double currentTime = glfwGetTime();
		double dt =  currentTime - m_LastTime;
		m_LastTime = currentTime;

		m_World->Update(dt);
		m_Renderer->Draw(dt);

		glfwPollEvents();
	}

private:
	std::shared_ptr<Renderer> m_Renderer;
	// TODO: This should ultimately live in GameFrame
	std::shared_ptr<GameWorld> m_World;

	double m_LastTime;
};