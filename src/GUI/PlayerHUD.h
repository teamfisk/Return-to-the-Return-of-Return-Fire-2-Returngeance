#ifndef GUI_PlayerHUD_h__
#define GUI_PlayerHUD_h__

#include "GUI/Frame.h"
#include "GUI/HealthOverlay.h"
#include "GUI/VehicleSelection.h"
#include "GUI/GameOverFrame.h"

#include "Events/GameOver.h"
#include "Events/Dead.h"
#include "Components/Player.h"

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
			EVENT_SUBSCRIBE_MEMBER(m_EGameOver, &GUI::PlayerHUD::OnGameOver);
			EVENT_SUBSCRIBE_MEMBER(m_EDead, &GUI::PlayerHUD::OnDead);

			m_HealthOverlay = new HealthOverlay(this, "HealthOverlay", m_World, m_PlayerID);

			m_Crosshair = new TextureFrame(this, "Crosshair");
			m_Crosshair->Width = 45;
			m_Crosshair->Height = 45;
			m_Crosshair->X = this->Width / 2.f - m_Crosshair->Width / 2.f;
			m_Crosshair->Y = this->Height / 2.f - m_Crosshair->Height / 2.f;
			m_Crosshair->SetTexture("Textures/GUI/crosshair.png");
			m_Crosshair->SetColor(glm::vec4(1.f, 1.f, 1.f, 0.75f));

			m_VehicleSelection = new VehicleSelection(this, "VehicleSelection", m_World, m_PlayerID);

			m_GameOverFrame = new GameOverFrame(this, "GameOver");
			m_GameOverFrame->Hide();
		}

	protected:
		std::shared_ptr<World> m_World;
		int m_PlayerID;

		HealthOverlay* m_HealthOverlay;
		TextureFrame* m_Crosshair;
		VehicleSelection* m_VehicleSelection;
		GameOverFrame* m_GameOverFrame;

		EventRelay<Frame, Events::GameOver> m_EGameOver;
		bool OnGameOver(const Events::GameOver &event)
		{
			if (m_PlayerID == event.Player)
				m_GameOverFrame->Show(true);
			else
				m_GameOverFrame->Show(false);

			return true;
		}

		EventRelay<Frame, Events::Dead> m_EDead;
		bool OnDead(const Events::Dead &event)
		{
			m_VehicleSelection->Show();
			m_HealthOverlay->SetColor(glm::vec4(1, 1, 1, 0));
			return true;
		}

		bool OnCommand(const Events::InputCommand &event)
		{
			if (event.PlayerID != m_PlayerID)
				return false;

			if (event.Command == "interface_confirm" && event.Value > 0)
			{
				/*if (m_GameOverFrame->Visible())
				{
					m_GameOverFrame->Hide();
				}*/
			}

			return true;
		}
	};

}

#endif // GUI_TextureFrame_h__
