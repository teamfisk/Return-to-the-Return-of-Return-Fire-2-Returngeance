#ifndef Components_Input_h__
#define Components_Input_h__

#include "Component.h"

namespace Components
{

struct Input : Component
{
	/*Input()
		: Keyboard(false)
		, Mouse(false)
		, GamepadID(0) { }

	bool Keyboard;
	bool Mouse;
	int GamepadID;*/

	virtual Input* Clone() const override { return new Input(*this); }
};

}

#endif // !Components_Input_h__