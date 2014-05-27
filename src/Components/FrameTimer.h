#ifndef Components_FrameTimer_h__
#define Components_FrameTimer_h__

#include "Component.h"

namespace Components
{

	struct FrameTimer : public Component
	{
		int Frames;
		virtual FrameTimer* Clone() const override { return new FrameTimer(*this); }
	};

}

#endif // Components_FrameTimer_h__
