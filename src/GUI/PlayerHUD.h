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
		}

	protected:
		std::shared_ptr<World> m_World;
		int m_PlayerID;

		HealthOverlay* m_HealthOverlay;
	};

}

#endif // GUI_TextureFrame_h__
