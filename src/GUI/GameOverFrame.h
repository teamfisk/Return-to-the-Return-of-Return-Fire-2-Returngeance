#ifndef GUI_GameOverFrame_h__
#define GUI_GameOverFrame_h__

#include "GUI/Frame.h"
#include "GUI/HealthOverlay.h"

namespace GUI
{

	class GameOverFrame : public Frame
	{
	public:
		GameOverFrame(Frame* parent, std::string name)
			: Frame(parent, name)
		{
			//Hide();
			m_Background = new TextureFrame(this, "Background");
			m_Background->SetTexture("Textures/GUI/black.png");
			m_Background->SetColor(glm::vec4(1.f, 1.f, 1.f, 0.6f));
			m_Text = new TextureFrame(this, "Message");
			m_Text->SetTexture("Textures/GUI/GloriousVictory.png");
		}

		void Show(bool victory)
		{
			if (victory)
				m_Text->SetTexture("Textures/GUI/GloriousVictory.png");
			else
				m_Text->SetTexture("Textures/GUI/ShamefulDefeat.png");

			Frame::Show();
		}

	protected:
		bool m_Victory;
		TextureFrame* m_Background;
		TextureFrame* m_Text;
	};
}

#endif // GUI_GameOverFrame_h__
