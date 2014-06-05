#ifndef Components_Team_h__
#define Components_Team_h__

#include "Component.h"
#include "Entity.h"

namespace Components
{

	struct Team : Component
	{
		unsigned int TeamID;

		virtual Team* Clone() const override { return new Team(*this); }
	};

}
#endif // !Components_Team_h__