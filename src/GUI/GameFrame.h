#ifndef GUI_GameFrame_h__
#define GUI_GameFrame_h__

#include "GUI/Frame.h"
#include "GUI/WorldFrame.h"
#include "GUI/Viewport.h"
#include "GUI/TextureFrame.h"
#include "GUI/PlayerHUD.h"

#include "GameWorld.h"

namespace GUI
{

class GameFrame : public Frame
{
public:
	GameFrame(Frame* parent, std::string name)
		: Frame(parent, name)
	{
		EVENT_SUBSCRIBE_MEMBER(m_EKeyUp, &GameFrame::OnKeyUp);

		m_World = std::make_shared<GameWorld>(EventBroker, ResourceManager);
		auto worldFrame = new WorldFrame(this, "GameWorldFrame", m_World);
		{
			vp1 = new Viewport(worldFrame, "Viewport1", m_World);
			vp1->X = 0;
			vp1->Width = 640;
			new PlayerHUD(vp1, "PlayerHUD", m_World, 1);

			vp2 = new Viewport(worldFrame, "Viewport2", m_World);
			vp2->X = vp1->Right();
			vp2->Width = 640;
			new PlayerHUD(vp2, "PlayerHUD", m_World, 2);

			m_FreeCamViewport = new Viewport(worldFrame, "ViewportFreeCam", m_World);
			m_FreeCamViewport->Show();
		}
		m_World->Initialize();
	}

private:
	EventRelay<Frame, Events::KeyUp> m_EKeyUp;

	std::shared_ptr<GameWorld> m_World;
	Viewport* vp1;
	Viewport* vp2;
	Viewport* m_FreeCamViewport;

	bool OnKeyUp(const Events::KeyUp &event)
	{ 
		if (event.KeyCode == GLFW_KEY_1)
		{
			if (m_FreeCamViewport->Hidden())
				m_FreeCamViewport->Show();
			else
				m_FreeCamViewport->Hide();
		}

		return true; 
	}
};

}

#endif // GUI_GameFrame_h__
