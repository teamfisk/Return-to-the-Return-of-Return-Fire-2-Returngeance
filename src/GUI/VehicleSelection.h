#ifndef GUI_VehicleSelection_h__
#define GUI_VehicleSelection_h__

#include "GUI/Frame.h"
#include "GUI/HealthOverlay.h"
#include "Events/SpawnVehicle.h"

namespace GUI
{

	class VehicleSelection : public Frame
	{
	public:
		VehicleSelection(Frame* parent, std::string name, std::shared_ptr<World> world, int playerID)
			: Frame(parent, name)
			, m_World(world)
			, m_PlayerID(playerID)
		{
			ResourceManager->Preload("Texture", "Textures/GUI/VehicleSelection/1.png");
			ResourceManager->Preload("Texture", "Textures/GUI/VehicleSelection/2.png");
			ResourceManager->Preload("Texture", "Textures/GUI/VehicleSelection/3.png");
			ResourceManager->Preload("Texture", "Textures/GUI/VehicleSelection/4.png");

			m_CurrentSelection = 0;

			glm::vec2 scale = Scale();
			m_CurrentCoordinate = -m_SelectionCoordinates[0] * scale + glm::vec2(Width / 2.f, Height / 2.f);
			m_TargetCoordinate = m_CurrentCoordinate;
			m_CurrentSize = m_SelectionSizes[0] * scale;
			m_TargetSize = m_CurrentSize;
			m_CurrentAlpha = 0.f;

			m_Background = new TextureFrame(this, "VehicleSelectImage1");
			m_Background->X = m_CurrentCoordinate.x;
			m_Background->Y = m_CurrentCoordinate.y;
			m_Background->Width = m_CurrentSize.x;
			m_Background->Height = m_CurrentSize.y;
			m_Background->SetTexture("Textures/GUI/VehicleSelection/1.png");
		}

		void Update(double dt) override
		{
			if (Hidden())
				return;

			glm::vec2 posDiff = m_TargetCoordinate - m_CurrentCoordinate;
			m_CurrentCoordinate += posDiff * 2.f * (float)dt;

			glm::vec2 sizeDiff = m_TargetSize - m_CurrentSize;
			m_CurrentSize += sizeDiff * 3.f * (float)dt;

			m_CurrentAlpha += (1 - m_CurrentAlpha) * 3.f * (float)dt;

			m_Background->X = m_CurrentCoordinate.x;
			m_Background->Y = m_CurrentCoordinate.y;
			m_Background->Width = m_CurrentSize.x;
			m_Background->Height = m_CurrentSize.y;
		}

	protected:
		std::shared_ptr<World> m_World;
		int m_PlayerID;
		int m_CurrentSelection;
		static glm::vec2 m_SelectionCoordinates[4];
		static glm::vec2 m_SelectionSizes[4];

		glm::vec2 m_CurrentCoordinate;
		glm::vec2 m_TargetCoordinate;
		glm::vec2 m_CurrentSize;
		glm::vec2 m_TargetSize;
		float m_CurrentAlpha;

		TextureFrame* m_Background;

		bool OnCommand(const Events::InputCommand &event) override
		{
			if (Hidden())
				return false;

			if (event.PlayerID != m_PlayerID)
				return false;

			const float deadzone = 0.5f;

			// Menu movement
			if (event.Command == "interface_horizontal" || event.Command == "interface_vertical")
			{
				// Horizontal scrolling
				if (event.Command == "interface_horizontal")
				{
					if (event.Value > deadzone)
						m_CurrentSelection++;
					else if (event.Value < -deadzone)
						m_CurrentSelection--;
				}
				// Vertical scrolling to switch between "rows" in an intuitive way
				else if (event.Command == "interface_vertical")
				{
					if (event.Value > deadzone)
					{
						if (m_CurrentSelection == 0)
							m_CurrentSelection++;
						else if (m_CurrentSelection == 3)
							m_CurrentSelection--;
					}
					else if (event.Value < -deadzone)
					{
						if (m_CurrentSelection == 1)
							m_CurrentSelection--;
						else if (m_CurrentSelection == 2)
							m_CurrentSelection++;
					}
				}

				m_CurrentSelection = (m_CurrentSelection < 0) ? 0 : ((m_CurrentSelection > 3) ? 3 : m_CurrentSelection);

				std::stringstream ss;
				ss << "Textures/GUI/VehicleSelection/" << m_CurrentSelection + 1 << ".png";
				m_Background->FadeToTexture(ss.str(), 1.f);

				glm::vec2 scale = Scale();
				glm::vec2 coord = -m_SelectionCoordinates[m_CurrentSelection];
				m_TargetCoordinate = coord * scale + glm::vec2(Width / 2.f, Height / 2.f);
				glm::vec2 size = m_SelectionSizes[m_CurrentSelection];
				m_TargetSize = size * scale;
			}

			// Vehicle select
			if (event.Command == "interface_confirm" && event.Value > 0)
			{
				Events::SpawnVehicle e;
				e.PlayerID = m_PlayerID;
				if (m_CurrentSelection == 0)
					e.VehicleType = "Tank"; // HACK: Base on selected vehicle ID
				else if (m_CurrentSelection == 1)
					e.VehicleType = "Helicopter";
				else if (m_CurrentSelection == 2)
					e.VehicleType = "HRSV";
				else if (m_CurrentSelection == 3)
					e.VehicleType = "Jeep";
				EventBroker->Publish(e);
				Hide();
			}

			return true;
		}
	};

	glm::vec2 VehicleSelection::m_SelectionCoordinates[4] = {
		glm::vec2(339, 506),
		glm::vec2(639, 377),
		glm::vec2(1249, 399),
		glm::vec2(1535, 628)
	};

	glm::vec2 VehicleSelection::m_SelectionSizes[4] = {
		glm::vec2(1632, 918),
		glm::vec2(1793, 1009),
		glm::vec2(1793, 1009),
		glm::vec2(1854, 1043)
	};

}

#endif // GUI_VehicleSelection_h__
