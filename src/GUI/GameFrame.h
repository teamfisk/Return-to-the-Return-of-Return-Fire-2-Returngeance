#ifndef GUI_GameFrame_h__
#define GUI_GameFrame_h__

#include "GUI/Frame.h"
#include "GUI/WorldFrame.h"
#include "GUI/Viewport.h"

#include "GameWorld.h"

namespace GUI
{

class GameFrame : public Frame
{
public:
	GameFrame(Frame* parent, std::string name)
		: Frame(parent, name)
	{
		m_World = std::make_shared<GameWorld>(EventBroker, ResourceManager);
		m_World->Initialize();

		auto worldFrame = new WorldFrame(this, "GameWorldFrame", m_World);
		{
			new Viewport(worldFrame, "Viewport1", m_World);
			new Viewport(worldFrame, "Viewport2", m_World);
		}
	}

	void Update(double dt) override
	{
		
	}

	void Draw(std::shared_ptr<Renderer> renderer) override
	{
		
	}

private:
	std::shared_ptr<GameWorld> m_World;
};

}

#endif // GUI_GameFrame_h__
