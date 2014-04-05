#ifndef Components_CustomShape_h__
#define Components_CustomShape_h__

#include "Component.h"
#include <string>

namespace Components
{

	struct CustomShape : Component
	{
		std::string fileName;
	};

}

#endif // !Components_CustomShape_h__
