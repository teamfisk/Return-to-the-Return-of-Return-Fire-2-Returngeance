#ifndef GUI_VehicleSelection_h__
#define GUI_VehicleSelection_h__

#include "GUI/Frame.h"
#include "GUI/HealthOverlay.h"

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
			m_CurrentSelection = 0;

			m_CurrentCoordinate = -m_SelectionCoordinates[0] + glm::vec2(Width / 2.f, Height / 2.f);
			m_TargetCoordinate = m_CurrentCoordinate;
			m_CurrentSize = m_SelectionSizes[0];
			m_TargetSize = m_CurrentSize;
			m_CurrentAlpha = 0.f;

			m_Background = new TextureFrame(this, "VehicleSelectImage1");
			m_Background->X = m_CurrentCoordinate.x;
			m_Background->Y = m_CurrentCoordinate.y;
			m_Background->Width = m_CurrentSize.x;
			m_Background->Height = m_CurrentSize.y;
			m_Background->SetTexture("Textures/GUI/VehicleSelection/1.png");
		}

		bool OnKeyUp(const Events::KeyUp &event) override
		{
			if (event.KeyCode == GLFW_KEY_LEFT)
			{
				m_CurrentSelection--;
			}
			else if (event.KeyCode == GLFW_KEY_RIGHT)
			{
				m_CurrentSelection++;
			}
			m_CurrentSelection = (m_CurrentSelection < 0) ? 0 : ((m_CurrentSelection > 3) ? 3 : m_CurrentSelection);

			std::stringstream ss;
			ss << "Textures/GUI/VehicleSelection/" << m_CurrentSelection + 1 << ".png";
			m_Background->FadeToTexture(ss.str(), 1.f);
			glm::vec2 coord = -m_SelectionCoordinates[m_CurrentSelection];
			m_TargetCoordinate = coord + glm::vec2(Width / 2.f, Height / 2.f);

			glm::vec2 size = m_SelectionSizes[m_CurrentSelection];
			m_TargetSize = size;

			return true;
		}

		void Update(double dt) override
		{
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
