#include "GUI/Frame.h"
#include "GUI/HealthOverlay.h"

#ifndef GUI_PlayerHUD_h__
#define GUI_PlayerHUD_h__

namespace GUI
{

	class PlayerHUD : public Frame
	{
	public:
		PlayerHUD(Frame* parent, std::string name, std::shared_ptr<World> world, int playerID)
			: Frame(parent, name)
			, m_World(world)
			, m_PlayerID(playerID)
		{
			m_HealthOverlay = new HealthOverlay(this, "HealthOverlay", m_World, m_PlayerID);
			m_Crosshair = new TextureFrame(this, "Crosshair");
			m_Crosshair->Width = 45;
			m_Crosshair->Height = 45;
			m_Crosshair->X = this->Width / 2.f - m_Crosshair->Width / 2.f;
			m_Crosshair->Y = this->Height / 2.f - m_Crosshair->Height / 2.f;
			m_Crosshair->SetTexture("Textures/GUI/crosshair.png");
			m_Crosshair->SetColor(glm::vec4(1.f, 1.f, 1.f, 0.75f));
		}

	protected:
		std::shared_ptr<World> m_World;
		int m_PlayerID;

		HealthOverlay* m_HealthOverlay;
		TextureFrame* m_Crosshair;
	};

}

#endif // GUI_TextureFrame_h__
