#ifndef Player_h__
#define Player_h__

#include "Component.h"

namespace Components
{

struct Player : Component
{
	Player()
		: ID(0) { }

	int ID;

	virtual Player* Clone() const override { return new Player(*this); }
};

}

#endif // Player_h__