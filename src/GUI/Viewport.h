#ifndef GUI_Viewport_h__
#define GUI_Viewport_h__

#include <memory>

#include "GUI/Frame.h"

namespace GUI
{

class Viewport : public Frame
{
public:
	// Create a frame as a child
	Viewport(std::shared_ptr<Frame> parent)
		: Frame(parent) { }
};

}

#endif // GUI_Viewport_h__
