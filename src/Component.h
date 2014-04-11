#ifndef Component_h__
#define Component_h__

#include "Factory.h"
#include "Entity.h"

struct Component
{	EntityID Entity;
};

class ComponentFactory : public Factory<Component*> { };

#endif // Component_h__