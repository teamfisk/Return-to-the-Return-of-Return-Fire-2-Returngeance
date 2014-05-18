#ifndef Components_Viewport_h__
#define Components_Viewport_h__

#include "Component.h"

namespace Components
{

struct Viewport : Component
{
	Viewport()
		: Left(0.f)
		, Top(0.f)
		, Right(1.f)
		, Bottom(1.f) { }

	float Left;
	float Top;
	float Right;
	float Bottom;

	virtual Viewport* Clone() const override { return new Viewport(*this); }
};

}
#endif // Components_Viewport_h__