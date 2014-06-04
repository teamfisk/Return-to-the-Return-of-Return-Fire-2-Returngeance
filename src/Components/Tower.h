#ifndef Tower_h__
#define Tower_h__

#include "Component.h"

namespace Components
{

	struct Tower : Component
	{
		Tower()
			: ID(0) { }

		int ID;
		EntityID GunBase;
		EntityID Gun;


		virtual Tower* Clone() const override { return new Tower(*this); }
	};

}

#endif // Tower_h__